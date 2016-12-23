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

#ifndef MODEL_HXX_
#define MODEL_HXX_

#include <vector>
#include <unordered_map>
#include <string>

#include "utilities.hxx"
#include "model/BaseObject.hxx"

#include "dynlib_scicos.h"

namespace org_scilab_modules_scicos
{

class SCICOS_IMPEXP Model
{
public:
    Model();
    ~Model();

    /*
     * Controller wrapped methods
     */

    ScicosID createObject(kind_t k);
    unsigned referenceObject(const ScicosID uid);
    unsigned& referenceCount(ScicosID uid);
    void deleteObject(ScicosID uid);

    template<typename T>
    inline bool getObjectProperty(ScicosID uid, kind_t /*k*/, object_properties_t p, T& v) const
    {
        return getObjectProperty(getObject(uid), p, v);
    }

    template<typename T>
    inline update_status_t setObjectProperty(ScicosID uid, kind_t /*k*/, object_properties_t p, T v)
    {
        return setObjectProperty(getObject(uid), p, v);
    }

    bool getObjectProperty(model::BaseObject* object, object_properties_t p, double& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, int& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, bool& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, std::string& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, ScicosID& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<double>& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<int>& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<bool>& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector< std::string >& v) const;
    bool getObjectProperty(model::BaseObject* object, object_properties_t p, std::vector<ScicosID>& v) const;

    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, double v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, int v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, bool v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, ScicosID v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, std::string v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, const std::vector<double>& v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, const std::vector<int>& v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, const std::vector<bool>& v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, const std::vector< std::string >& v);
    update_status_t setObjectProperty(model::BaseObject* object, object_properties_t p, const std::vector<ScicosID>& v);
    /*
     * Model custom methods
     */

    kind_t getKind(ScicosID uid) const;
    std::vector<ScicosID> getAll(kind_t k) const;
    model::BaseObject* getObject(ScicosID uid) const;
    template<typename T>
    T* getObject(ScicosID uid) const
    {
        return static_cast<T*>(getObject(uid));
    };

    model::Datatype* flyweight(const model::Datatype& d);
    void erase(model::Datatype* d);

private:
    ScicosID lastId;
    bool has_looped;

    typedef std::unordered_map<ScicosID, model::BaseObject* > objects_map_t;
    objects_map_t allObjects;

    typedef std::vector<model::Datatype*> datatypes_set_t;
    datatypes_set_t datatypes;
};

} /* namespace org_scilab_modules_scicos */

#endif /* MODEL_HXX_ */
