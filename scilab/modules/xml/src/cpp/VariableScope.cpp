/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "VariableScope.hxx"
#include "XMLObject.hxx"

namespace org_modules_xml
{
    xmlFreeFunc VariableScope::XMLFreeFunc = 0;
    std::map<void *, XMLObject *> * VariableScope::mapLibXMLToXMLObject = new std::map<void *, XMLObject *>();

    VariableScope::VariableScope(int _initialSize)
    {
        position = -1;
        initialSize = _initialSize;
        scope = new XMLObject*[initialSize];
        std::fill(scope, scope + initialSize, static_cast<XMLObject *>(0));
        freePlaces = new std::stack<int>();
        initXMLMemory();
    }

    VariableScope::~VariableScope()
    {
        for (int i = 0; i <= position; i++)
        {
            if (scope[i])
            {
                delete scope[i];
            }
        }
        delete[] scope;
        delete freePlaces;
    }

    int VariableScope::getVariableId(const XMLObject & obj)
    {
        int returnValue;

        if (freePlaces->size() != 0)
        {
            returnValue = freePlaces->top();
            freePlaces->pop();
        }
        else
        {
            if (position == initialSize - 1)
            {
                int newSize = initialSize * 2;
                XMLObject ** newScope = new XMLObject*[newSize];
                std::fill(newScope + initialSize, newScope + newSize, static_cast<XMLObject *>(0));
                for (int i = 0; i < initialSize; i++)
                {
                    newScope[i] = scope[i];
                }
                delete[] scope;
                initialSize = newSize;
                scope = newScope;
            }
            returnValue = ++position;
        }
        scope[returnValue] = const_cast<XMLObject*>(&obj);

        return returnValue;
    }

    XMLObject * VariableScope::getVariableFromId(int id)
    {
        if (id >= 0 && id < initialSize)
        {
            return scope[id];
        }

        return 0;
    }

    void VariableScope::registerPointers(void * libxml, XMLObject * obj)
    {
        if (libxml)
        {
            (*mapLibXMLToXMLObject)[libxml] = obj;
        }
    }

    void VariableScope::unregisterPointer(void * libxml)
    {
        if (libxml)
        {
            mapLibXMLToXMLObject->erase(libxml);
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
        VariableScope::XMLFreeFunc = freeFunc;
        return &_xmlFreeFunc;
    }

    void VariableScope::_xmlFreeFunc(void * mem)
    {
        std::map<void *, XMLObject *>::const_iterator it = mapLibXMLToXMLObject->find(mem);
        if (it != mapLibXMLToXMLObject->end())
        {
            delete it->second;
        }

        VariableScope::XMLFreeFunc(mem);
    }
}

