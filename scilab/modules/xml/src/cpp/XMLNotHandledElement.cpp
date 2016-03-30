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
#include "XMLNotHandledElement.hxx"
#include "VariableScope.hxx"

extern "C"
{
    extern const char *nodes_type[];
}

namespace org_modules_xml
{
XMLNotHandledElement::XMLNotHandledElement(const XMLObject & _parent, xmlNode * _node): XMLObject(), parent(_parent)
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
