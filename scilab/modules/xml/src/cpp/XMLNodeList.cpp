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

#include <string>

#include "XMLObject.hxx"
#include "XMLElement.hxx"
#include "XMLNodeList.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{

    int XMLNodeList::getNodeListSize(xmlNode * node)
    {
        xmlNode * n = node;
        int i = 0;
        for (; n; n = n->next, i++);

        return i;
    }

    XMLNodeList::XMLNodeList(XMLElement * elem)
        : XMLList()
    {
        this->elem = elem;
        this->size = getNodeListSize(elem->getRealNode());
        this->prev = 1;
        this->prevNode = elem->getRealNode();
    }

    XMLNodeList::~XMLNodeList()
    {
        scope.removeId<XMLNodeList>(id);
    }

    XMLObject * XMLNodeList::getXMLObjectParent()
    {
        return elem;
    }

    XMLObject * XMLNodeList::getListElement(int index)
    {
        if (index >= 1 && index <= size)
        {
            if (index != prev)
            {
                if (index < prev)
                {
                    for (int i = prev; i > index; i--, prevNode = prevNode->prev);
                }
                else
                {
                    for (int i = prev; i < index; i++, prevNode = prevNode->next);
                }
                prev = index;
            }

            return new XMLElement(elem->getXMLDocument(), prevNode);
        }

        return 0;
    }
}
