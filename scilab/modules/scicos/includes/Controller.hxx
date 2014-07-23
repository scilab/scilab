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

#ifndef CONTROLLER_HXX_
#define CONTROLLER_HXX_

#include <vector>
#include <string>

#include "utilities.hxx"
#include "Model.hxx"
#include "View.hxx"

namespace org_scilab_modules_scicos
{

/**
 * Controller from the MVC pattern
 *
 * All calls to objects should use this controller, all usable objects are referenced by this controller.
 */
class Controller
{
public:
    static Controller* get_instance();
    static void delete_instance();

    void register_view(View* v);
    void unregister_view(View* v);

    ScicosID createObject(kind_t k);
    void deleteObject(ScicosID uid);

    model::BaseObject* getObject(ScicosID uid);
    update_status_t setObject(model::BaseObject* o);

    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<double>& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<int>& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector< std::string >& v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::vector<ScicosID>& v);

    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<double>& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<int>& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector< std::string >& v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, const std::vector<ScicosID>& v);

private:
    void dispatch(update_status_t status, ScicosID uid, kind_t k, object_properties_t p)
    {
        if (status == SUCCESS)
        {
            for (view_set_t::iterator iter = allViews.begin(); iter != allViews.end(); iter++)
            {
                (*iter)->propertyUpdated(uid, k, p);
            }
        }
        for (view_set_t::iterator iter = allViews.begin(); iter != allViews.end(); iter++)
        {
            (*iter)->propertyUpdated(uid, k, p, status);
        }
    }

    template<typename T>
    update_status_t generic_setObjectProp(ScicosID uid, kind_t k, object_properties_t p, T v)
    {
        update_status_t status = model.setObjectProperty(uid, k, p, v);
        dispatch(status, uid, k, p);
        return status;
    }

private:
    static Controller* _instance;

    Model model;
    typedef std::vector<View*> view_set_t;
    view_set_t allViews;

    Controller();
    ~Controller();
};

} /* namespace org_scilab_modules_scicos */

#endif /* CONTROLLER_HXX_ */
