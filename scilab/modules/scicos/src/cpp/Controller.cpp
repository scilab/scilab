/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2014-2014 - Scilab Enterprises - Clement DAVID
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

#define REF_DEBUG 0
#if REF_DEBUG
#include "scilabWrite.hxx"
#define REF_PRINT(uid, refCount) \
	do { \
	std::stringstream print; \
	print << "referenceObject( " << uid << " ) : " << refCount << '\n'; \
	scilabForcedWrite(print.str().data()); \
	} while(0)
#define UNREF_PRINT(uid, count) \
	do { \
	std::stringstream print; \
	print << "unreferenceObject( " << uid << " ) : " << refCount << '\n'; \
	scilabForcedWrite(print.str().data()); \
	} while(0)
#define CLONE_PRINT(uid, clone) \
	do { \
	std::stringstream print; \
	print << "cloneObject( " << uid << " ) : " << clone << '\n'; \
	scilabForcedWrite(print.str().data()); \
	} while(0)
#else
#define REF_PRINT(uid, refCount)
#define UNREF_PRINT(uid, refCount)
#define CLONE_PRINT(uid, clone)
#endif

#include "Controller.hxx"
#include "model/BaseObject.hxx"

#include "LoggerView.hxx"

extern "C" {
#include "scicos.h"
}

namespace org_scilab_modules_scicos
{

static inline void lock(std::atomic_flag* m)
{
    while (m->test_and_set(std::memory_order_acquire))  // acquire lock
        ; // spin
}

static inline void unlock(std::atomic_flag* m)
{
    m->clear(std::memory_order_release);
}

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
    REF_PRINT(uid, refCount);

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
        UNREF_PRINT(uid, refCount);

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
        unlinkVector(uid, k, PARENT_DIAGRAM, CHILDREN);
        unlinkVector(uid, k, PARENT_BLOCK, CHILDREN);
        // RELATED_TO is not referenced back
    }
    else if (k == BLOCK)
    {
        unlinkVector(uid, k, PARENT_DIAGRAM, CHILDREN);
        unlinkVector(uid, k, PARENT_BLOCK, CHILDREN);

        deleteVector(uid, k, INPUTS);
        deleteVector(uid, k, OUTPUTS);
        deleteVector(uid, k, EVENT_INPUTS);
        deleteVector(uid, k, EVENT_OUTPUTS);

        unlink(uid, k, CHILDREN, PARENT_BLOCK);
        deleteVector(uid, k, CHILDREN);
        // FIXME what about REFERENCED_PORT ?
    }
    else if (k == DIAGRAM)
    {
        unlink(uid, k, CHILDREN, PARENT_DIAGRAM);
        deleteVector(uid, k, CHILDREN);
    }
    else if (k == LINK)
    {
        unlinkVector(uid, k, PARENT_DIAGRAM, CHILDREN);
        unlinkVector(uid, k, PARENT_BLOCK, CHILDREN);

        unlinkVector(uid, k, SOURCE_PORT, CONNECTED_SIGNALS);
        unlinkVector(uid, k, DESTINATION_PORT, CONNECTED_SIGNALS);
    }
    else if (k == PORT)
    {
        unlinkVector(uid, k, SOURCE_BLOCK, INPUTS);
        unlinkVector(uid, k, SOURCE_BLOCK, OUTPUTS);
        unlinkVector(uid, k, SOURCE_BLOCK, EVENT_INPUTS);
        unlinkVector(uid, k, SOURCE_BLOCK, EVENT_OUTPUTS);

        unlink(uid, k, CONNECTED_SIGNALS, SOURCE_PORT);
        unlink(uid, k, CONNECTED_SIGNALS, DESTINATION_PORT);
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

void Controller::unlinkVector(ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop)
{
    ScicosID v;
    getObjectProperty(uid, k, uid_prop, v);
    if (v != ScicosID())
    {
        auto o = getObject(v);
        if (o == nullptr)
        {
            return;
        }

        std::vector<ScicosID> children;
        getObjectProperty(o->id(), o->kind(), ref_prop, children);

        std::vector<ScicosID>::iterator it = std::find(children.begin(), children.end(), uid);
        if (it != children.end())
        {
            children.erase(it);
        }

        setObjectProperty(o->id(), o->kind(), ref_prop, children);
    }
}

void Controller::unlink(ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop)
{
    std::vector<ScicosID> v;
    getObjectProperty(uid, k, uid_prop, v);
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
            if (oppositeRef == uid)
            {
                setObjectProperty(o->id(), o->kind(), ref_prop, ScicosID());
            }
        }
    }
}

void Controller::deleteVector(ScicosID uid, kind_t k, object_properties_t uid_prop)
{
    std::vector<ScicosID> children;
    getObjectProperty(uid, k, uid_prop, children);

    for (ScicosID id : children)
    {
        deleteObject(id);
    }
}

