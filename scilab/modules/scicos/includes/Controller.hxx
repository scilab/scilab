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
    static View* register_view(const std::string& name, View* v);
    static void unregister_view(View* v);
    static View* unregister_view(const std::string& name);
    static View* look_for_view(const std::string& name);

    static void end_simulation();

    Controller();
    ~Controller();

    ScicosID createObject(kind_t k);
    unsigned referenceObject(const ScicosID uid) const;
    template<typename T>
    T* referenceObject(T* o) const
    {
        referenceObject(o->id());
        return o;
    }
    template<typename T>
    T* referenceObject(model::BaseObject* o) const
    {
        referenceObject(o->id());
        return static_cast<T*>(o);
    }
    void deleteObject(ScicosID uid);
    ScicosID cloneObject(ScicosID uid, bool cloneChildren, bool clonePorts);
    model::BaseObject* cloneObject(std::map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, bool cloneChildren, bool clonePorts);

    kind_t getKind(ScicosID uid) const;
    std::vector<ScicosID> getAll(kind_t k) const;
    void sortAndFillKind(std::vector<ScicosID>& uids, std::vector<int>& kind);
    model::BaseObject* getObject(ScicosID uid) const;
    template<typename T>
    T* getObject(ScicosID uid) const
    {
        return static_cast<T*>(getObject(uid));
    }

    /*
     * C++ API
     */

    template<typename T>
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, T& v) const
    {
        lock(&m_instance.onModelStructuralModification);
        bool ret = m_instance.model.getObjectProperty(object, p, v);
        unlock(&m_instance.onModelStructuralModification);
        return ret;
    }

    template<typename T>
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, const T& v)
    {
        lock(&m_instance.onModelStructuralModification);
        update_status_t ret = m_instance.model.setObjectProperty(object, p, v);
        unlock(&m_instance.onModelStructuralModification);
        return ret;
    }

    /*
     * C / Java API
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
     * Methods
     */

    template<typename T> void cloneProperties(model::BaseObject* initial, model::BaseObject* clone);
    template<typename T> bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T& v) const;
    template<typename T> update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T v);
    void deepClone(std::map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, model::BaseObject* clone, object_properties_t p, bool cloneIfNotFound);
    void deepCloneVector(std::map<model::BaseObject*, model::BaseObject*>& mapped, model::BaseObject* initial, model::BaseObject* clone, object_properties_t p, bool cloneIfNotFound);
    void unlinkVector(model::BaseObject* o, object_properties_t uid_prop, object_properties_t ref_prop);
    void unlink(model::BaseObject* o, object_properties_t uid_prop, object_properties_t ref_prop);
    void deleteVector(model::BaseObject* o, object_properties_t uid_prop);
};

} /* namespace org_scilab_modules_scicos */

#endif /* CONTROLLER_HXX_ */
