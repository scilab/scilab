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
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLXPath.hxx"
#include "XMLNodeSet.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{
    XMLXPath::XMLXPath(XMLDocument * doc, xmlXPathObject * xpath)
        : XMLObject()
    {
        this->doc = doc;
        this->xpath = xpath;
    }

    XMLXPath::~XMLXPath()
    {
        scope.removeId<XMLXPath>(id);
    }

    XMLObject * XMLXPath::getXMLObjectParent()
    {
        return doc;
    }

    XMLNodeSet * XMLXPath::getNodeSet()
    {
	return new XMLNodeSet(doc, xpath->nodesetval);
    }
}
