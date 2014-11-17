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
#include <memory>

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
    std::shared_ptr<model::BaseObject> o;
    switch (k)
    {
        case ANNOTATION:
            o = std::make_shared<model::Annotation>(model::Annotation());
            break;
        case DIAGRAM:
            o = std::make_shared<model::Diagram>(model::Diagram());
            break;
        case BLOCK:
            o = std::make_shared<model::Block>(model::Block());
            break;
        case LINK:
            o = std::make_shared<model::Link>(model::Link());
            break;
        case PORT:
            o = std::make_shared<model::Port>(model::Port());
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
}

std::shared_ptr<model::BaseObject> Model::getObject(ScicosID uid) const
{
    objects_map_t::const_iterator iter = allObjects.lower_bound(uid);
    if (iter == allObjects.end() || uid < iter->first)
    {
        throw std::string("key has not been found");
    }

    return iter->second;
}

// datatypes being a vector of Datatype pointers, we need a dereferencing comparison operator to use std::lower_bound()
static bool isInferior(const model::Datatype* d1, const model::Datatype* d2)
{
    return *d1 < *d2;
}

model::Datatype* Model::flyweight(const model::Datatype& d)
{
    datatypes_set_t::iterator iter = std::lower_bound(datatypes.begin(), datatypes.end(), &d, isInferior);
    if (iter != datatypes.end() && !(d < **iter)) // if d is found
    {
        (*iter)->m_refCount++;
        return *iter;
    }
    else
    {
        return *datatypes.insert(iter, new model::Datatype(d));
    }
}

void Model::erase(model::Datatype* d)
{
    datatypes_set_t::iterator iter = std::lower_bound(datatypes.begin(), datatypes.end(), d, isInferior);
    if (iter != datatypes.end() && !(*d < **iter)) // if d is found
    {
        (*iter)->m_refCount--;
        if ((*iter)->m_refCount < 0)
        {
            delete *iter;
            datatypes.erase(iter);
        }
    }
}

} /* namespace org_scilab_modules_scicos */
