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
    XMLNs::XMLNs(XMLObject * parent, xmlNs * ns)
        : XMLObject()
    {
        this->ns = ns;
        this->parent = parent;
	scilabType = XMLNAMESPACE;
    }

    XMLNs::~XMLNs()
    {
        scope.removeId<XMLNs>(id);
    }

    XMLObject * XMLNs::getXMLObjectParent()
    {
        return parent;
    }

    std::string * XMLNs::toString()
    {
        std::string str = "XML Namespace\n";
        str += "uri: " + std::string(this->getURI()) + "\n";
        str += "prefix: " + std::string(this->getPrefix());

        return new std::string(str);
    }
}
