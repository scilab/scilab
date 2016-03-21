/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2016 - Scilab Enterprises - Antoine ELIAS
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#include "ScilabWebView.hxx"
#include "CallGraphicController.hxx"
#include "configvariable.hxx"
#include "webutils.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "storeCommand.h"
}

#ifdef _MSC_VER
#ifdef _DEBUG
#pragma comment(lib,"sioclientd.lib")
#else
#pragma comment(lib,"sioclient.lib")
#endif
#endif

ScilabWebView* ScilabWebView::me = nullptr;

void ScilabNativeWebView__createObject(int uid)
{
    ScilabWebView::getInstance()->createObject(uid);
}

void ScilabNativeWebView__deleteObject(int uid)
{
    ScilabWebView::getInstance()->deleteObject(uid);
}

void ScilabNativeWebView__updateObject(int uid, int prop)
{
    ScilabWebView::getInstance()->updateObject(uid, prop);
}

void ScilabWebView::registerToController(void)
{
    if (ConfigVariable::getGraphicAddress().empty())
    {
        return;
    }

    //register view in graphic MVC
    org_scilab_modules_graphic_objects::CallGraphicController::registerScilabWebView(getScilabJavaVM());

    //set and start sioclient
    //bind main event on socket
    client.set_open_listener(std::bind(&graphic_listener::on_connected, &l));
    client.set_close_listener(std::bind(&graphic_listener::on_close, &l, std::placeholders::_1));
    client.set_fail_listener(std::bind(&graphic_listener::on_fail, &l));

    //connect it
    client.connect(ConfigVariable::getGraphicAddress());

    //wait connection OK
    l.lock.lock();
    if (!l.connect_finish)
    {
        l.cond.wait(l.lock);
    }

    l.lock.unlock();

    //get connected socket
    s = client.socket();

    s->on("imagepath", sio::socket::event_listener_aux([&](std::string const & name, sio::message::ptr const & data, bool isAck, sio::message::list & ack_resp)
    {
        l.lock.lock();
        std::string path = (std::string)data->get_map()["path"]->get_string();
        WebUtils::setImagePath(path);
        l.cond.notify_all();
        l.lock.unlock();
    }));

    s->on("callback", sio::socket::event_listener_aux([&](std::string const & name, sio::message::ptr const & data, bool isAck, sio::message::list & ack_resp)
    {
        l.lock.lock();
        int uid = (int)data->get_map()["uid"]->get_int();

        switch (WebUtils::getStyle(uid))
        {
            case __GO_UI_PUSHBUTTON__:
                //nothing to update, exec callback only
                break;
            case __GO_UI_CHECKBOX__:
            case __GO_UI_RADIOBUTTON__:
            {
                //get checked status
                bool status = data->get_map()["value"]->get_bool();
                WebUtils::updateValue(uid, status);
                break;
            }
            case __GO_UI_POPUPMENU__:
            case __GO_UI_LISTBOX__:
            {
                std::vector<double> d;
                std::vector<sio::message::ptr> v = data->get_map()["value"]->get_vector();
                for (auto it : v)
                {
                    d.push_back(it->get_double() + 1); //1 indexed
                }

                WebUtils::updateValue(uid, d);
                break;
            }
            break;
            case __GO_UI_EDIT__:
            {
                std::string value = (std::string)data->get_map()["value"]->get_string();
                WebUtils::updateValue(uid, value);
                break;
            }
            case __GO_UI_SLIDER__:
            case __GO_UI_SPINNER__:
            {
                double value = (double)data->get_map()["value"]->get_double();
                WebUtils::updateValue(uid, value);
                break;
            }

            case __GO_UI_TAB__:
            {
                double value = (double)data->get_map()["value"]->get_int();
                WebUtils::updateValue(uid, value);
                break;
            }
        }

        l.cond.notify_all();
        l.lock.unlock();

        //get calltype type and callback instruction from MVC
        int cbtype = WebUtils::getIntProperty(uid, __GO_CALLBACKTYPE__);
        std::string cb = WebUtils::getStringProperty(uid, __GO_CALLBACK__);

        if (cbtype == -1)
        {
            return;
        }

        std::string str;
        str = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;";
        str += "gcbo = getcallbackobject(" + std::to_string(uid) + ");";
        str += cb;
        str += ";if exists(\"%oldgcbo\") then gcbo = %oldgcbo; else clear gcbo; end;";

        if (cbtype == 10 || cbtype == 12)
        {
            StorePrioritaryCommand((char*)str.data());
        }
        else
        {
            StoreCommand((char*)str.data());
        }
    }));

    WebUtils::fillSetter();
}

void ScilabWebView::unregisterToController(void)
{
    org_scilab_modules_graphic_objects::CallGraphicController::unregisterScilabWebView(getScilabJavaVM());

    client.sync_close();
    client.clear_con_listeners();

}


void ScilabWebView::createObject(int uid)
{
    if (WebUtils::isManaged(uid) == false)
    {
        return;
    }

    if (WebUtils::isFigure(uid))
    {
        std::ostringstream ostr;
        WebUtils::createFigure(uid, ostr);
        s->emit("graphic_create", ostr.str());
        return;
    }

    if (WebUtils::isUIcontrol(uid))
    {
        std::ostringstream ostr;
        WebUtils::createUIControl(uid, ostr);
        WebUtils::updateDefaultProperties(uid, ostr);
        s->emit("graphic_create", ostr.str());
        return;
    }
}

void ScilabWebView::deleteObject(int uid)
{
    if (WebUtils::isManaged(uid) == false)
    {
        return;
    }

    std::ostringstream ostr;
    WebUtils::deleteObject(uid, ostr);
    s->emit("graphic_delete", ostr.str());
}

void ScilabWebView::updateObject(int uid, int prop)
{
    if (WebUtils::isManaged(uid) == false)
    {
        return;
    }

    if (prop == __GO_VISIBLE__)
    {
        std::ostringstream ostr;
        WebUtils::setVisible(uid, ostr);
        s->emit("graphic_update", ostr.str());
        return;
    }

    if (WebUtils::isFigure(uid))
    {
        switch (prop)
        {
            case __GO_AXES_SIZE__:
            {
                std::ostringstream ostr;
                WebUtils::setFigureSize(uid, ostr);
                s->emit("graphic_update", ostr.str());
                return;
            }
            case __GO_LAYOUT__:
            {
                std::ostringstream ostr;
                WebUtils::setUILayout(uid, ostr);
                s->emit("graphic_update", ostr.str());
                return;
            }
        }
    }

    if (WebUtils::isUIcontrol(uid))
    {
        if (WebUtils::hasValidParent(uid) || prop == __GO_PARENT__)
        {
            std::ostringstream ostr;
            if (WebUtils::set(prop, uid, ostr))
            {
                s->emit("graphic_update", ostr.str());
            }
        }
        else
        {
            //parent is not already set, put update in waiting queue
            WebUtils::addInWaitingQueue(uid, prop);
        }
        return;
    }
}
