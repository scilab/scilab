/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "VariableScope.hxx"
#include "XMLObject.hxx"

#define SCOPE_SIZE 1024

namespace org_modules_xml
{
    VariableScope *XMLObject::scope = 0;

      XMLObject::XMLObject()
    {
        if (!scope)
        {
            scope = new VariableScope(SCOPE_SIZE);
        }
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
