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
#include "XMLNotHandledElement.hxx"
#include "VariableScope.hxx"

extern "C"
{
    extern const char *nodes_type[];
}

namespace org_modules_xml
{
    XMLNotHandledElement::XMLNotHandledElement(const XMLObject & _parent, xmlNode * _node):XMLObject(), parent(_parent)
    {
        node = _node;
        scilabType = XMLNOTHANDLED;
        id = scope->getVariableId(*this);
        scope->registerPointers(node, this);
    }

    XMLNotHandledElement::~XMLNotHandledElement()
    {
        scope->unregisterPointer(node);
        scope->removeId(id);
    }

    void *XMLNotHandledElement::getRealXMLPointer() const
    {
        return static_cast < void *>(node);
    }

    const XMLObject *XMLNotHandledElement::getXMLObjectParent() const
    {
        return &parent;
    }

    const std::string XMLNotHandledElement::toString() const
    {
        std::ostringstream oss;

        oss << "Not handled XML Element" << std::endl;
        oss << "type: " << nodes_type[node->type - 1];

        return oss.str();
    }
}
