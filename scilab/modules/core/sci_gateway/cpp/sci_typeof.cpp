/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Bruno JOFRET
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
#include "internal.hxx"
#include "core_gw.hxx"
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
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "typeof", 2);
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
