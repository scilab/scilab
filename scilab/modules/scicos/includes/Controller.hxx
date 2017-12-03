/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014-2016 - Scilab Enterprises - Clement DAVID
 * Copyright (C) 2017 - ESI Group - Clement DAVID
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

#ifndef CONTROLLER_HXX_
#define CONTROLLER_HXX_

#include <atomic>
#include <string>
#include <vector>
#include <map>

#include "utilities.hxx"
#include "Model.hxx"
#include "View.hxx"
#include "model/BaseObject.hxx"

#include "dynlib_scicos.h"

namespace org_scilab_modules_scicos
{

/**
 * Controller from the MVC pattern
 *
 * All calls to objects should use this controller, all usable objects are referenced by this controller.
 */
class SCICOS_IMPEXP Controller
{
public:
#if !defined SWIG
    static View* register_view(const std::string& name, View* v);
    static void unregister_view(View* v);
    static View* unregister_view(const std::string& name);
    static View* look_for_view(const std::string& name);
#endif /* !defined SWIG */

    /*
     * SWIG mapped API
     */

    static void end_simulation();

    Controller();
    ~Controller();

    ScicosID createObject(kind_t k)
    {
        return createBaseObject(k)->id();
    }

    unsigned referenceObject(const ScicosID uid) const
    {
        return referenceBaseObject(getBaseObject(uid))->refCount();
    }

    void deleteObject(ScicosID uid)
    {
        deleteBaseObject(getBaseObject(uid));
    }

    ScicosID cloneObject(ScicosID uid, bool cloneChildren, bool clonePorts);

    kind_t getKind(ScicosID uid) const;
    std::vector<ScicosID> getAll(kind_t k) const;
    void sortAndFillKind(std::vector<ScicosID>& uids, std::vector<int>& kind);

    /*
     * C++ API
     */
#if !defined SWIG
    model::BaseObject* createBaseObject(kind_t k);
    template<typename T>
    T* createBaseObject(kind_t k)
    {
        return static_cast<T*>(createBaseObject(k));
    }
    model::BaseObject* referenceBaseObject(model::BaseObject* o) const;
    template<typename T>
    T* referenceBaseObject(T* o) const
    {
        referenceBaseObject(static_cast<model::BaseObject*>(o));
        return o;
    }
    template<typename T>
    T* referenceBaseObject(model::BaseObject* o) const
    {
        referenceBaseObject(o);
        return static_cast<T*>(o);
    }
    void deleteBaseObject(model::BaseObject* o);

    model::BaseObject* getBaseObject(ScicosID uid) const;
    template<typename T>
    T* getBaseObject(ScicosID uid) const
    {
        return static_cast<T*>(getBaseObject(uid));
    }
    model::BaseObject* cloneBaseObject(std::unordered_map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, bool cloneChildren, bool clonePorts);
#endif /* !defined SWIG */

    /*
     * C++ API: getters and setters
     */
    template<typename K, typename T>
    bool getObjectProperty(K* object, object_properties_t p, T& v) const
    {
        static_assert(std::is_base_of<model::BaseObject, K>::value, "object should be an MVC object");

        lock(&m_instance.onModelStructuralModification);
        bool ret = m_instance.model.getObjectProperty(object, p, v);
        unlock(&m_instance.onModelStructuralModification);
        return ret;
    };

    template<typename K, typename T>
    update_status_t setObjectProperty(K* object, object_properties_t p, T const& v)
    {
        static_assert(std::is_base_of<model::BaseObject, K>::value, "object should be an MVC object");

        lock(&m_instance.onModelStructuralModification);
        update_status_t status = m_instance.model.setObjectProperty(object, p, v);
        unlock(&m_instance.onModelStructuralModification);

        lock(&m_instance.onViewsStructuralModification);
        for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
        {
            (*iter)->propertyUpdated(object->id(), object->kind(), p, status);
        }
        unlock(&m_instance.onViewsStructuralModification);
        return status;
    };

    /*
     * SWIG mapped API: getters and setters
     */

    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<double>& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<int>& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<bool>& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector< std::string >& v) const;
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<ScicosID>& v) const;

    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::string& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<double>& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<int>& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<bool>& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector< std::string >& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<ScicosID>& v);

private:
    typedef std::vector<View*> view_set_t;
    typedef std::vector<std::string> view_name_set_t;

    /**
     * Shared data through all instance of the controllers
     */
    struct SharedData
    {
        std::atomic_flag onModelStructuralModification;
        Model model;

        std::atomic_flag onViewsStructuralModification;
        view_name_set_t allNamedViews;
        view_set_t allViews;

        SharedData();
        ~SharedData();
    };

    /*
     * Helpers
     */

    static inline void lock(std::atomic_flag* m)
    {
        while (m->test_and_set(std::memory_order_acquire))  // acquire lock
            ; // spin
    }

    static inline void unlock(std::atomic_flag* m)
    {
        m->clear(std::memory_order_release);
    }

    /**
     * Shared instance of the data
     *
     * This will be allocated on-demand by Controller::get_instance()
     */
    static SharedData m_instance;

    /*
     * per instance data, for performance reasons
     */
    std::string _strShared;
    std::vector<double> _vecDblShared;
    std::vector<int> _vecIntShared;
    std::vector<std::string> _vecStrShared;
    std::vector<ScicosID> _vecIDShared;

    /*
     * Methods
     */

    template<typename T> void cloneProperties(model::BaseObject* initial, model::BaseObject* clone);
    template<typename T> void cloneProperties(model::BaseObject* initial, model::BaseObject* clone, T& temporary);
    template<typename T> bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T& v) const;
    template<typename T> update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T v);
    void deepClone(std::unordered_map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, model::BaseObject* clone, object_properties_t p, bool cloneIfNotFound);
    void deepCloneVector(std::unordered_map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, model::BaseObject* clone, object_properties_t p, bool cloneIfNotFound);
    void updateChildrenRelatedPropertiesAfterClone(std::unordered_map<model::BaseObject*, model::BaseObject*>& mapped);
    void unlinkVector(model::BaseObject* o, object_properties_t uid_prop, object_properties_t ref_prop);
    void unlink(model::BaseObject* o, object_properties_t uid_prop, object_properties_t ref_prop);
    void deleteVector(model::BaseObject* o, object_properties_t uid_prop);
    void deleteOwnedReference(model::BaseObject* o, object_properties_t uid_prop);
};

} /* namespace org_scilab_modules_scicos */

#endif /* CONTROLLER_HXX_ */
