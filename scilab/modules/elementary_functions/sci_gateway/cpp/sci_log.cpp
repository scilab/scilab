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
int C2F(wlog)(double*, double*, double*, double*);
int C2F(dscal)(int*, double*, double*, int*);
}

/*
clear a;nb = 2500;a = rand(nb, nb);tic();log(a);toc
clear a;nb = 2500;a = -rand(nb, nb);tic();log(a);toc
clear a;nb = 2500;a = rand(nb, nb); a = a + a *%i;tic();log(a);toc
*/

types::Function::ReturnValue log_or_log10(types::typed_list &in, int _iRetCount, types::typed_list &out, bool bLog10);

/*--------------------------------------------------------------------------*/

types::Function::ReturnValue sci_log(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return log_or_log10(in, _iRetCount, out, false);
}

types::Function::ReturnValue sci_log10(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    return log_or_log10(in, _iRetCount, out, true);
}

/*--------------------------------------------------------------------------*/

types::Function::ReturnValue log_or_log10(types::typed_list &in, int _iRetCount, types::typed_list &out, bool bLog10)
{
    double inverseLog10 = 0.434294481903251827651128918916605082294397005803666566114;
    auto fname = bLog10 ? "log10" : "log";

    int iAlert = 1;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), fname, 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + (bLog10 ? L"_log10" : L"_log");
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    types::Double* pDblIn = in[0]->getAs<types::Double>();
    types::Double* pDblOut = new types::Double(pDblIn->getDims(), pDblIn->getDimsArray(), pDblIn->isComplex());

    double* pInR = pDblIn->get();
    double* pOutR = pDblOut->get();
    int size = pDblIn->getSize();
    if (pDblIn->isComplex())
    {
        double* pInI = pDblIn->getImg();
        double* pOutI = pDblOut->getImg();
        for (int i = 0; i < size; i++)
        {
            //If the value is less than precision (eps).
            if (iAlert && pInR[i] == 0 && pInI[i] == 0)
            {
                if (ConfigVariable::getIeee() == 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d : Singularity of the function.\n"), fname, 1);
                    return types::Function::Error;
                }
                else if (ConfigVariable::getIeee() == 1)
                {
                    if (ConfigVariable::getWarningMode())
                    {
                        sciprint(_("%ls: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), fname, 1);
                    }
                }

                iAlert = 0;
            }

            C2F(wlog)(pInR + i, pInI + i, pOutR + i, pOutI + i);
        }

        if (bLog10)
        {
            int iOne = 1;
            C2F(dscal)(&size, &inverseLog10, pOutR, &iOne);
            C2F(dscal)(&size, &inverseLog10, pOutI, &iOne);
        }
    }
    else
    {
        bool bIsLessZero = false;
        for (int i = 0; i < size; i++)
        {
            if (iAlert && pInR[i] == 0)
            {
                if (ConfigVariable::getIeee() == 0)
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d : Singularity of the function.\n"), fname, 1);
                    return types::Function::Error;
                }
                else if (ConfigVariable::getIeee() == 1)
                {
                    if (ConfigVariable::getWarningMode())
                    {
                        sciprint(_("%ls: Warning: Wrong value for input argument #%d : Singularity of the function.\n"), fname, 1);
                    }
                }

                iAlert = 0;
            }
            else if (pInR[i] < 0)
            {
                bIsLessZero = true;
            }
        }

        if (bIsLessZero)
        {
            pDblOut->setComplex(true);
            double zero = 0;
            double* pOutI = pDblOut->getImg();

            for (int i = 0; i < size; i++)
            {
                C2F(wlog)(pInR + i, &zero, pOutR + i, pOutI + i);
            }

            if (bLog10)
            {
                int iOne = 1;
                C2F(dscal)(&size, &inverseLog10, pOutR, &iOne);
                C2F(dscal)(&size, &inverseLog10, pOutI, &iOne);
            }
        }
        else
        {
            for (int i = 0; i < size; i++)
            {
                pOutR[i] = std::log(pInR[i]);
            }

            if (bLog10)
            {
                int iOne = 1;
                C2F(dscal)(&size, &inverseLog10, pOutR, &iOne);
            }
        }
    }

    out.push_back(pDblOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