template<typename T>
void Controller::cloneProperties(model::BaseObject* initial, ScicosID clone)
{
    for (int i = 0; i < MAX_OBJECT_PROPERTIES; ++i)
    {
        enum object_properties_t p = static_cast<enum object_properties_t>(i);

        T value;
        bool status = getObjectProperty(initial->id(), initial->kind(), p, value);
        if (status)
        {
            setObjectProperty(clone, initial->kind(), p, value);
        }
    }
}

ScicosID Controller::cloneObject(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, bool cloneChildren, bool clonePorts)
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
        deepClone(mapped, uid, o, k, PARENT_BLOCK, false);
        deepClone(mapped, uid, o, k, RELATED_TO, true);
    }
    else if (k == BLOCK)
    {
        deepClone(mapped, uid, o, k, PARENT_DIAGRAM, false);
        if (clonePorts)
        {
            // Only copy the block, without any port
            deepCloneVector(mapped, uid, o, k, INPUTS, true);
            deepCloneVector(mapped, uid, o, k, OUTPUTS, true);
            deepCloneVector(mapped, uid, o, k, EVENT_INPUTS, true);
            deepCloneVector(mapped, uid, o, k, EVENT_OUTPUTS, true);
        }

        deepClone(mapped, uid, o, k, PARENT_BLOCK, false);
        if (cloneChildren)
        {
            deepCloneVector(mapped, uid, o, k, CHILDREN, true);
        }
        // FIXME what about REFERENCED_PORT ?
    }
    else if (k == DIAGRAM)
    {
        if (cloneChildren)
        {
            deepCloneVector(mapped, uid, o, k, CHILDREN, true);
        }
    }
    else if (k == LINK)
    {
        deepClone(mapped, uid, o, k, PARENT_DIAGRAM, false);
        deepClone(mapped, uid, o, k, PARENT_BLOCK, false);
        deepClone(mapped, uid, o, k, SOURCE_PORT, false);
        deepClone(mapped, uid, o, k, DESTINATION_PORT, false);
    }
    else if (k == PORT)
    {
        deepClone(mapped, uid, o, k, SOURCE_BLOCK, false);
        deepCloneVector(mapped, uid, o, k, CONNECTED_SIGNALS, false);
    }
    return o;
}

void Controller::deepClone(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound)
{
    ScicosID v;
    getObjectProperty(uid, k, p, v);

    ScicosID cloned = ScicosID();

    std::map<ScicosID, ScicosID>::iterator it = mapped.find(v);
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
                cloned = cloneObject(mapped, v, true, true);
            }
            else
            {
                cloned = ScicosID();
            }
        }
        else
        {
            cloned = ScicosID();
        }
    }

    setObjectProperty(clone, k, p, cloned);
    // When cloning a Link, connect both extremities together
    if ((p == SOURCE_PORT || p == DESTINATION_PORT) && cloned != ScicosID())
    {
        setObjectProperty(cloned, PORT, CONNECTED_SIGNALS, clone);
    }
}

void Controller::deepCloneVector(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound)
{
    std::vector<ScicosID> v;
    getObjectProperty(uid, k, p, v);

    std::vector<ScicosID> cloned;
    cloned.reserve(v.size());

    for (const ScicosID & id : v)
    {
        if (id == ScicosID())
        {
            // Deleted Block, the cloning is done at Adapter level
            cloned.push_back(id);
            continue;
        }

        std::map<ScicosID, ScicosID>::iterator it = mapped.find(id);
        if (it != mapped.end())
        {
            if (k == PORT)
            {
                // We get here if we are cloning a block connected to a link that comes before itself in the objects list,
                // so which has already been cloned but could not be connected yet.
                int port_kind;
                getObjectProperty(clone, PORT, PORT_KIND, port_kind);
                if (port_kind == PORT_IN || port_kind == PORT_EIN)
                {
                    setObjectProperty(it->second, LINK, DESTINATION_PORT, clone);
                }
                else
                {
                    // FIXME: fix case for implicit ports, in which case connect the first unconnected link end, it doesn't matter which one
                    setObjectProperty(it->second, LINK, SOURCE_PORT, clone);
                }
            }
            cloned.push_back(it->second);
        }
        else
        {
            if (cloneIfNotFound)
            {
                if (id != ScicosID())
                {
                    cloned.push_back(cloneObject(mapped, id, true, true));
                }
                else
                {
                    cloned.push_back(ScicosID());
                }
            }
            else
            {
                cloned.push_back(ScicosID());
            }
        }
    }

    setObjectProperty(clone, k, p, cloned);
}

ScicosID Controller::cloneObject(ScicosID uid, bool cloneChildren, bool clonePorts)
{
    std::map<ScicosID, ScicosID> mapped;
    ScicosID clone = cloneObject(mapped, uid, cloneChildren, clonePorts);
    CLONE_PRINT(uid, clone);

    return clone;
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

