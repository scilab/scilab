/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "XMLObject.hxx"
#include "XMLElement.hxx"
#include "XMLNs.hxx"
#include "VariableScope.hxx"

namespace org_modules_xml
{

    XMLNs::XMLNs(const XMLObject & _parent, xmlNs * _ns):XMLObject(), parent(_parent)
    {
        ns = _ns;
        scope->registerPointers(ns, this);
        scilabType = XMLNAMESPACE;
        id = scope->getVariableId(*this);
    }

    XMLNs::XMLNs(const XMLElement & elem, char *prefix, char *href):XMLObject(), parent(elem)
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
