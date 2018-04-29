/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014-2017 - Scilab Enterprises - Clement DAVID
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

#include <cassert>
#include <string>
#include <vector>
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
    lastId(ScicosID()), has_looped(false), allObjects()
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

/* define a custom delete as the BaseObject class is fully abstract */
static inline void deleteBaseObject(model::BaseObject* o)
{
    switch (o->kind())
    {
        case ANNOTATION:
            delete static_cast<model::Annotation*>(o);
            break;
        case DIAGRAM:
            delete static_cast<model::Diagram*>(o);
            break;
        case BLOCK:
            delete static_cast<model::Block*>(o);
            break;
        case LINK:
            delete static_cast<model::Link*>(o);
            break;
        case PORT:
            delete static_cast<model::Port*>(o);
            break;
        default:
            break;
    }
};


model::BaseObject* Model::createObject(kind_t k)
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
        default:
            return nullptr;
    }

    /*
     * Found the next unused id
     */
    lastId++;
    if (lastId == ScicosID())
    {
        lastId++;
        has_looped = true;
    }

    if (has_looped)
    {
        bool has_looped_twice = false;

        // while key is found
        for (allobjects_t::iterator iter = allObjects.find(lastId);
                iter != allObjects.end();
                iter = allObjects.find(lastId))
        {
            // try a valid ID
            lastId++;
            if (lastId == ScicosID())
            {
                lastId++;

                // return the invalid value if the loop counter encounter 2 zeros.
                if (has_looped_twice)
                {
                    deleteBaseObject(o);
                    return nullptr;
                }
                else
                {
                    has_looped_twice = true;
                }
            }
        }
    }

    /*
     * Insert then return
     */
    o->id(lastId);
    allObjects.emplace(lastId, o);
    return o;
}

unsigned Model::referenceObject(model::BaseObject* object)
{
    return ++object->refCount();
}

unsigned& Model::referenceCount(model::BaseObject* object)
{
    return object->refCount();
}

void Model::deleteObject(model::BaseObject* object)
{
    if (object->refCount() == 0)
    {
        if (allObjects.erase(object->id()))
        {
            deleteBaseObject(object);
        }
        else
        {
            // something went wrong, assert !
            assert(0);
        }
    }
    else
    {
        --object->refCount();
    }
}

kind_t Model::getKind(ScicosID uid) const
{
    model::BaseObject* o = getObject(uid);
    if (o != nullptr)
    {
        return o->kind();
    }
    else
    {
        // return the first kind, it will be always ignored as the object is no more valid
        return ANNOTATION;
    }
}

std::vector<model::BaseObject*> Model::getAll(kind_t k) const
{
    std::vector<model::BaseObject*> all;
    for (const auto& it : allObjects)
        if (it.second->kind() == k)
        {
            all.emplace_back(it.second);
        }

    return all;
}

model::BaseObject* Model::getObject(ScicosID uid) const
{
    allobjects_t::const_iterator iter = allObjects.find(uid);
    if (iter != allObjects.end() && iter->first == uid)
    {
        return iter->second;
    }

    return nullptr;
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
