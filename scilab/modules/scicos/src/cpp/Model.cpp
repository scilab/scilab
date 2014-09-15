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

#include <string>
#include <utility>
#include <algorithm>

#include "Model.hxx"
#include "utilities.hxx"

#include "model/BaseObject.hxx"
#include "model/Annotation.hxx"
#include "model/Diagram.hxx"
#include "model/Block.hxx"
#include "model/Link.hxx"
#include "model/Port.hxx"

namespace org_scilab_modules_scicos
{

Model::Model() :
    lastId(0), allObjects()
{
    std::vector<int> datatypeDefault (3, 1);
    datatypeDefault[0] = -1;
    datatypes.push_back(new model::Datatype(datatypeDefault));
}

Model::~Model()
{
    while (!datatypes.empty())
    {
        Model::erase(datatypes[0]);
    }
    datatypes.clear();
}

ScicosID Model::createObject(kind_t k)
{
    /*
     * Allocate the object per kind
     */
    model::BaseObject* o;
    switch (k)
    {
        case ANNOTATION:
            o = new model::Annotation();
            break;
        case DIAGRAM:
            o = new model::Diagram();
            break;
        case BLOCK:
            o = new model::Block();
            break;
        case LINK:
            o = new model::Link();
            break;
        case PORT:
            o = new model::Port();
            break;
    }

    /*
     * Found the next unused id
     */
    lastId++;
    if (lastId == 0)
    {
        lastId++;
    }

    // full map, detection
    bool has_looped = false;

    objects_map_t::iterator iter = allObjects.lower_bound(lastId);
    while (iter != allObjects.end() && !(lastId < iter->first)) // while key is found
    {
        // try a valid ID
        lastId++;
        if (lastId == 0)
        {
            lastId++;

            // if the map is full, return 0;
            if (has_looped)
            {
                delete o;
                return 0;
            }
            has_looped = true;
        }

        // look for it
        iter = allObjects.lower_bound(lastId);
    }

    /*
     * Insert then return
     */
    allObjects.insert(iter, std::make_pair(lastId, o));
    o->id(lastId);
    return lastId;
}

void Model::deleteObject(ScicosID uid)
{
    objects_map_t::iterator iter = allObjects.lower_bound(uid);
    if (iter == allObjects.end() || uid < iter->first)
    {
        throw std::string("key has not been found");
    }

    allObjects.erase(iter);
    delete iter->second;
}

model::BaseObject* Model::getObject(ScicosID uid) const
{
    objects_map_t::const_iterator iter = allObjects.lower_bound(uid);
    if (iter == allObjects.end() || uid < iter->first)
    {
        throw std::string("key has not been found");
    }

    return iter->second;
}

update_status_t Model::setObject(model::BaseObject* o)
{
    objects_map_t::iterator iter = allObjects.lower_bound(o->id());
    if (iter == allObjects.end() || o->id() < iter->first)
    {
        throw std::string("key has not been found");
    }

    if (*iter->second == *o)
    {
        return NO_CHANGES;
    }

    o->id(iter->second->id());
    delete iter->second;
    iter->second = o;
    return SUCCESS;
}

model::Datatype* Model::flyweight(const model::Datatype& d)
{
    datatypes_set_t::iterator iter = std::lower_bound(datatypes.begin(), datatypes.end(), &d);
    if (iter != datatypes.end() && !(d < **iter)) // if d is found
    {
        (*iter)->refCount++;
        return *iter;
    }
    else
    {
        return *datatypes.insert(iter, new model::Datatype(d));
    }
}

void Model::erase(model::Datatype* d)
{
    datatypes_set_t::iterator iter = std::lower_bound(datatypes.begin(), datatypes.end(), d);
    if (iter != datatypes.end() && !(*d < **iter)) // if d is found
    {
        (*iter)->refCount--;
        if ((*iter)->refCount < 0)
        {
            delete *iter;
            datatypes.erase(iter);
        }
    }
}

} /* namespace org_scilab_modules_scicos */
