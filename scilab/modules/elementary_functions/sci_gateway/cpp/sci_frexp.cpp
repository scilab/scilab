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

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "basic_functions.h"
}
/*
clear a; nb = 2500; a = rand(nb, nb); tic(); [x,y]=frexp(a); toc
*/

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_frexp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "frexp", 1);
        return types::Function::Error;
    }

    if (_iRetCount != 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "frexp", 2);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_frexp";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::Double* pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->getDims() > 2)
    {
        std::wstring wstFuncName = L"%hm_frexp";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    if (pDblIn->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: Real matrix expected.\n"), "frexp", 1);
        return types::Function::Error;
    }

    types::Double* pDblCoef = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
    types::Double* pDblExp  = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());

    double* pIn = pDblIn->get();
    double* pCoef = pDblCoef->get();
    double* pFrexp = pDblExp->get();
    int size = pDblIn->getSize();

    for (int i = 0; i < size; i++)
    {
        pCoef[i] = dfrexps(pIn[i], pFrexp + i);
    }

    out.push_back(pDblCoef);
    out.push_back(pDblExp);

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
