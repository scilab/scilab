/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2013 - Scilab Enterprises - Cedric Delamarre
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "integer_gw.hxx"
#include "function.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

types::Function::ReturnValue sci_iconvert(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "iconvert", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "iconvert", 1);
        return types::Function::Error;
    }

    if (in[0]->isInt() == false && in[0]->isDouble() == false && in[0]->isBool() == false)
    {
        // call overload
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_iconvert";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    // perfom operation
    return Overload::call(L"%_iconvert", in, _iRetCount, out);
}
