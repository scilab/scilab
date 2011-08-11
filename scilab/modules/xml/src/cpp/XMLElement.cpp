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

#include "XMLObject.hxx"
#include "XMLElement.hxx"
#include "XMLDocument.hxx"
#include "XMLNodeList.hxx"
#include "XMLNs.hxx"
#include "XMLAttr.hxx"
#include "VariableScope.hxx"

extern "C" {
    extern const char * nodes_type[];
}

namespace org_modules_xml
{
    XMLElement::XMLElement(XMLDocument * doc, xmlNode * node)
        : XMLObject()
    {
        this->node = node;
        this->doc = doc;
	scilabType = XMLELEMENT;
    }

    XMLElement::~XMLElement()
    {
        scope.removeId<XMLElement>(id);
    }

    XMLObject * XMLElement::getXMLObjectParent()
    {
        return doc;
    }

    const char * XMLElement::getNodeContent()
    {
        return (const char *)xmlNodeGetContent(node);
    }

    std::string * XMLElement::dump()
    {
        xmlBufferPtr buffer = xmlBufferCreate();
        xmlNodeDump(buffer, doc->getRealDocument(), node, 0, 1);
	std::string * str = new std::string((const char *)buffer->content);
        xmlBufferFree(buffer);

        return str;
    }

    std::string * XMLElement::toString()
    {
	std::string ns = "";
	std::string prefix = "";

	if (node->ns)
	{
	    if (node->ns->href)
	    {
		ns = std::string((const char *)node->ns->href);
	    }

	    if (node->ns->prefix)
	    {
		prefix = std::string((const char *)node->ns->prefix);
	    }
	}

        std::string str = "XML Element\n";
        str += "name: " + std::string(getNodeName()) + "\n";
	str += "namespace URI: " + ns + "\n";
        str += "namespace prefix: " + prefix + "\n";
	str += "type: " + std::string(nodes_type[getNodeType() - 1]) + "\n";
        str += "definition line: " + intToStr(node->line);
	
        return new std::string(str);
    }

    XMLNs * XMLElement::getNodeNameSpace(void)
    {
        return new XMLNs(this, node->ns);
    }

    XMLNodeList * XMLElement::getChildren(void)
    {
        return new XMLNodeList(new XMLElement(doc, node->children));
    }

    XMLAttr * XMLElement::getAttributes(void)
    {
        return new XMLAttr(this);
    }

    XMLElement * XMLElement::getParentElement(void)
    {
        return new XMLElement(doc, node->parent);
    }
}
