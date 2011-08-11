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

    XMLNodeSet::XMLNodeSet(XMLDocument * doc, xmlNodeSet * nodeSet)
        : XMLList()
    {
        this->doc = doc;
        this->nodeSet = nodeSet;
	if (nodeSet)
	{
	    this->size = nodeSet->nodeNr;
	}
	else
	{
	    this->size = 0;
	}
    }

    XMLNodeSet::~XMLNodeSet()
    {
        scope.removeId<XMLNodeSet>(id);
    }

    XMLObject * XMLNodeSet::getXMLObjectParent()
    {
        return doc;
    }

    XMLObject * XMLNodeSet::getListElement(int index)
    {
        if (nodeSet && index >= 1 && index <= size)
        {
	    xmlNode * node = nodeSet->nodeTab[index - 1];
	    switch (node->type)
	    {
	    case XML_ELEMENT_NODE :
	    case XML_TEXT_NODE :
	    case XML_CDATA_SECTION_NODE :
	    case XML_COMMENT_NODE :
		return new XMLElement(doc, node);
	    case XML_ATTRIBUTE_NODE :
		return new XMLAttr(new XMLElement(doc, node->parent));
	    case XML_NAMESPACE_DECL :
		return new XMLNs(doc, (xmlNs *) node);
	    case XML_ELEMENT_DECL :
	    case XML_ATTRIBUTE_DECL :
	    case XML_ENTITY_DECL :
	    case XML_XINCLUDE_START :
	    case XML_XINCLUDE_END:
		return new XMLNotHandledElement(doc, node);
	    }

	}
	
        return 0;
    }
}
