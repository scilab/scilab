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

#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "Scierror.h"
#include "localization.h"

Function::ReturnValue sci_isglobal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::typed_list::iterator inIterator;
    int iWrongType = 1;

    if(in.size() != 1)
    {
        Scierror(77,_("%s: Wrong number of input argument(s): %d expected.\n"), "isgloba", 1);
        return Function::Error;
    }
    else
    {
        if(in[0]->getType() != InternalType::RealString)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: Single string expected.\n"), "isgloba", 1);
            return Function::Error;
        }

        String* pS = in[0]->getAsString();
        if(pS->size_get() != 1)
        {
            Scierror(999,_("%s: Wrong type for input argument #%d: Single string expected.\n"), "isgloba", 1);
            return Function::Error;
        }

        if(symbol::Context::getInstance()->isGlobalExists(pS->string_get(0)))
        {
            out.push_back(new Bool(1));
        }
        else
        {
            out.push_back(new Bool(0));
        }
    }
    return Function::OK;
}
