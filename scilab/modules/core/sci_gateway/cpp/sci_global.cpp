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

types::Function::ReturnValue sci_global(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    //check input arguments
    for(int i = 0 ; i < in.size() ; i++)
    {
        if(in[i]->getType() != InternalType::RealString)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"), "global", i + 1);
            return Function::Error;
        }

        if(in[i]->getAsString()->size_get() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "global", i + 1);
            return Function::Error;
        }
    }

    //check output arguments
    if(_iRetCount > 1)
    {
        Scierror(999,_("%s: Wrong number of output arguments: At most %d expected.\n"), "global", 1);
        return Function::Error;
    }

    symbol::Context* pCtx = symbol::Context::getInstance();

    for(int i = 0 ; i < in.size() ; i++)
    {
        char* pstVar = in[i]->getAsString()->string_get(0);

        //does it visible in current global scope
        if(pCtx->isGlobalVisible(pstVar) == false)
        {
            //does it exist in global
            if(pCtx->isGlobalExists(pstVar) == false)
            {
                InternalType* pIT = pCtx->get(pstVar);
                if(pIT)
                {//variable have already a value in current local scope
                    
                    //set global at local value
                    pCtx->setGlobalValue(pstVar, *pIT);
                    pCtx->remove(pstVar);
                }
                else
                {
                    //create global variable with default value []
                    pCtx->createEmptyGlobalValue(pstVar);
                }
            }

            //set visible in current global scope
            pCtx->setGlobalVisible(pstVar);
        }
    }
    return types::Function::OK;
}
