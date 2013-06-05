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
#include "string.hxx"
#include "core_gw.hxx"

extern "C"
{
#include "localization.h"
#include "Scierror.h"
}

using namespace types;

types::Function::ReturnValue sci_global(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //check input arguments
    for (int i = 0 ; i < in.size() ; i++)
    {
        if (in[i]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: String expected.\n"), "global", i + 1);
            return Function::Error;
        }

        if (in[i]->getAs<types::String>()->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "global", i + 1);
            return Function::Error;
        }
    }

    //check output arguments
    if (_iRetCount > 1)
    {
        Scierror(999, _("%s: Wrong number of output arguments: At most %d expected.\n"), "global", 1);
        return Function::Error;
    }

    symbol::Context* pCtx = symbol::Context::getInstance();

    for (int i = 0 ; i < in.size() ; i++)
    {
        symbol::Symbol pstVar = *new symbol::Symbol(in[i]->getAs<types::String>()->get(0));

        //does it visible in current global scope
        if (pCtx->isGlobalVisible(pstVar) == false)
        {
            //does it exist in global
            if (pCtx->isGlobalExists(pstVar) == false)
            {
                InternalType* pIT = pCtx->get(pstVar);
                //create global variable with default value []
                if (pIT)
                {
                    //protect var againt setGlobalVisible
                    pIT->IncreaseRef();
                }

                //create a empty global variable => []
                pCtx->createEmptyGlobalValue(pstVar);

                //set visible in current global scope
                pCtx->setGlobalVisible(pstVar);

                if (pIT)
                {
                    //assign old local value
                    pCtx->put(pstVar, *pIT);
                    //unprotect var againt setGlobalVisible
                    pIT->DecreaseRef();
                }
            }
            else
            {
                //set visible in current global scope
                pCtx->setGlobalVisible(pstVar);
            }
        }
    }
    return types::Function::OK;
}
