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
#include "VariableScope.hxx"

namespace org_modules_xml
{
    XMLAttr::XMLAttr(XMLElement * elem)
        : XMLObject()
    {
        this->elem = elem;
	scilabType = XMLATTRIBUTE;
    }

    XMLAttr::~XMLAttr()
    {
        scope.removeId<XMLAttr>(id);
    }

    XMLObject * XMLAttr::getXMLObjectParent()
    {
        return elem;
    }

    const char * XMLAttr::getAttributeValue(const char * name)
    {
        xmlNode * node = elem->getRealNode();
        xmlAttr * attrs = xmlHasProp(node, (const xmlChar *)name);
        if (attrs)
        {
            return (const char *)attrs->children->content;
        }

        return 0;
    }

    const char * XMLAttr::getAttributeValue(const char * ns, const char * name)
    {
        xmlNode * node = elem->getRealNode();
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

    std::string * XMLAttr::toString()
    {
	std::string str = "XML Attributes\n";
	xmlNode * node = elem->getRealNode();
        xmlAttr * attrs = 0;

        for (xmlAttr * cur = node->properties; cur; cur = cur->next)
        {
	    if (cur->ns)
	    {
		str += " " + std::string((const char *)cur->ns->prefix) + ":" + std::string((const char *)cur->name) + " --> " + std::string((const char *)cur->children->content) + "\n"; 
	    }
	    else
	    {
		str += " " + std::string((const char *)cur->name) + " --> " + std::string((const char *)cur->children->content) + "\n"; 
	    }
	}

	return new std::string(str);
    }
}
