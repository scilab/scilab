/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
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

#include "core_gw.hxx"
#include "function.hxx"
#include "context.hxx"
#include "double.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

types::Function::ReturnValue sci_argn(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRhs = static_cast<int>(in.size());
    //check input arguments
    if (iRhs > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "argn", 0, 1);
        return types::Function::Error;
    }

    //check output arguments
    if (iRhs == 0 && _iRetCount > 2)
    {
        Scierror(41, _("%s: Wrong number of output arguments: %d expected.\n"), "argn", 2);
        return types::Function::Error;
    }

    //check input arguments types
    for (int i = 0 ; i < in.size() ; i++)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "argn", i + 1);
            return types::Function::Error;
        }
        else
        {
            if (in[i]->getAs<types::Double>()->getSize() != 1)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "argn", i + 1);
                return types::Function::Error;
            }
            else
            {
                if (in[i]->getAs<types::Double>()->isComplex())
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "argn", i + 1);
                    return types::Function::Error;
                }
            }
        }
    }

    symbol::Context* pContext = symbol::Context::getInstance();

    types::InternalType* pIn = pContext->get(symbol::Symbol(L"nargin"));
    types::InternalType* pOut = pContext->get(symbol::Symbol(L"nargout"));

    if (pIn == NULL || pOut == NULL)
    {
        out.push_back(types::Double::Empty());
        if (_iRetCount == 2)
        {
            out.push_back(types::Double::Empty());
        }
    }
    else
    {
        if (iRhs == 0 && _iRetCount == 1)
        {
            //arng() returns lhs
            out.push_back(pOut);
        }
        else if (iRhs == 0 && _iRetCount == 2)
        {
            //[a,b] = arng() returns lhs and rhs
            out.push_back(pOut);
            out.push_back(pIn);
        }
        else if (iRhs == 1)
        {
            //argn(x)
            double dblVal = in[0]->getAs<types::Double>()->getReal(0, 0);
            if (dblVal == 1)
            {
                //x == 1 returns lhs
                out.push_back(pOut);
            }
            else if (dblVal == 2)
            {
                //x == 2returns rhs
                out.push_back(pIn);
            }
            else if (dblVal == 0)
            {
                //x == 0 returns lhs
                out.push_back(pOut);
                if (_iRetCount == 2)
                {
                    //[a,b] = argn(0) returns lhs ans rhs
                    out.push_back(pIn);
                }
            }
            else
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' or '%s'.\n"), "argn", 1, "0", "1", "2");
                return types::Function::Error;
            }
        }
    }

    return types::Function::OK;
}
