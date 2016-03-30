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

#include <complex>

#include "elem_func_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "overload.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
#include "sciprint.h"
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();atanh(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();atanh(a);toc
*/
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_atanh(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn = NULL;
    types::Double* pDblOut = NULL;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "atanh", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "atanh", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble())
    {
        pDblIn = in[0]->getAs<types::Double>();
        double* pInR = pDblIn->get();
        double* pInI = pDblIn->getImg();

        int iSize = pDblIn->getSize();
        bool bComplex = pDblIn->isComplex();
        bool bAlreadyDisp = false;

        if (bComplex == false)
        {
            // check values
            for (int i = 0; i < iSize; i++)
            {
                double dAbsIn = abs(pInR[i]);
                if (dAbsIn == 1)
                {
                    if (ConfigVariable::getIeee() == 0)
                    {
                        if (pInI && pDblIn->isComplex() == false)
                        {
                            delete[] pInI;
                        }

                        Scierror(78, _("%s: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), "atanh", 1);
                        return types::Function::Error;
                    }

                    if (ConfigVariable::getIeee() == 1 && ConfigVariable::getWarningMode() && bAlreadyDisp == false)
                    {
                        bAlreadyDisp = true;
                        sciprint(_("%s: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), "atanh", 1);
                    }
                }
                else if (dAbsIn > 1 && bComplex == false)
                {
                    bComplex = true;
                    pInI = new double[iSize];
                    memset(pInI, 0x00, iSize * sizeof(double));
                }
            }
        }

        pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), bComplex);
        double* pOutR = pDblOut->get();
        double* pOutI = pDblOut->getImg();

        if (bComplex)
        {
            // using scilab 5 macro atanh is faster than std::atanh (08/2015)
            // see comment a the begins of this gateway
            for (int i = 0; i < iSize; i++)
            {
                //zcoss(-pInI[i], pInR[i], &pOutR[i], &pOutI[i]);
                std::complex<double> c(pInR[i], pInI[i]);
                std::complex<double> d = std::atanh(c);

                pOutR[i] = d.real();
                pOutI[i] = d.imag();
            }

            if (pInI && pDblIn->isComplex() == false)
            {
                delete[] pInI;
            }
        }
        else
        {
            for (int i = 0; i < iSize; i++)
            {
                pOutR[i] = std::atanh(pInR[i]);
            }
        }
        out.push_back(pDblOut);
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_atanh";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
