/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include <algorithm>

#include "Controller.hxx"

#include "LoggerView.hxx"

namespace org_scilab_modules_scicos
{

/*
 * Implement SharedData methods
 */
Controller::SharedData::SharedData() :
    model(), allViews()
{
    LoggerView* v = new LoggerView();
    allViews.push_back(v);
}

Controller::SharedData::~SharedData()
{
    for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); ++iter)
    {
        delete *iter;
    }
}

Controller::SharedData* Controller::_instance = 0;

void Controller::delete_all_instances()
{
    if (_instance == 0)
    {
        return;
    }

    delete _instance;
}

void Controller::register_view(View* v)
{
    if (_instance == 0)
    {
        _instance = new SharedData();
    }

    _instance->allViews.push_back(v);
}

void Controller::unregister_view(View* v)
{
    if (_instance == 0)
    {
        return;
    }

    view_set_t::iterator it = std::find(_instance->allViews.begin(), _instance->allViews.end(), v);
    if (it != _instance->allViews.end())
    {
        _instance->allViews.erase(it);
    }
}

Controller::Controller()
{
    if (_instance == 0)
    {
        _instance = new SharedData();
    }
}

Controller::Controller(const Controller& c)
{
    // _instance is already initialized

    // silent unused parameter warnings
    (void) c;
}

Controller::~Controller()
{
}

ScicosID Controller::createObject(kind_t k)
{
    ScicosID id = _instance->model.createObject(k);

    for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); ++iter)
    {
        (*iter)->objectCreated(id, k);
    }

    return id;
}

static void unlink_vector(Controller& controller, ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop)
{
    ScicosID v;
    controller.getObjectProperty(uid, k, uid_prop, v);
    if (v != 0)
    {
        auto o = controller.getObject(v);

        std::vector<ScicosID> children;
        controller.getObjectProperty(o->id(), o->kind(), ref_prop, children);

        std::vector<ScicosID>::iterator it = std::find(children.begin(), children.end(), uid);
        if (it != children.end())
        {
            children.erase(it);
        }

        controller.setObjectProperty(o->id(), o->kind(), ref_prop, children);
    }
}

static void unlink(Controller& controller, ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop)
{
    ScicosID v;
    controller.getObjectProperty(uid, k, uid_prop, v);
    if (v != 0)
    {
        auto o = controller.getObject(v);
        controller.setObjectProperty(o->id(), o->kind(), ref_prop, 0);
    }
}

void Controller::deleteObject(ScicosID uid)
{
    // disconnect / remove references first
    auto initial = getObject(uid);
    const kind_t k = initial->kind();
    if (k == ANNOTATION)
    {
        unlink_vector(*this, uid, k, PARENT_DIAGRAM, CHILDREN);
        // RELATED_TO is not referenced back
    }
    else if (k == BLOCK)
    {
        unlink_vector(*this, uid, k, PARENT_DIAGRAM, CHILDREN);
        // INPUTS will be removed on delete
        // OUTPUTS will be removed on delete
        // EVENT_INPUTS will be removed on delete
        // EVENT_OUTPUTS will be removed on delete
        unlink_vector(*this, uid, k, PARENT_BLOCK, CHILDREN);
        // CHILDREN will be removed on delete
        // FIXME what about REFERENCED_PORT ?
    }
    else if (k == DIAGRAM)
    {
        // CHILDREN will be removed on delete
    }
    else if (k == LINK)
    {
        unlink_vector(*this, uid, k, PARENT_DIAGRAM, CHILDREN);
        unlink_vector(*this, uid, k, SOURCE_PORT, CONNECTED_SIGNALS);
        unlink_vector(*this, uid, k, DESTINATION_PORT, CONNECTED_SIGNALS);
    }
    else if (k == PORT)
    {
        unlink(*this, uid, k, SOURCE_BLOCK, INPUTS);
        unlink(*this, uid, k, SOURCE_BLOCK, OUTPUTS);
        unlink(*this, uid, k, SOURCE_BLOCK, EVENT_INPUTS);
        unlink(*this, uid, k, SOURCE_BLOCK, EVENT_OUTPUTS);

        unlink(*this, uid, k, CONNECTED_SIGNALS, SOURCE_PORT);
        unlink(*this, uid, k, CONNECTED_SIGNALS, DESTINATION_PORT);
    }

    // delete the object
    _instance->model.deleteObject(uid);

    for (view_set_t::iterator iter = _instance->allViews.begin(); iter != _instance->allViews.end(); ++iter)
    {
        (*iter)->objectDeleted(uid, k);
    }
}

