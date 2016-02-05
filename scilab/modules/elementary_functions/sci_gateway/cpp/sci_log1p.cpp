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
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "sciprint.h"
#include "localization.h"
#include "elem_common.h"
#include "log.h"
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();log1p(a);toc
*/
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_log1p(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iAlert = 1;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "log1p", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "log1p", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_log1p";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::Double* pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->isComplex())
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A real expected.\n"), "log1p", 1);
        return types::Function::Error;
    }

    double* pInR = pDblIn->get();
    int size = pDblIn->getSize();
    for (int i = 0; i < size; i++)
    {
        if (pInR[i] <= -1)
        {
            if (ConfigVariable::getIeee() == 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d : Singularity of the function.\n"), "log1p", 1);
                return types::Function::Error;
            }
            else if (ConfigVariable::getIeee() == 1)
            {
                if (ConfigVariable::getWarningMode())
                {
                    sciprint(_("%s: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), "log1p", 1);
                    break;
                }
            }
        }
    }

    types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray());
    double* pOutR = pDblOut->get();

    for (int i = 0; i < size; i++)
    {
        pOutR[i] = dlog1ps(pInR[i]);
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
