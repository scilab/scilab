/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "VariableScope.hxx"
#include "XMLObject.hxx"
#include "XMLNodeList.hxx"

namespace org_modules_xml
{
    xmlFreeFunc VariableScope::XMLFreeFunc = 0;
      std::map < void *, XMLObject * >*VariableScope::mapLibXMLToXMLObject = new std::map < void *, XMLObject * >();
      std::map < void *, XMLNodeList * >*VariableScope::mapLibXMLToXMLNodeList = new std::map < void *, XMLNodeList * >();
      std::map < const XMLObject *, std::vector < const XMLObject *>*>*VariableScope::parentToChildren =
        new std::map < const XMLObject *, std::vector < const XMLObject *>*>();

      VariableScope::VariableScope(int _initialSize)
    {
        position = -1;
        initialSize = _initialSize;
        scope = new std::vector < XMLObject * >();
        freePlaces = new std::stack < int >();
          parentToChildren = new std::map < const XMLObject *, std::vector < const XMLObject *>*>();
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
        delete parentToChildren;
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
            std::map < const XMLObject *, std::vector < const XMLObject *>*>::const_iterator it = parentToChildren->find(parent);

            if (it != parentToChildren->end())
            {
                it->second->push_back(&obj);
            }
            else
            {
                (*parentToChildren)[parent] = new std::vector < const XMLObject *>();

                (*parentToChildren)[parent]->push_back(&obj);
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
            (*mapLibXMLToXMLObject)[libxml] = obj;
        }
    }

    void VariableScope::registerPointers(void *libxml, XMLNodeList * nodeList)
    {
        if (libxml)
        {
            (*mapLibXMLToXMLNodeList)[libxml] = nodeList;
        }
    }

    void VariableScope::unregisterPointer(void *libxml)
    {
        if (libxml)
        {
            mapLibXMLToXMLObject->erase(libxml);
        }
    }

    void VariableScope::unregisterNodeListPointer(void *libxml)
    {
        if (libxml)
        {
            mapLibXMLToXMLNodeList->erase(libxml);
        }
    }

    XMLObject *VariableScope::getXMLObjectFromLibXMLPtr(void *libxml) const
    {
        if (libxml)
        {
            std::map < void *, XMLObject * >::const_iterator it = mapLibXMLToXMLObject->find(libxml);
            if (it != mapLibXMLToXMLObject->end())
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
            std::map < void *, XMLNodeList * >::const_iterator it = mapLibXMLToXMLNodeList->find(libxml);
            if (it != mapLibXMLToXMLNodeList->end())
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
            removeChildFromParent((*scope)[id]);
            removeDependencies((*scope)[id]);
            (*scope)[id] = 0;
            freePlaces->push(id);
        }
    }

    void VariableScope::removeDependencies(XMLObject * obj)
    {
        std::map < const XMLObject *, std::vector < const XMLObject *>*>::const_iterator it = parentToChildren->find(obj);

        if (it != parentToChildren->end())
        {
            for (unsigned int i = 0; i < it->second->size(); i++)
            {
                const XMLObject *child = (*(it->second))[i];

                if (child && getVariableFromId(child->getId()) == child)
                {
                    delete child;
                }
            }
            delete it->second;

            parentToChildren->erase(obj);
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
        std::map < void *, XMLObject * >::const_iterator it = mapLibXMLToXMLObject->find(mem);

        if (it != mapLibXMLToXMLObject->end())
        {
            delete it->second;

            mapLibXMLToXMLObject->erase(mem);
        }

        std::map < void *, XMLNodeList * >::const_iterator itnl = mapLibXMLToXMLNodeList->find(mem);

        if (itnl != mapLibXMLToXMLNodeList->end())
        {
            delete itnl->second;

            mapLibXMLToXMLNodeList->erase(mem);
        }

        XMLFreeFunc(mem);
    }

    inline void VariableScope::removeChildFromParent(const XMLObject * child)
    {
        const XMLObject *parent = child->getXMLObjectParent();
        std::map < const XMLObject *, std::vector < const XMLObject *>*>::const_iterator it = parentToChildren->find(parent);

        if (it != parentToChildren->end())
        {
            for (unsigned int i = 0; i < it->second->size(); i++)
            {
                if (child == (*(it->second))[i])
                {
                    (*(it->second))[i] = 0;
                }
            }
        }
    }
}
