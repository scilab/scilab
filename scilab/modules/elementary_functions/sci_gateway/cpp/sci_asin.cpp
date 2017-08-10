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
#include "sin.h"
    int C2F(wasin)(double*, double*, double*, double*);
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();asin(a);toc
clear a;nb = 2500;a = rand(nb, nb) + 0.5;tic();asin(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();asin(a);toc
*/
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_asin(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn   = NULL;
    types::Double* pDblOut  = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "asin", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "asin", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_asin";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (pDblIn->isComplex())
    {
        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), true);
        int size = pDblIn->getSize();

        double* pInR = pDblIn->get();
        double* pInI = pDblIn->getImg();
        double* pOutR = pDblOut->get();
        double* pOutI = pDblOut->getImg();

        for (int i = 0; i < size; i++)
        {
            C2F(wasin)(pInR + i, pInI + i, pOutR + i, pOutI + i);
        }
    }
    else
    {
        bool bOutSide = 0;
        //check if all variables are between [-1,1]
        double* pInR = pDblIn->get();
        int size = pDblIn->getSize();
        for (int i = 0; i < size; i++)
        {
            if (std::fabs(pInR[i]) > 1)
            {
                bOutSide = 1;
                break;
            }
        }

        if (bOutSide) // Values outside [-1,1]
        {
            pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), true);
            double* pOutR = pDblOut->get();
            double* pOutI = pDblOut->getImg();
            double zero = 0;
            for (int i = 0; i < size; i++)
            {
                C2F(wasin)(pInR + i, &zero, pOutR + i, pOutI + i);
            }
        }
        else //all values are in [-1,1]
        {
            pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), false);
            double* pOutR = pDblOut->get();
            for (int i = 0; i < size; i++)
            {
                pOutR[i] = std::asin(pInR[i]);
            }
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
