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

#include "core_gw.hxx"
#include "function.hxx"
#include "callable.hxx"
#include "context.hxx"
#include "string.hxx"
#include "bool.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_isglobal(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::typed_list::iterator inIterator;
    int iWrongType = 1;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "isglobal", 1);
        return types::Function::Error;
    }
    else
    {
        if (in[0]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "isglobal", 1);
            return types::Function::Error;
        }

        types::String* pS = in[0]->getAs<types::String>();
        if (pS->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: Single string expected.\n"), "isglobal", 1);
            return types::Function::Error;
        }

        if (symbol::Context::getInstance()->isGlobalVisible(symbol::Symbol(pS->get(0))))
        {
            out.push_back(new types::Bool(1));
        }
        else
        {
            out.push_back(new types::Bool(0));
        }
    }
    return types::Function::OK;
}
