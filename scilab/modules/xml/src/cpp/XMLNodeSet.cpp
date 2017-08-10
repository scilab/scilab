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

XMLNodeSet::XMLNodeSet(const XMLDocument & _doc, xmlXPathObject * _xpath): XMLList(), doc(_doc), xpath(_xpath)
{
    nodeSet = xpath->nodesetval;
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
    xmlXPathFreeObject(xpath);
}

void *XMLNodeSet::getRealXMLPointer() const
{
    return static_cast < void *>(nodeSet);
}

const char **XMLNodeSet::getContentFromList() const
{
    const char **list = new const char *[size];
    for (int i = 0; i < size; i++)
    {
        list[i] = (const char *)xmlNodeGetContent(nodeSet->nodeTab[i]);
    }

    return list;
}

const char **XMLNodeSet::getNameFromList() const
{
    const char **list = new const char *[size];
    for (int i = 0; i < size; i++)
    {
        list[i] = nodeSet->nodeTab[i]->name ? (const char *)nodeSet->nodeTab[i]->name : "";
    }

    return list;
}

void XMLNodeSet::setAttributeValue(const char **prefix, const char **name, const char **value, int lsize) const
{
    for (int i = 0; i < size; i++)
    {
        XMLAttr::setAttributeValue(nodeSet->nodeTab[i], prefix, name, value, lsize);
    }
}

void XMLNodeSet::setAttributeValue(const char **name, const char **value, int lsize) const
{
    for (int i = 0; i < size; i++)
    {
        XMLAttr::setAttributeValue(nodeSet->nodeTab[i], name, value, lsize);
    }
}

void XMLNodeSet::remove() const
{
    for (int i = 0; i < size; i++)
    {
        xmlNode *node = nodeSet->nodeTab[i];
        xmlUnlinkNode(node);
        xmlFreeNode(node);
    }
}

const XMLObject *XMLNodeSet::getXMLObjectParent() const
{
    return &doc;
}

const XMLObject *XMLNodeSet::getListElement(int index)
{
    if (nodeSet && index >= 1 && index <= size)
    {
        XMLObject *obj = 0;
        xmlNode *node = nodeSet->nodeTab[index - 1];

        switch (node->type)
        {
            case XML_ELEMENT_NODE:
            case XML_TEXT_NODE:
            case XML_CDATA_SECTION_NODE:
            case XML_COMMENT_NODE:
            case XML_ATTRIBUTE_NODE:
                obj = scope->getXMLObjectFromLibXMLPtr(node);
                if (obj)
                {
                    return static_cast < XMLElement * >(obj);
                }

                return new XMLElement(doc, node);
            case XML_NAMESPACE_DECL:
                obj = scope->getXMLObjectFromLibXMLPtr(node);
                if (obj)
                {
                    return static_cast < XMLNs * >(obj);
                }

                return new XMLNs(doc, (xmlNs *) node);
            case XML_ELEMENT_DECL:
            case XML_ATTRIBUTE_DECL:
            case XML_ENTITY_DECL:
            case XML_XINCLUDE_START:
            case XML_XINCLUDE_END:
            case XML_DOCUMENT_NODE:
                obj = scope->getXMLObjectFromLibXMLPtr(node);
                if (obj)
                {
                    return static_cast < XMLNotHandledElement * >(obj);
                }

                return new XMLNotHandledElement(doc, node);
            default:
                break;
        }
    }

    return 0;
}
}
