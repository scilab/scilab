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

#ifndef MODEL_HXX_
#define MODEL_HXX_

#include <map>
#include <string>

#include "utilities.hxx"
#include "model/BaseObject.hxx"

namespace org_scilab_modules_scicos
{

class Model
{
public:
    Model();
    ~Model();

    ScicosID createObject(kind_t k);
    void deleteObject(ScicosID uid);

    model::BaseObject* getObject(ScicosID uid) const;
    update_status_t setObject(model::BaseObject* o);

    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double* v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int* v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool* v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string* v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID* v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len, double** v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len, int** v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len, std::string** v);
    bool getObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t* len, ScicosID** v);

    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, double v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, int v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, bool v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, ScicosID v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, std::string v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len, double* v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len, int* v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len, std::string* v);
    update_status_t setObjectProperty(ScicosID uid, kind_t k, object_properties_t p, size_t len, ScicosID* v);

private:
    ScicosID lastId;
    typedef std::map<ScicosID, model::BaseObject*> objects_map_t;
    objects_map_t allObjects;
};

} /* namespace org_scilab_modules_scicos */

#endif /* MODEL_HXX_ */
