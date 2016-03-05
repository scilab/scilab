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

#include "XMLObject.hxx"
#include "XMLDocument.hxx"
#include "XMLElement.hxx"
#include "XMLXPath.hxx"
#include "XMLNodeSet.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{
XMLXPath::XMLXPath(const XMLDocument & _doc, xmlXPathObject * _xpath): XMLObject(), doc(_doc)
{
    xpath = _xpath;
    scope->registerPointers(xpath, this);
    id = scope->getVariableId(*this);
}

XMLXPath::~XMLXPath()
{
    scope->unregisterPointer(xpath);
    scope->removeId(id);
}

void *XMLXPath::getRealXMLPointer() const
{
    return static_cast < void *>(xpath);
}

const XMLObject *XMLXPath::getXMLObjectParent() const
{
    return &doc;
}

const XMLNodeSet *XMLXPath::getNodeSet() const
{
    XMLObject *obj = scope->getXMLObjectFromLibXMLPtr(xpath->nodesetval);
    if (obj)
    {
        return static_cast < XMLNodeSet * >(obj);
    }

    return new XMLNodeSet(doc, xpath);
}
}
