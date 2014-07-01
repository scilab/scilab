/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <algorithm>

#include "Controller.hxx"

#include "LoggerView.hxx"

namespace org_scilab_modules_scicos
{

Controller* Controller::_instance = 0;

Controller* Controller::get_instance()
{
    if (_instance == 0)
    {
        _instance = new Controller();
    }
    return _instance;
}

void Controller::delete_instance()
{
    if (_instance != 0)
    {
        delete _instance;
    }
}

Controller::Controller()
{
    LoggerView* v = new LoggerView();

    register_view(v);
}

Controller::~Controller()
{
    for (view_set_t::iterator iter = allViews.begin(); iter != allViews.end(); iter++)
    {
        delete *iter;
    }
    allViews.clear();
}

void Controller::register_view(View* v)
{
    allViews.push_back(v);
}

void Controller::unregister_view(View* v)
{
    view_set_t::iterator it = std::find(allViews.begin(), allViews.end(), v);
    if (it != allViews.end())
    {
        allViews.erase(it);
    }
}

ScicosID Controller::createObject(kind_t k)
{
    ScicosID id = model.createObject(k);

    for (view_set_t::iterator iter = allViews.begin(); iter != allViews.end(); iter++)
    {
        (*iter)->objectCreated(id, k);
    }

    return id;
}

void Controller::deleteObject(ScicosID uid)
{
    model.deleteObject(uid);

    for (view_set_t::iterator iter = allViews.begin(); iter != allViews.end(); iter++)
    {
        (*iter)->objectDeleted(uid);
    }
}

model::BaseObject* Controller::getObject(ScicosID uid)
{
    return model.getObject(uid);
}

update_status_t Controller::setObject(model::BaseObject* o)
{
    update_status_t status = model.setObject(o);

    if (status == SUCCESS)
    {
        for (view_set_t::iterator iter = allViews.begin(); iter != allViews.end(); iter++)
        {
            (*iter)->objectUpdated(o->id(), o->kind());
        }
    }
    return status;
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
                                   double* v)
{
    return model.getObjectProperty(uid, k, p, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int* v)
{
    return model.getObjectProperty(uid, k, p, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool* v)
{
    return model.getObjectProperty(uid, k, p, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
                                   std::string* v)
{
    return model.getObjectProperty(uid, k, p, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
                                   ScicosID* v)
{
    return model.getObjectProperty(uid, k, p, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len,
                                   double** v)
{
    return model.getObjectProperty(uid, k, p, len, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
                                   size_t* len, int** v)
{
    return model.getObjectProperty(uid, k, p, len, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
                                   size_t* len, std::string** v)
{
    return model.getObjectProperty(uid, k, p, len, v);
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
                                   size_t* len, ScicosID** v)
{
    return model.getObjectProperty(uid, k, p, len, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        double v)
{
    return generic_setObjectProp<double>(uid, k, p, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        int v)
{
    return generic_setObjectProp<int>(uid, k, p, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        bool v)
{
    return generic_setObjectProp<bool>(uid, k, p, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        ScicosID v)
{
    return generic_setObjectProp<ScicosID>(uid, k, p, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        std::string v)
{
    return generic_setObjectProp<std::string>(uid, k, p, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        size_t len, double* v)
{
    return generic_setObjectProp<double>(uid, k, p, len, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        size_t len, int* v)
{
    return generic_setObjectProp<int>(uid, k, p, len, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        size_t len, std::string* v)
{
    return generic_setObjectProp<std::string>(uid, k, p, len, v);
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p,
        size_t len, ScicosID* v)
{
    return generic_setObjectProp<ScicosID>(uid, k, p, len, v);
}

} /* namespace org_scilab_modules_scicos */
