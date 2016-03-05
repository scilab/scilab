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

#include "VariableScope.hxx"
#include "XMLObject.hxx"

#define SCOPE_SIZE 1024

namespace org_modules_xml
{
VariableScope *XMLObject::scope = 0;

#ifdef SCILAB_DEBUG_XML
std::set<XMLObject *> XMLObject::pointers;
#endif

XMLObject::XMLObject(): id(0), valid(true)
{
    if (!scope)
    {
        scope = new VariableScope(SCOPE_SIZE);
    }

#ifdef SCILAB_DEBUG_XML
    XMLObject::pointers.insert(this);
    //std::cout << "Create = " << (void*)this << std::endl;
#endif

    scilabType = -1;
}

XMLObject *XMLObject::getVariableFromId(int id)
{
    if (!scope)
    {
        return 0;
    }

    return scope->getVariableFromId(id);
}

int XMLObject::createOnStack(int pos, void *pvApiCtx) const
{
    if (scilabType != -1)
    {
        return createXMLObjectAtPos(scilabType, pos, id, pvApiCtx);
    }

    return 0;
}

void XMLObject::resetScope()
{
    if (scope)
    {
        delete scope;

        scope = 0;
    }
}
}
