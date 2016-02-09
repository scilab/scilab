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
#pragma comment(lib,"libboost_system-vc120-mt-gd-1_60.lib")
#pragma comment(lib,"libboost_random-vc120-mt-gd-1_60.lib")
#else //RELEASE
#pragma comment(lib,"sioclient.lib")
#pragma comment(lib,"libboost_system-vc120-mt-1_60.lib")
#pragma comment(lib,"libboost_random-vc120-mt-1_60.lib")
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

    s->on("callback", sio::socket::event_listener_aux([&](std::string const& name, sio::message::ptr const& data, bool isAck, sio::message::list &ack_resp){
        l.lock.lock();
        int uid = data->get_map()["uid"]->get_int();
        //get calltype type and callback instruction from MVC
        int cbtype = WebUtils::getIntProperty(uid, __GO_CALLBACKTYPE__);
        std::string cb = WebUtils::getStringProperty(uid, __GO_CALLBACK__);

        std::string str = "if exists(\"gcbo\") then %oldgcbo = gcbo; end;";
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
        l.cond.notify_all();
        l.lock.unlock();
    }));
}

void ScilabWebView::unregisterToController(void)
{
    org_scilab_modules_graphic_objects::CallGraphicController::unregisterScilabWebView(getScilabJavaVM());
}


void ScilabWebView::createObject(int uid)
{
    if (WebUtils::isManaged(uid) == false)
    {
        return;
    }

    if (WebUtils::isFigure(uid))
    {
        std::string str;
        WebUtils::createFigure(uid, str);
        std::cout << "createFigure" << str << std::endl;
        s->emit("graphic_create", str);
        return;
    }

    if (WebUtils::isUIcontrol(uid))
    {
        std::string str;
        WebUtils::createUIControl(uid, str);
        s->emit("graphic_create", str);
        return;
    }
}

void ScilabWebView::deleteObject(int uid)
{
    //if (WebUtils::isManaged(uid) == false)
    //{
    //    return;
    //}

    //sio::message::ptr obj = sio::object_message::create();
    //auto& m = obj->get_map();

    //m["uid"] = sio::int_message::create(uid);
    //s->emit("graphic_delete", obj);
}

void ScilabWebView::updateObject(int uid, int prop)
{
    if (WebUtils::isManaged(uid) == false)
    {
        return;
    }

    if (WebUtils::isFigure(uid))
    {
        switch (prop)
        {
            case __GO_SIZE__:
            {
                std::string str;
                WebUtils::setFigureSize(uid, str);
                s->emit("graphic_update", str);
                return;
            }
        }
    }

    if (WebUtils::isUIcontrol(uid))
    {
        switch (prop)
        {
            case __GO_PARENT__:
            {
                std::string str;
                WebUtils::setParent(uid, str);
                s->emit("graphic_update", str);
                return;
            }
            case __GO_POSITION__:
            {
                std::string str;
                WebUtils::setUIPosition(uid, str);
                s->emit("graphic_update", str);
                return;
            }
            case __GO_UI_STRING__:
            {
                std::string str;
                WebUtils::setUIString(uid, str);
                s->emit("graphic_update", str);
                return;
            }
        }
    }
}
