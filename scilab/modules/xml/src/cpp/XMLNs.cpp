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
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{

XMLNs::XMLNs(const XMLObject & _parent, xmlNs * _ns): XMLObject(), parent(_parent)
{
    ns = _ns;
    scope->registerPointers(ns, this);
    scilabType = XMLNAMESPACE;
    id = scope->getVariableId(*this);
}

XMLNs::XMLNs(const XMLElement & elem, char *prefix, char *href): XMLObject(), parent(elem)
{
    ns = xmlNewNs(elem.getRealNode(), (const xmlChar *)href, (const xmlChar *)prefix);
    scope->registerPointers(ns, this);
    scilabType = XMLNAMESPACE;
    id = scope->getVariableId(*this);
}

XMLNs::~XMLNs()
{
    scope->unregisterPointer(ns);
    scope->removeId(id);
}

void *XMLNs::getRealXMLPointer() const
{
    return static_cast < void *>(ns);
}

const XMLObject *XMLNs::getXMLObjectParent() const
{
    return &parent;
}

const std::string XMLNs::toString() const
{
    std::ostringstream oss;

    oss << "XML Namespace" << std::endl;
    oss << "href: " << getHref() << std::endl;
    oss << "prefix: " << getPrefix();

    return oss.str();
}
}
