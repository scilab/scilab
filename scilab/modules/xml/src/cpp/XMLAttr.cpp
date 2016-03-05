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
#include <string.h>

#include "XMLObject.hxx"
#include "XMLAttr.hxx"
#include "XMLElement.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{
XMLAttr::XMLAttr(const XMLElement & _elem): XMLObject(), elem(_elem)
{
    scilabType = XMLATTRIBUTE;
    id = scope->getVariableId(*this);
    scope->registerPointers(elem.getRealNode()->properties, this);
}

XMLAttr::~XMLAttr()
{
    scope->unregisterPointer(elem.getRealNode()->properties);
    scope->removeId(id);
}

void *XMLAttr::getRealXMLPointer() const
{
    return static_cast < void *>(elem.getRealNode()->properties);
}

const XMLObject *XMLAttr::getXMLObjectParent() const
{
    return &elem;
}

const char *XMLAttr::getAttributeValue(const char *name) const
{
    xmlNode *node = elem.getRealNode();
    xmlAttr *attrs = xmlHasProp(node, (const xmlChar *)name);
    if (attrs)
    {
        return (const char *)attrs->children->content;
    }

    return 0;
}

const char *XMLAttr::getAttributeValue(const char *ns, const char *name) const
{
    xmlNode *node = elem.getRealNode();
    xmlAttr *attrs = 0;
    for (xmlAttr * cur = node->properties; cur; cur = cur->next)
    {
        if (cur->ns && !strcmp(name, (const char *)cur->name)
                && (!strcmp(ns, (const char *)cur->ns->prefix) || !strcmp(ns, (const char *)cur->ns->href)))
        {
            attrs = cur;
            break;
        }
    }

    if (attrs)
    {
        return (const char *)attrs->children->content;
    }

    return 0;
}

const char *XMLAttr::getAttributeValue(int index) const
{
    xmlNode *node = elem.getRealNode();
    int i = 1;
    for (xmlAttr * cur = node->properties; cur; cur = cur->next, i++)
    {
        if (i == index)
        {
            return (const char *)cur->children->content;
        }
    }

    return 0;
}

void XMLAttr::setAttributeValue(const char *prefix, const char *name, const char *value) const
{
    setAttributeValue(elem.getRealNode(), prefix, name, value);
}

void XMLAttr::setAttributeValue(xmlNode * node, const char *prefix, const char *name, const char *value)
{
    if (node && node->type == XML_ELEMENT_NODE)
    {
        xmlAttr *attrs = 0;

        for (xmlAttr * cur = node->properties; cur; cur = cur->next)
        {
            if (cur->ns && !strcmp(name, (const char *)cur->name)
                    && (!strcmp(prefix, (const char *)cur->ns->prefix) || !strcmp(prefix, (const char *)cur->ns->href)))
            {
                attrs = cur;
                break;
            }
        }

        if (attrs)
        {
            xmlSetNsProp(node, attrs->ns, (const xmlChar *)name, (const xmlChar *)value);
        }
        else
        {
            xmlNs *ns = 0;

            if (!strncmp(prefix, "http://", strlen("http://")))
            {
                ns = xmlSearchNsByHref(node->doc, node, (const xmlChar *)prefix);
            }
            else
            {
                ns = xmlSearchNs(node->doc, node, (const xmlChar *)prefix);
            }

            if (ns)
            {
                xmlSetNsProp(node, ns, (const xmlChar *)name, (const xmlChar *)value);
            }
            else
            {
                xmlSetProp(node, (const xmlChar *)name, (const xmlChar *)value);
            }
        }
    }
}

void XMLAttr::setAttributeValue(xmlNode * node, const char **prefix, const char **name, const char **value, int size)
{
    if (node && node->type == XML_ELEMENT_NODE)
    {
        for (int i = 0; i < size; i++)
        {
            setAttributeValue(node, prefix[i], name[i], value[i]);
        }
    }
}

void XMLAttr::setAttributeValue(const char **prefix, const char **name, const char **value, int size) const
{
    for (int i = 0; i < size; i++)
    {
        setAttributeValue(prefix[i], name[i], value[i]);
    }
}

void XMLAttr::setAttributeValue(int index, const char *value) const
{
    setAttributeValue(elem.getRealNode(), index, value);
}

void XMLAttr::setAttributeValue(xmlNode * node, int index, const char *value)
{
    if (node && node->type == XML_ELEMENT_NODE)
    {
        int i = 1;

        for (xmlAttr * cur = node->properties; cur; cur = cur->next, i++)
        {
            if (i == index)
            {
                cur->children->content = xmlStrdup((const xmlChar *)value);
            }
        }
    }
}

void XMLAttr::setAttributeValue(const char *name, const char *value) const
{
    setAttributeValue(elem.getRealNode(), name, value);
}

void XMLAttr::setAttributeValue(xmlNode * node, const char *name, const char *value)
{
    if (node && node->type == XML_ELEMENT_NODE)
    {
        xmlAttr *attrs = xmlHasProp(node, (const xmlChar *)name);

        if (attrs)
        {
            xmlSetProp(node, (const xmlChar *)name, (const xmlChar *)value);
        }
        else
        {
            xmlNewProp(node, (const xmlChar *)name, (const xmlChar *)value);
        }
    }
}

void XMLAttr::setAttributeValue(xmlNode * node, const char **name, const char **value, int size)
{
    if (node && node->type == XML_ELEMENT_NODE)
    {
        for (int i = 0; i < size; i++)
        {
            setAttributeValue(node, name[i], value[i]);
        }
    }
}

void XMLAttr::setAttributeValue(const char **name, const char **value, int size) const
{
    for (int i = 0; i < size; i++)
    {
        setAttributeValue(name[i], value[i]);
    }
}

int XMLAttr::getSize() const
{
    return getSize(elem.getRealNode()->properties);
}

int XMLAttr::getSize(xmlAttr * attr)
{
    int i = 0;
    for (xmlAttr * cur = attr; cur; cur = cur->next, i++)
    {
        ;
    }

    return i;
}

const std::string XMLAttr::toString() const
{
    std::ostringstream oss;
    xmlNode *node = elem.getRealNode();

    oss << "XML Attributes" << std::endl;
    for (xmlAttr * cur = node->properties; cur; cur = cur->next)
    {
        if (cur->ns)
        {
            oss << cur->ns->prefix << ":" << cur->name << " --> " << cur->children->content << std::endl;
        }
        else
        {
            oss << cur->name << " --> " << cur->children->content << std::endl;
        }
    }

    return oss.str();
}

const char ** XMLAttr::getNames() const
{
    int size = getSize();
    int i = 0;
    char ** arr = new char*[size + 1];
    xmlNode * node = elem.getRealNode();

    for (xmlAttr * cur = node->properties; cur; cur = cur->next, i++)
    {
        arr[i] = (char *)cur->name;
    }
    arr[size] = 0;

    return const_cast<const char**>(arr);
}
}
