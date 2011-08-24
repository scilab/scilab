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
    XMLXPath::XMLXPath(const XMLDocument & _doc, xmlXPathObject * _xpath) : XMLObject(), doc(_doc)
    {
        xpath = _xpath;
        scope.registerPointers(xpath, this);
    }

    XMLXPath::~XMLXPath()
    {
        scope.unregisterPointer(xpath);
        scope.removeId<XMLXPath>(id);
    }

    const XMLObject * XMLXPath::getXMLObjectParent() const
    {
        return &doc;
    }

    const XMLNodeSet * XMLXPath::getNodeSet() const
    {
        return new XMLNodeSet(doc, xpath->nodesetval);
    }
}
