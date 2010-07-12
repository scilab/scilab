/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2010-2010 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "function.hxx"
#include "context.hxx"
#include "types.hxx"
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
    if(iRhs > 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected."), "argn", 0, 1);
        return Function::Error;
    }

    //check output arguments
    if(iRhs == 1 && _iRetCount != 1)
    {//argn(x) returns only one value
        Scierror(41,_("%s: Wrong number of output arguments: %d expected.\n"), "argn", 1);
        return Function::Error;
    }
    else if(iRhs == 0 && _iRetCount > 2)
    {
        Scierror(41,_("%s: Wrong number of output arguments: %d expected.\n"), "argn", 2);
        return Function::Error;
    }

    //check input arguments types
    for(int i = 0 ; i < in.size() ; i++)
    {
        if(in[i]->getType() != InternalType::RealDouble)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "argn", i + 1);
            return Function::Error;
        }
        else
        {
            if(in[i]->getAsDouble()->size_get() != 1)
            {
                Scierror(999,_("%s: Wrong type for input argument #%d: A scalar expected.\n"), "argn", i + 1);
                return Function::Error;
            }
            else
            {
                if(in[i]->getAsDouble()->isComplex())
                {
                    Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "argn", i + 1);
                    return Function::Error;
                }
            }
        }
    }

    symbol::Context* pContext = symbol::Context::getInstance();
    InternalType *pOut = pContext->get(L"nargout");
    InternalType *pIn = pContext->get(L"nargin");

    if(pIn == NULL || pOut == NULL)
    {
        Double* pD = new Double(0,0);
        out.push_back(pD);
        out.push_back(pD);
    }
    else
    {
        if(iRhs == 1)
        {
            double dblVal = in[0]->getAsDouble()->real_get(0,0);
            if(dblVal == 1)
            {
                out.push_back(pOut);
            }
            else if(dblVal == 2)
            {
                out.push_back(pIn);
            }
            else
            {
                Scierror(999,_("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "argn", 1, "1", "2");
                return Function::Error;
            }
        }
        else
        {
            out.push_back(pOut);
            out.push_back(pIn);
        }
    }

    return types::Function::OK;
}