ScicosID Controller::cloneObject(std::map<ScicosID, ScicosID>& mapped, ScicosID uid)
{
    auto initial = getObject(uid);
    const kind_t k = initial->kind();
    ScicosID o = createObject(k);
    mapped.insert(std::make_pair(uid, o));

    // Get then set all properties per type that do not manage ScicosID
    cloneProperties<double>(initial, o);
    cloneProperties<int>(initial, o);
    cloneProperties<bool>(initial, o);
    cloneProperties<std::string>(initial, o);
    cloneProperties<std::vector<double> >(initial, o);
    cloneProperties<std::vector<int> >(initial, o);
    cloneProperties<std::vector<std::string> >(initial, o);

    // deep copy children, manage ScicosID and std::vector<ScicosID>
    if (k == ANNOTATION)
    {
        deepClone(mapped, uid, o, k, PARENT_DIAGRAM, false);
        deepClone(mapped, uid, o, k, RELATED_TO, true);
    }
    else if (k == BLOCK)
    {
        deepClone(mapped, uid, o, k, PARENT_DIAGRAM, false);
        deepCloneVector(mapped, uid, o, k, INPUTS, true);
        deepCloneVector(mapped, uid, o, k, OUTPUTS, true);
        deepCloneVector(mapped, uid, o, k, EVENT_INPUTS, true);
        deepCloneVector(mapped, uid, o, k, EVENT_OUTPUTS, true);
        deepClone(mapped, uid, o, k, PARENT_BLOCK, false);
        deepCloneVector(mapped, uid, o, k, CHILDREN, true);
        // FIXME what about REFERENCED_PORT ?
    }
    else if (k == DIAGRAM)
    {
        deepCloneVector(mapped, uid, o, k, CHILDREN, true);
    }
    else if (k == LINK)
    {
        deepClone(mapped, uid, o, k, PARENT_DIAGRAM, false);
        deepClone(mapped, uid, o, k, SOURCE_PORT, true);
        deepClone(mapped, uid, o, k, DESTINATION_PORT, true);
    }
    else if (k == PORT)
    {
        deepClone(mapped, uid, o, k, SOURCE_BLOCK, true);
        deepCloneVector(mapped, uid, o, k, CONNECTED_SIGNALS, true);
    }

    return o;
}

void Controller::deepClone(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound)
{
    ScicosID v;
    getObjectProperty(uid, k, p, v);

    ScicosID cloned = 0;

    std::map<ScicosID, ScicosID>::iterator it = mapped.find(v);
    if (it != mapped.end())
    {
        cloned = it->second;
    }
    else
    {
        if (cloneIfNotFound)
        {
            if (v != 0)
            {
                cloned = cloneObject(mapped, v);
            }
            else
            {
                cloned = 0;
            }
        }
        else
        {
            cloned = 0;
        }
    }

    setObjectProperty(clone, k, p, cloned);
}

void Controller::deepCloneVector(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound)
{
    std::vector<ScicosID> v;
    getObjectProperty(uid, k, p, v);

    std::vector<ScicosID> cloned;
    cloned.reserve(v.size());

    for (const ScicosID& id : v)
    {

        std::map<ScicosID, ScicosID>::iterator it = mapped.find(id);
        if (it != mapped.end())
        {
            cloned.push_back(it->second);
        }
        else
        {
            if (cloneIfNotFound)
            {
                if (id != 0)
                {
                    cloned.push_back(cloneObject(mapped, id));
                }
                else
                {
                    cloned.push_back(0);
                }
            }
            else
            {
                cloned.push_back(0);
            }
        }
    }

    setObjectProperty(clone, k, p, cloned);
}

ScicosID Controller::cloneObject(ScicosID uid)
{
    std::map<ScicosID, ScicosID> mapped;
    return cloneObject(mapped, uid);
}

std::shared_ptr<model::BaseObject> Controller::getObject(ScicosID uid) const
{
    return _instance->model.getObject(uid);
}

} /* namespace org_scilab_modules_scicos */
