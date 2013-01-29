/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/

#include "functions_gw.hxx"
#include "context.hxx"
#include "string.hxx"

extern "C" {
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/

using namespace types;

Function::ReturnValue sci_whereis(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    InternalType *pIT = NULL;
    wstring stModule;

    /* Check the number of input argument */
    if (in.size() != 1)
    {
        Scierror(999, _("%s: Wrong number of input argument(s): %d expected.\n"), "whereis", 1);
        return Function::Error;
    }

    /* Check the number of output argument */
    if (_iRetCount != 1)
    {
        Scierror(999, _("%s: Wrong number of output argument(s): %d expected.\n"), "whereis", 1);
        return Function::Error;
    }

    if (in[0]->isString())
    {
        types::String* pS = in[0]->getAs<types::String>();

        if (pS->getSize() != 1)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A String expected.\n"), "whereis", 1);
            return Function::Error;
        }

        ;
        pIT = symbol::Context::getInstance()->getFunction(symbol::Symbol(pS->get(0)));
        if (pIT == NULL)
        {
            out.push_back(types::Double::Empty());
            return Function::OK;
        }
    }
    else
    {
        pIT = in[0];
    }

    switch (pIT->getType())
    {
        case InternalType::RealFunction :
        case InternalType::RealMacro :
        case InternalType::RealMacroFile :
            stModule = pIT->getAs<Callable>()->getModule();
            break;
        default :
            out.push_back(types::Double::Empty());
            return Function::OK;
    }

    out.push_back(new types::String(stModule.c_str()));
    return Function::OK;
}
/*--------------------------------------------------------------------------*/
