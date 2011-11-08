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
#include <string.h>

#include "XMLObject.hxx"
#include "XMLAttr.hxx"
#include "XMLElement.hxx"
#include "XMLDocument.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{
    XMLAttr::XMLAttr(const XMLElement & _elem) : XMLObject(), elem(_elem)
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

    void * XMLAttr::getRealXMLPointer() const
    {
        return static_cast<void *>(elem.getRealNode()->properties);
    }

    const XMLObject * XMLAttr::getXMLObjectParent() const
    {
        return &elem;
    }

    const char * XMLAttr::getAttributeValue(const char * name) const
    {
        xmlNode * node = elem.getRealNode();
        xmlAttr * attrs = xmlHasProp(node, (const xmlChar *)name);
        if (attrs)
        {
            return (const char *)attrs->children->content;
        }

        return 0;
    }

    const char * XMLAttr::getAttributeValue(const char * ns, const char * name) const
    {
        xmlNode * node = elem.getRealNode();
        xmlAttr * attrs = 0;
        for (xmlAttr * cur = node->properties; cur; cur = cur->next)
        {
            if (cur->ns && !strcmp(name, (const char *)cur->name) && (!strcmp(ns, (const char *)cur->ns->prefix) || !strcmp(ns, (const char *)cur->ns->href)))
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

    const char * XMLAttr::getAttributeValue(int index) const
    {
        xmlNode * node = elem.getRealNode();
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

    void XMLAttr::setAttributeValue(const char * prefix, const char * name, const char * value) const
    {
        xmlNode * node = elem.getRealNode();
        xmlAttr * attrs = 0;
        for (xmlAttr * cur = node->properties; cur; cur = cur->next)
        {
            if (cur->ns && !strcmp(name, (const char *)cur->name) && (!strcmp(prefix, (const char *)cur->ns->prefix) || !strcmp(prefix, (const char *)cur->ns->href)))
            {
                attrs = cur;
                break;
            }
        }

        if (attrs)
        {
            xmlSetNsProp(node, attrs->ns, (const xmlChar *)name, (const xmlChar *)value);
        }
        else if (strlen(value))
        {
            xmlNs * ns = 0;
            if (!strncmp(prefix, "http://", strlen("http://")))
            {
                ns = xmlSearchNsByHref(elem.getXMLDocument().getRealDocument(), node, (const xmlChar *)prefix);
            }
            else
            {
                ns = xmlSearchNs(elem.getXMLDocument().getRealDocument(), node, (const xmlChar *)prefix);
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

    void XMLAttr::setAttributeValue(int index, const char * value) const
    {
        xmlNode * node = elem.getRealNode();
        unsigned int i = 1;
        for (xmlAttr * cur = node->properties; cur; cur = cur->next, i++)
        {
            if (i == index)
            {
                if (strlen(value))
                {
                    cur->children->content = xmlStrdup((const xmlChar *)value);
                }
                else
                {
                    cur->children->content = xmlStrdup((const xmlChar *)"");
                }
            }
        }
    }

    void XMLAttr::setAttributeValue(const char * name, const char * value) const
    {
        xmlNode * node = elem.getRealNode();
        xmlAttr * attrs = xmlHasProp(node, (const xmlChar *)name);
        if (attrs)
        {
            xmlSetProp(node, (const xmlChar *)name, (const xmlChar *)value);
        }
        else
        {
            if (strlen(value))
            {
                xmlNewProp(node, (const xmlChar *)name, (const xmlChar *)value);
            }
        }
    }

    int XMLAttr::getSize() const
    {
        int i = 0;
        for (xmlAttr * cur = elem.getRealNode()->properties; cur; cur = cur->next, i++);

        return i;
    }

    const std::string XMLAttr::toString() const
    {
        std::ostringstream oss;
        xmlNode * node = elem.getRealNode();

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
}
