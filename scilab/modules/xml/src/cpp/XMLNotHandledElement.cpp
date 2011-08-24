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
#include "XMLNotHandledElement.hxx"
#include "VariableScope.hxx"

extern "C" {
    extern const char * nodes_type[];
}

namespace org_modules_xml
{
    XMLNotHandledElement::XMLNotHandledElement(const XMLObject & _parent, xmlNode * _node) : XMLObject(), parent(_parent)
    {
        node = _node;
        scilabType = XMLNOTHANDLED;
    }

    XMLNotHandledElement::~XMLNotHandledElement()
    {
        scope.removeId<XMLNotHandledElement>(id);
    }

    const XMLObject * XMLNotHandledElement::getXMLObjectParent() const
    {
        return &parent;
    }

    const std::string XMLNotHandledElement::toString() const
    {
        return std::string("Not handled XML Element\ntype") + std::string(nodes_type[node->type - 1]);
    }
}
