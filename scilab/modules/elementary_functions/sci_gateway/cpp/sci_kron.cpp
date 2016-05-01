/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Cedric DELAMARRE
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
/*--------------------------------------------------------------------------*/
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "types_kronecker.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_kron(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblA    = NULL;
    types::Double* pDblB    = NULL;
    types::Double* pDblOut  = NULL;

    if (in.size() != 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "kron", 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "kron", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() && in[1]->isDouble()) // double .*. double
    {
        pDblA = in[0]->getAs<types::Double>();
        pDblB = in[1]->getAs<types::Double>();

        KroneckerMultiplyDoubleByDouble(pDblA, pDblB, &pDblOut);
    }
    else if (in[0]->isInt() || in[1]->isInt())
    {
        // int .*. double || double .*. int || int .*. int
        return Overload::call(L"%_kron", in, _iRetCount, out);
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_kron";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
