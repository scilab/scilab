/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Anais AUBERT
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
#include <string.h>
#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"

extern "C"
{
#include "basic_functions.h"
#include "Scierror.h"
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_sign(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRows   = 0;
    int iCols   = 0;
    int iType   = 0;
    int* piAddr = NULL;

    double *pdblReal    = NULL;
    double *pdblImg     = NULL;
    double *pdblRealRet = NULL;
    double *pdblImgRet  = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "sign", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "sign", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        types::Double* pDblIn = in[0]->getAs<types::Double>();
        int dims = pDblIn->getDims();
        int *dimsArray = pDblIn->getDimsArray();
        int size = pDblIn->getSize();

        if (pDblIn->isComplex())
        {
            types::Double* pOut = new types::Double(dims, dimsArray, true);
            double *dOutR   = pOut->getReal();
            double *dOutImg = pOut->getImg() ;
            double *dInR    = pDblIn->getReal();
            double *dInImg  = pDblIn->getImg() ;

            for (int i = 0; i < size; i++)
            {
                double dblTemp = dpythags(dInR[i], dInImg[i]);
                if (dblTemp == 0)
                {
                    dOutR[i] = 0;
                    dOutImg[i] = 0;
                }
                else
                {
                    dOutR[i] =  dInR[i] / dblTemp;
                    dOutImg[i] = dInImg[i] / dblTemp;
                }
            }

            out.push_back(pOut);
        }
        else
        {
            types::Double* pOut = new types::Double(dims, dimsArray);
            double * dOutR  = pOut->getReal();
            double * dInR   = pDblIn->getReal();

            for (int i = 0; i < size; i++)
            {
                dOutR[i] = dsignsEx(dInR[i]);
            }

            out.push_back(pOut);
        }
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_sign";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
