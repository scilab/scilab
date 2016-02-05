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
#include "expm.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_expm(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn = NULL;
    types::Double* pDblOut = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "expm", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "expm", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_expm";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->getDims() > 2)
    {
        return Overload::call(L"%hm_expm", in, _iRetCount, out);
    }

    if (pDblIn->getSize() == 0)
    {
        out.push_back(types::Double::Empty());
        return types::Function::OK;
    }

    if (pDblIn->getCols() != pDblIn->getRows())
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A square matrix expected.\n"), "expm", 1);
        return types::Function::Error;
    }

    pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

    if (pDblIn->isComplex())
    {
        zexpms2(pDblIn->get(), pDblIn->getImg(), pDblOut->get(), pDblOut->getImg(), pDblIn->getCols());
    }
    else
    {
        dexpms2(pDblIn->get(), pDblOut->get(), pDblIn->getCols());
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
