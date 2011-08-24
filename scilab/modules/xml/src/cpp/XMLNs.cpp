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
#include "XMLNs.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{

    XMLNs::XMLNs(const XMLObject & _parent, xmlNs * _ns) : XMLObject(), parent(_parent)
    {
        ns = _ns;
        scope.registerPointers(ns, this);
        scilabType = XMLNAMESPACE;
    }

    XMLNs::XMLNs(const XMLElement & elem, char * prefix, char * href) : XMLObject(), parent(elem)
    {
        ns = xmlNewNs(elem.getRealNode(), (const xmlChar *)href, (const xmlChar *)prefix);
        scope.registerPointers(ns, this);
        scilabType = XMLNAMESPACE;
    }

    XMLNs::~XMLNs()
    {
        scope.unregisterPointer(ns);
        scope.removeId<XMLNs>(id);
    }

    const XMLObject * XMLNs::getXMLObjectParent() const
    {
        return &parent;
    }

    const std::string XMLNs::toString() const
    {
        std::string str = "XML Namespace\n";
        str += "href: " + std::string(getHref()) + "\n";
        str += "prefix: " + std::string(getPrefix());

        return str;
    }
}
