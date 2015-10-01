/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "internal.hxx"
#include "types_gw.hxx"
#include "function.hxx"
#include "overload.hxx"
#include "string.hxx"
#include "context.hxx"

extern "C" {
#include "Scierror.h"
#include "localization.h"
}

#define OPTION L"overload"

types::Function::ReturnValue sci_typeof(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(999, _("%s: Wrong number of input arguments: %d or %d expected.\n"), "typeof", 1, 2);
        return types::Function::Error;
    }

    // Old typeof call
    if (in.size() == 1)
    {
        // for compatibilities with scilab 5 (cell and struct was tlist)
        if (in[0]->isStruct() || in[0]->isCell())
        {
            out.push_back(new types::String(in[0]->getShortTypeStr().c_str()));
            return types::Function::OK;
        }

        // Special cases for rational and state-space list
        if (in[0]->isTList())
        {
            if (in[0]->getShortTypeStr() == L"r")
            {
                out.push_back(new types::String(L"rational"));
                return types::Function::OK;
            }
            else if (in[0]->getShortTypeStr() == L"lss")
            {
                out.push_back(new types::String(L"state-space"));
                return types::Function::OK;
            }
        }

        out.push_back(new types::String(in[0]->getTypeStr().c_str()));
        return types::Function::OK;
    }

    // Check second argument is a string
    if (in[1]->isString() == false || in[1]->getAs<types::String>()->getSize() != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), "typeof", 2);
        return types::Function::Error;
    }

    if (wcscmp(in[1]->getAs<types::String>()->get(0, 0), OPTION) == 0)
    {
        out.push_back(new types::String(in[0]->getShortTypeStr().c_str()));
        return types::Function::OK;
    }
    else
    {
        Scierror(999, _("%s: Wrong values for input argument #%d: '%s' expected .\n"), "typeof", 2, OPTION);
        return types::Function::Error;
    }

    return types::Function::Error;

}
