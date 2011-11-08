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
#include "XMLDocument.hxx"
#include "XMLAttr.hxx"
#include "XMLNs.hxx"
#include "XMLNotHandledElement.hxx"
#include "XMLNodeSet.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{

    XMLNodeSet::XMLNodeSet(const XMLDocument & _doc, xmlNodeSet * _nodeSet) : XMLList(), doc(_doc)
    {
        nodeSet = _nodeSet;
        if (nodeSet)
        {
            scope->registerPointers(nodeSet, this);
            size = nodeSet->nodeNr;
        }
        else
        {
            size = 0;
        }
        scilabType = XMLSET;
        id = scope->getVariableId(*this);
    }

    XMLNodeSet::~XMLNodeSet()
    {
        scope->unregisterPointer(nodeSet);
        scope->removeId(id);
    }

    const XMLObject * XMLNodeSet::getXMLObjectParent() const
    {
        return &doc;
    }

    const XMLObject * XMLNodeSet::getListElement(int index)
    {
        if (nodeSet && index >= 1 && index <= size)
        {
            XMLObject * obj = 0;
            XMLElement * e = 0;
            xmlNode * node = nodeSet->nodeTab[index - 1];
            switch (node->type)
            {
            case XML_ELEMENT_NODE :
            case XML_TEXT_NODE :
            case XML_CDATA_SECTION_NODE :
            case XML_COMMENT_NODE :
                obj = scope->getXMLObjectFromLibXMLPtr(node);
                if (obj)
                {
                    return static_cast<XMLElement *>(obj);
                }

                return new XMLElement(doc, node);
            case XML_ATTRIBUTE_NODE :
                obj = scope->getXMLObjectFromLibXMLPtr(node->parent->properties);
                if (obj)
                {
                    return static_cast<XMLAttr *>(obj);
                }

                e = new XMLElement(doc, node->parent);

                return new XMLAttr(*e);
            case XML_NAMESPACE_DECL :
                obj = scope->getXMLObjectFromLibXMLPtr(node);
                if (obj)
                {
                    return static_cast<XMLNs *>(obj);
                }

                return new XMLNs(doc, (xmlNs *)node);
            case XML_ELEMENT_DECL :
            case XML_ATTRIBUTE_DECL :
            case XML_ENTITY_DECL :
            case XML_XINCLUDE_START :
            case XML_XINCLUDE_END :
            case XML_DOCUMENT_NODE :
                obj = scope->getXMLObjectFromLibXMLPtr(node);
                if (obj)
                {
                    return static_cast<XMLNotHandledElement *>(obj);
                }

                return new XMLNotHandledElement(doc, node);
            default :
                break;
            }
        }

        return 0;
    }
}
