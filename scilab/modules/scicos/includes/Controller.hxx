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
 * === LICENSE_END ===
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
    void deleteObject(ScicosID uid);
    ScicosID cloneObject(ScicosID uid, bool cloneChildren, bool clonePorts);

    kind_t getKind(ScicosID uid) const;
    std::vector<ScicosID> getAll(kind_t k) const;
    void sortAndFillKind(std::vector<ScicosID>& uids, std::vector<int>& kind);
    model::BaseObject* getObject(ScicosID uid) const;
    template<typename T>
    T* getObject(ScicosID uid) const
    {
        return static_cast<T*>(getObject(uid));
    }

    template<typename T>
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, T& v) const
    {
        while (m_instance.onModelStructuralModification.test_and_set(std::memory_order_acquire))  // acquire lock
            ; // spin
        bool ret = m_instance.model.getObjectProperty(uid, k, p, v);
        m_instance.onModelStructuralModification.clear(std::memory_order_release); // unlock
        return ret;
    };

    template<typename T>
    update_status_t setObjectProperty(const ScicosID& uid, kind_t k, object_properties_t p, T v)
    {
        while (m_instance.onModelStructuralModification.test_and_set(std::memory_order_acquire))  // acquire lock
            ; // spin
        update_status_t status = m_instance.model.setObjectProperty(uid, k, p, v);
        m_instance.onModelStructuralModification.clear(std::memory_order_release); // unlock

        while (m_instance.onViewsStructuralModification.test_and_set(std::memory_order_acquire))  // acquire lock
            ; // spin
        for (view_set_t::iterator iter = m_instance.allViews.begin(); iter != m_instance.allViews.end(); ++iter)
        {
            (*iter)->propertyUpdated(uid, k, p, status);
        }
        m_instance.onViewsStructuralModification.clear(std::memory_order_release); // unlock
        return status;
    }

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

    /**
     * Shared instance of the data
     *
     * This will be allocated on-demand by Controller::get_instance()
     */
    static SharedData m_instance;

    /*
     * Methods
     */

    ScicosID cloneObject(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, bool cloneChildren, bool clonePorts);
    template<typename T>
    void cloneProperties(model::BaseObject* initial, ScicosID clone);
    void deepClone(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound);
    void deepCloneVector(std::map<ScicosID, ScicosID>& mapped, ScicosID uid, ScicosID clone, kind_t k, object_properties_t p, bool cloneIfNotFound);
    void unlinkVector(ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop);
    void unlink(ScicosID uid, kind_t k, object_properties_t uid_prop, object_properties_t ref_prop);
    void deleteVector(ScicosID uid, kind_t k, object_properties_t uid_prop);
};

} /* namespace org_scilab_modules_scicos */

#endif /* CONTROLLER_HXX_ */
