/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
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

#include <atomic>
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <algorithm>
#include <iterator>

#include "utilities.hxx"

#include "Controller.hxx"
#include "model/BaseObject.hxx"

#include "LoggerView.hxx"

extern "C" {
#include "scicos.h"
}

namespace org_scilab_modules_scicos
{

/*
 * Implement SharedData methods
 */
Controller::SharedData::SharedData() :
    onModelStructuralModification(), model(),
    onViewsStructuralModification(), allNamedViews(), allViews()
{
    unlock(&onModelStructuralModification);
    unlock(&onViewsStructuralModification);
}

Controller::SharedData::~SharedData()
{
    lock(&onViewsStructuralModification);
    for (view_set_t::iterator iter = allViews.begin(); iter != allViews.end(); ++iter)
    {
        delete *iter;
    }
    unlock(&onViewsStructuralModification);
}

Controller::SharedData Controller::m_instance;

View* Controller::register_view(const std::string& name, View* v)
{
    lock(&m_instance.onViewsStructuralModification);

    m_instance.allNamedViews.push_back(name);
    m_instance.allViews.push_back(v);

    unlock(&m_instance.onViewsStructuralModification);
    return v;
}

void Controller::unregister_view(View* v)
{
    lock(&m_instance.onViewsStructuralModification);

    view_set_t::iterator it = std::find(m_instance.allViews.begin(), m_instance.allViews.end(), v);
    if (it != m_instance.allViews.end())
    {
        size_t d = std::distance(m_instance.allViews.begin(), it);
        m_instance.allNamedViews.erase(m_instance.allNamedViews.begin() + d);
        m_instance.allViews.erase(m_instance.allViews.begin() + d);
    }

    unlock(&m_instance.onViewsStructuralModification);
}

View* Controller::unregister_view(const std::string& name)
{
    View* view = nullptr;

    lock(&m_instance.onViewsStructuralModification);

    view_name_set_t::iterator it = std::find(m_instance.allNamedViews.begin(), m_instance.allNamedViews.end(), name);
    if (it != m_instance.allNamedViews.end())
    {
        size_t d = std::distance(m_instance.allNamedViews.begin(), it);
        view = *(m_instance.allViews.begin() + d);
        m_instance.allNamedViews.erase(m_instance.allNamedViews.begin() + d);
        m_instance.allViews.erase(m_instance.allViews.begin() + d);
    }
    unlock(&m_instance.onViewsStructuralModification);

    return view;
}

View* Controller::look_for_view(const std::string& name)
{
    View* view = nullptr;

    lock(&m_instance.onViewsStructuralModification);
    view_name_set_t::iterator it = std::find(m_instance.allNamedViews.begin(), m_instance.allNamedViews.end(), name);
    if (it != m_instance.allNamedViews.end())
    {
        size_t d = std::distance(m_instance.allNamedViews.begin(), it);
        view = *(m_instance.allViews.begin() + d);
    }
    unlock(&m_instance.onViewsStructuralModification);

    return view;
}

void Controller::end_simulation()
{
    end_scicos_sim();
}

Controller::Controller()
{
}

Controller::~Controller()
{
}

ScicosID Controller::createObject(kind_t k)
{
    lock(&m_instance.onModelStructuralModification);
    ScicosID uid = m_instance.model.createObject(k);
    unlock(&m_instance.onModelStructuralModification);

    lock(&m_instance.onViewsStructuralModification);
    for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
    {
        (*iter)->objectCreated(uid, k);
    }
    unlock(&m_instance.onViewsStructuralModification);

    return uid;
}

unsigned Controller::referenceObject(const ScicosID uid) const
{
    lock(&m_instance.onModelStructuralModification);

    unsigned refCount = m_instance.model.referenceObject(uid);

    auto o = m_instance.model.getObject(uid);
    unlock(&m_instance.onModelStructuralModification);
    if (o == nullptr)
    {
        // defensive programming
        return 0u;
    }

    lock(&m_instance.onViewsStructuralModification);
    for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
    {
        (*iter)->objectReferenced(uid, o->kind(), refCount);
    }
    unlock(&m_instance.onViewsStructuralModification);

    return refCount;
}

void Controller::deleteObject(ScicosID uid)
{
    // if this object is the empty uid, ignore it : is is not stored in the model
    if (uid == ScicosID())
    {
        return;
    }

    lock(&m_instance.onModelStructuralModification);

    auto initial = m_instance.model.getObject(uid);
    if (initial == nullptr)
    {
        // defensive programming
        unlock(&m_instance.onModelStructuralModification);
        return;
    }
    const kind_t k = initial->kind();

    // if this object has been referenced somewhere else do not delete it but decrement the reference counter
    unsigned& refCount = m_instance.model.referenceCount(uid);
    unlock(&m_instance.onModelStructuralModification);
    if (refCount > 0)
    {
        --refCount;

        lock(&m_instance.onViewsStructuralModification);
        for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
        {
            (*iter)->objectUnreferenced(uid, k, refCount);
        }
        unlock(&m_instance.onViewsStructuralModification);
        return;
    }

    // We need to delete this object and cleanup all the referenced model object

    // disconnect / remove references of weak connected objects and decrement the reference count of all strongly connected objects.
    if (k == ANNOTATION)
    {
        unlinkVector(initial, PARENT_DIAGRAM, CHILDREN);
        unlinkVector(initial, PARENT_BLOCK, CHILDREN);
        // RELATED_TO is not referenced back
    }
    else if (k == BLOCK)
    {
        unlinkVector(initial, PARENT_DIAGRAM, CHILDREN);
        unlinkVector(initial, PARENT_BLOCK, CHILDREN);

        deleteVector(initial, INPUTS);
        deleteVector(initial, OUTPUTS);
        deleteVector(initial, EVENT_INPUTS);
        deleteVector(initial, EVENT_OUTPUTS);

        unlink(initial, CHILDREN, PARENT_BLOCK);
        deleteVector(initial, CHILDREN);
        // FIXME what about REFERENCED_PORT ?
    }
    else if (k == DIAGRAM)
    {
        unlink(initial, CHILDREN, PARENT_DIAGRAM);
        deleteVector(initial, CHILDREN);
    }
    else if (k == LINK)
    {
        unlinkVector(initial, PARENT_DIAGRAM, CHILDREN);
        unlinkVector(initial, PARENT_BLOCK, CHILDREN);

        unlinkVector(initial, SOURCE_PORT, CONNECTED_SIGNALS);
        unlinkVector(initial, DESTINATION_PORT, CONNECTED_SIGNALS);
    }
    else if (k == PORT)
    {
        unlinkVector(initial, SOURCE_BLOCK, INPUTS);
        unlinkVector(initial, SOURCE_BLOCK, OUTPUTS);
        unlinkVector(initial, SOURCE_BLOCK, EVENT_INPUTS);
        unlinkVector(initial, SOURCE_BLOCK, EVENT_OUTPUTS);

        unlink(initial, CONNECTED_SIGNALS, SOURCE_PORT);
        unlink(initial, CONNECTED_SIGNALS, DESTINATION_PORT);
    }

    // delete the object
    lock(&m_instance.onModelStructuralModification);
    m_instance.model.deleteObject(uid);
    unlock(&m_instance.onModelStructuralModification);

    lock(&m_instance.onViewsStructuralModification);
    for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
    {
        (*iter)->objectDeleted(uid, k);
    }
    unlock(&m_instance.onViewsStructuralModification);
}

void Controller::unlinkVector(model::BaseObject* initial, object_properties_t uid_prop, object_properties_t ref_prop)
{
    ScicosID v;
    getObjectProperty(initial->id(), initial->kind(), uid_prop, v);
    if (v != ScicosID())
    {
        auto o = getObject(v);
        if (o == nullptr)
        {
            return;
        }

        std::vector<ScicosID> children;
        getObjectProperty(o->id(), o->kind(), ref_prop, children);

        std::vector<ScicosID>::iterator it = std::find(children.begin(), children.end(), initial->id());
        if (it != children.end())
        {
            children.erase(it);
        }

        setObjectProperty(o->id(), o->kind(), ref_prop, children);
    }
}

void Controller::unlink(model::BaseObject* initial, object_properties_t uid_prop, object_properties_t ref_prop)
{
    std::vector<ScicosID> v;
    getObjectProperty(initial->id(), initial->kind(), uid_prop, v);
    for (const ScicosID id : v)
    {
        if (id != ScicosID())
        {
            auto o = getObject(id);
            if (o == nullptr)
            {
                continue;
            }

            // Find which end of the link is connected to the port
            ScicosID oppositeRef;
            getObjectProperty(o->id(), o->kind(), ref_prop, oppositeRef);
            if (oppositeRef == initial->id())
            {
                setObjectProperty(o->id(), o->kind(), ref_prop, ScicosID());
            }
        }
    }
}

void Controller::deleteVector(model::BaseObject* initial, object_properties_t uid_prop)
{
    std::vector<ScicosID> children;
    getObjectProperty(initial->id(), initial->kind(), uid_prop, children);

    for (ScicosID id : children)
    {
        deleteObject(id);
    }
}

template<typename T>
void Controller::cloneProperties(model::BaseObject* initial, model::BaseObject* clone)
{
    for (int i = 0; i < MAX_OBJECT_PROPERTIES; ++i)
    {
        enum object_properties_t p = static_cast<enum object_properties_t>(i);

        T value;
        bool status = getObjectProperty(initial, p, value);
        if (status)
        {
            setObjectProperty(clone, p, value);
        }
    }
}

model::BaseObject* Controller::cloneObject(std::map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, bool cloneChildren, bool clonePorts)
{
    const kind_t k = initial->kind();
    ScicosID o = createObject(k);
    model::BaseObject* cloned = getObject(o);
    mapped.insert(std::make_pair(initial, cloned));

    lock(&m_instance.onViewsStructuralModification);
    for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
    {
        (*iter)->objectCloned(initial->id(), o, k);
    }
    unlock(&m_instance.onViewsStructuralModification);

    // Get then set all properties per type that do not manage ScicosID
    cloneProperties<double>(initial, cloned);
    cloneProperties<int>(initial, cloned);
    cloneProperties<bool>(initial, cloned);
    cloneProperties<std::string>(initial, cloned);
    cloneProperties<std::vector<double> >(initial, cloned);
    cloneProperties<std::vector<int> >(initial, cloned);
    cloneProperties<std::vector<std::string> >(initial, cloned);

    // deep copy children, manage ScicosID and std::vector<ScicosID>
    if (k == ANNOTATION)
    {
        deepClone(mapped, initial, cloned, PARENT_DIAGRAM, false);
        deepClone(mapped, initial, cloned, PARENT_BLOCK, false);
        deepClone(mapped, initial, cloned, RELATED_TO, true);
    }
    else if (k == BLOCK)
    {
        deepClone(mapped, initial, cloned, PARENT_DIAGRAM, false);
        if (clonePorts)
        {
            // copy the block and all its ports
            deepCloneVector(mapped, initial, cloned, INPUTS, true);
            deepCloneVector(mapped, initial, cloned, OUTPUTS, true);
            deepCloneVector(mapped, initial, cloned, EVENT_INPUTS, true);
            deepCloneVector(mapped, initial, cloned, EVENT_OUTPUTS, true);
        }

        deepClone(mapped, initial, cloned, PARENT_BLOCK, false);
        if (cloneChildren)
        {
            deepCloneVector(mapped, initial, cloned, CHILDREN, true);
        }
        // FIXME what about REFERENCED_PORT ?
    }
    else if (k == DIAGRAM)
    {
        if (cloneChildren)
        {
            deepCloneVector(mapped, initial, cloned, CHILDREN, true);
        }
    }
    else if (k == LINK)
    {
        deepClone(mapped, initial, cloned, PARENT_DIAGRAM, false);
        deepClone(mapped, initial, cloned, PARENT_BLOCK, false);
        deepClone(mapped, initial, cloned, SOURCE_PORT, false);
        deepClone(mapped, initial, cloned, DESTINATION_PORT, false);
    }
    else if (k == PORT)
    {
        deepClone(mapped, initial, cloned, SOURCE_BLOCK, false);
        deepCloneVector(mapped, initial, cloned, CONNECTED_SIGNALS, false);
    }
    return cloned;
}

void Controller::deepClone(std::map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, model::BaseObject* clone, object_properties_t p, bool cloneIfNotFound)
{
    ScicosID v;
    getObjectProperty(initial, p, v);

    model::BaseObject* opposite = getObject(v);
    model::BaseObject* cloned;

    std::map<model::BaseObject*, model::BaseObject*>::iterator it = mapped.find(opposite);
    if (it != mapped.end())
    {
        cloned = it->second;
    }
    else
    {
        if (cloneIfNotFound)
        {
            if (v != ScicosID())
            {
                cloned = cloneObject(mapped, opposite, true, true);
            }
            else
            {
                cloned = nullptr;
            }
        }
        else
        {
            cloned = nullptr;
        }
    }

    if (cloned == nullptr)
    {
        setObjectProperty(clone, p, ScicosID());
    }
    else
    {
        setObjectProperty(clone, p, cloned->id());
    }
}

void Controller::deepCloneVector(std::map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, model::BaseObject* clone, object_properties_t p, bool cloneIfNotFound)
{
    std::vector<ScicosID> v;
    getObjectProperty(initial, p, v);

    std::vector<model::BaseObject*> cloned;
    cloned.reserve(v.size());

    for (const ScicosID & id : v)
    {
        if (id == ScicosID())
        {
            // Deleted Block, the cloning is done at Adapter level
            cloned.push_back(nullptr);
            continue;
        }

        model::BaseObject* opposite = getObject(id);
        std::map<model::BaseObject*, model::BaseObject*>::iterator it = mapped.find(opposite);
        if (it != mapped.end())
        {
            cloned.push_back(it->second);
        }
        else
        {
            if (cloneIfNotFound)
            {
                if (id != ScicosID())
                {
                    model::BaseObject* clone = cloneObject(mapped, opposite, true, true);
                    cloned.push_back(clone);
                }
                else
                {
                    cloned.push_back(nullptr);
                }
            }
            else
            {
                cloned.push_back(nullptr);
            }
        }
    }

    // update the ScicosID related properties after cloning all the objects
    if (p == CHILDREN)
    {
        for (auto const & it : mapped)
        {
            model::BaseObject* initial = it.first;
            model::BaseObject* cloned = it.second;

            switch (initial->kind())
            {
                case PORT:
                    deepCloneVector(mapped, initial, cloned, CONNECTED_SIGNALS, false);
                    break;
                case LINK:
                    deepClone(mapped, initial, cloned, SOURCE_PORT, false);
                    deepClone(mapped, initial, cloned, DESTINATION_PORT, false);
                    break;
                default:
                    break;
            }
        }
    }

    // set the main object vector property
    std::vector<ScicosID> clonedUIDs(cloned.size());
    std::transform(cloned.begin(), cloned.end(), clonedUIDs.begin(), [](model::BaseObject * o)
    {
        if (o == nullptr)
        {
            return ScicosID();
        }
        else
        {
            return o->id();
        }
    });

    setObjectProperty(clone, p, clonedUIDs);
}

ScicosID Controller::cloneObject(ScicosID uid, bool cloneChildren, bool clonePorts)
{
    std::map<model::BaseObject*, model::BaseObject*> mapped;
    model::BaseObject* clone = cloneObject(mapped, getObject(uid), cloneChildren, clonePorts);

    return clone->id();
}

kind_t Controller::getKind(ScicosID uid) const
{
    lock(&m_instance.onModelStructuralModification);

    kind_t kind = m_instance.model.getKind(uid);

    unlock(&m_instance.onModelStructuralModification);
    return kind;
}

std::vector<ScicosID> Controller::getAll(kind_t k) const
{
    lock(&m_instance.onModelStructuralModification);

    auto vec = m_instance.model.getAll(k);

    unlock(&m_instance.onModelStructuralModification);
    return vec;
}

void Controller::sortAndFillKind(std::vector<ScicosID>& uids, std::vector<int>& kinds)
{
    lock(&m_instance.onModelStructuralModification);

    // create a container of pair
    struct local_pair
    {
        ScicosID first;
        int second;
    };
    std::vector<local_pair> container(uids.size());

    // fill it
    for (size_t i = 0; i < uids.size(); ++i)
    {
        container[i] = { uids[i], m_instance.model.getKind(uids[i]) };
    }

    // sort according to the kinds
    std::stable_sort(container.begin(), container.end(), [] (const local_pair & a, const local_pair & b)
    {
        return a.second < b.second;
    });

    // move things back
    uids.clear();
    kinds.reserve(uids.capacity());
    for (const auto & v : container)
    {
        uids.push_back(v.first);
        kinds.push_back(v.second);
    }

    unlock(&m_instance.onModelStructuralModification);
}

template<typename T>
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T& v) const
{
    lock(&m_instance.onModelStructuralModification);
    bool ret = m_instance.model.getObjectProperty(uid, k, p, v);
    unlock(&m_instance.onModelStructuralModification);
    return ret;
}

bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<double>& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<int>& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<bool>& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector< std::string >& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}
bool Controller::getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<ScicosID>& v) const
{
    return getObjectProperty<>(uid, k, p, v);
}

template<typename T>
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T v)
{
    lock(&m_instance.onModelStructuralModification);
    update_status_t status = m_instance.model.setObjectProperty(uid, k, p, v);
    unlock(&m_instance.onModelStructuralModification);

    lock(&m_instance.onViewsStructuralModification);
    for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
    {
        (*iter)->propertyUpdated(uid, k, p, status);
    }
    unlock(&m_instance.onViewsStructuralModification);
    return status;
}

update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::string& v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<double>& v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<int>& v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<bool>& v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector< std::string >& v)
{
    return setObjectProperty<>(uid, k, p, v);
}
update_status_t Controller::setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<ScicosID>& v)
{
    return setObjectProperty<>(uid, k, p, v);
}

model::BaseObject* Controller::getObject(ScicosID uid) const
{
    lock(&m_instance.onModelStructuralModification);
    model::BaseObject* o = m_instance.model.getObject(uid);
    unlock(&m_instance.onModelStructuralModification);
    return o;
}

} /* namespace org_scilab_modules_scicos */

