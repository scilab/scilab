/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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

#include "VariableScope.hxx"
#include "XMLObject.hxx"
#include "XMLNodeList.hxx"
#include <iostream>

namespace org_modules_xml
{
xmlFreeFunc VariableScope::XMLFreeFunc = 0;
std::map < void *, XMLObject * > VariableScope::mapLibXMLToXMLObject = std::map < void *, XMLObject * >();
std::map < void *, XMLNodeList * > VariableScope::mapLibXMLToXMLNodeList = std::map < void *, XMLNodeList * >();
std::map < const XMLObject *, std::map < const XMLObject *, bool>*> VariableScope::parentToChildren = std::map < const XMLObject *, std::map < const XMLObject *, bool>*>();

VariableScope::VariableScope(int _initialSize)
{
    position = -1;
    initialSize = _initialSize;
    scope = new std::vector < XMLObject * >();
    freePlaces = new std::stack < int >();
    initXMLMemory();
}

VariableScope::~VariableScope()
{
    for (unsigned int i = 0; i < scope->size(); i++)
    {
        if ((*scope)[i])
        {
            delete(*scope)[i];
        }
    }
    delete scope;
    delete freePlaces;
}

/**
 * To avoid unused place in the vector, we use a stack which contains the empty places.
 */
int VariableScope::getVariableId(const XMLObject & obj)
{
    int returnValue;
    const XMLObject *parent = obj.getXMLObjectParent();

    if (freePlaces->size() != 0)
    {
        returnValue = freePlaces->top();
        freePlaces->pop();
        (*scope)[returnValue] = const_cast < XMLObject * >(&obj);
    }
    else
    {
        returnValue = (int)scope->size();
        scope->push_back(const_cast < XMLObject * >(&obj));
    }

    if (parent)
    {
        std::map < const XMLObject *, std::map < const XMLObject *, bool>*>::const_iterator it = parentToChildren.find(parent);

        if (it != parentToChildren.end())
        {
            std::map < const XMLObject *, bool>::iterator jt = it->second->find(&obj);
            if (jt != it->second->end())
            {
                jt->second = true;
            }
            else
            {
                it->second->insert(std::pair<const XMLObject *, bool>(&obj, true));
            }
        }
        else
        {
            std::map < const XMLObject *, bool> * map = new std::map < const XMLObject *, bool>();
            map->insert(std::pair<const XMLObject *, bool>(&obj, true));
            parentToChildren.insert(std::pair<const XMLObject *, std::map < const XMLObject *, bool>*>(parent, map));
        }
    }

    return returnValue;
}

XMLObject *VariableScope::getVariableFromId(int id)
{
    if (id >= 0 && id < (int)scope->size())
    {
        return (*scope)[id];
    }

    return 0;
}

/**
 * There are two motivations to register libxml pointers:
 *   i) if a XMLObject is associated to a libxml node, then when this node will be removed
 *      the XMLObject must be destroyed. This job is done in _xmlFreeFunc which is called
 *      by libxml when a libxml node is freed.
 *   ii) To avoid multiple instances of a XMLObject which wraps the same node, the function
 *       getXMLObjectFromLibXMLPtr is used to know if a XMLObject already exists for the
 *       libxml node.
 */
void VariableScope::registerPointers(void *libxml, XMLObject * obj)
{
    if (libxml)
    {
        mapLibXMLToXMLObject[libxml] = obj;
    }
}

void VariableScope::registerPointers(void *libxml, XMLNodeList * nodeList)
{
    if (libxml)
    {
        mapLibXMLToXMLNodeList[libxml] = nodeList;
    }
}

void VariableScope::unregisterPointer(void *libxml)
{
    if (libxml)
    {
        mapLibXMLToXMLObject.erase(libxml);
    }
}

void VariableScope::unregisterNodeListPointer(void *libxml)
{
    if (libxml)
    {
        mapLibXMLToXMLNodeList.erase(libxml);
    }
}

XMLObject *VariableScope::getXMLObjectFromLibXMLPtr(void *libxml) const
{
    if (libxml)
    {
        std::map < void *, XMLObject * >::const_iterator it = mapLibXMLToXMLObject.find(libxml);
        if (it != mapLibXMLToXMLObject.end())
        {
            return it->second;
        }
    }

    return 0;
}

XMLNodeList *VariableScope::getXMLNodeListFromLibXMLPtr(void *libxml)const
{
    if (libxml)
    {
        std::map < void *, XMLNodeList * >::const_iterator it = mapLibXMLToXMLNodeList.find(libxml);
        if (it != mapLibXMLToXMLNodeList.end())
        {
            return it->second;
        }
    }

    return 0;
}

void VariableScope::removeId(int id)
{
    if (id >= 0 && id < (int)scope->size() && (*scope)[id])
    {
        XMLObject * const child = (*scope)[id];
        removeChildFromParent(child);
        removeDependencies(child);
        (*scope)[id] = 0;
        freePlaces->push(id);
    }
}

void VariableScope::removeDependencies(XMLObject * obj)
{
    std::map < const XMLObject *, std::map < const XMLObject *, bool>*>::const_iterator it = parentToChildren.find(obj);

    if (it != parentToChildren.end())
    {
        for (std::map < const XMLObject *, bool>::const_iterator i = it->second->begin(), e = it->second->end(); i != e; ++i)
        {
            const XMLObject *child = i->first;
            if (child && i->second && getVariableFromId(child->getId()) == child)
            {
                delete child;
            }
        }
        delete it->second;

        parentToChildren.erase(obj);
    }
}

void VariableScope::initXMLMemory()
{
    xmlFreeFunc freeFunc;
    xmlMallocFunc mallocFunc;
    xmlReallocFunc reallocFunc;
    xmlStrdupFunc strdupFunc;

    xmlMemGet(&freeFunc, &mallocFunc, &reallocFunc, &strdupFunc);
    freeFunc = getFreeFunc(freeFunc);
    xmlMemSetup(freeFunc, mallocFunc, reallocFunc, strdupFunc);
}

xmlFreeFunc VariableScope::getFreeFunc(xmlFreeFunc freeFunc)
{
    if (!XMLFreeFunc)
    {
        XMLFreeFunc = freeFunc;
    }

    return &_xmlFreeFunc;
}

void VariableScope::_xmlFreeFunc(void *mem)
{
    std::map < void *, XMLObject * >::const_iterator it = mapLibXMLToXMLObject.find(mem);

    if (it != mapLibXMLToXMLObject.end())
    {
        delete it->second;

        mapLibXMLToXMLObject.erase(mem);
    }

    std::map < void *, XMLNodeList * >::const_iterator itnl = mapLibXMLToXMLNodeList.find(mem);

    if (itnl != mapLibXMLToXMLNodeList.end())
    {
        delete itnl->second;

        mapLibXMLToXMLNodeList.erase(mem);
    }

    XMLFreeFunc(mem);
}

inline void VariableScope::removeChildFromParent(const XMLObject * child)
{
    const XMLObject *parent = child->getXMLObjectParent();
    std::map < const XMLObject *, std::map < const XMLObject *, bool>*>::iterator i = parentToChildren.find(parent);

    if (i != parentToChildren.end())
    {
        std::map < const XMLObject *, bool>::iterator j = i->second->find(child);
        if (j != i->second->end())
        {
            j->second = false;
        }
    }
}
}
