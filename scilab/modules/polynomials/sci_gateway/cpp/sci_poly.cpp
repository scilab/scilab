/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
 * Copyright (C) 2015 - Scilab Enterprises - Anais AUBERT
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
#include "polynomials_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "string.hxx"
#include "polynom.hxx"
#include "overload.hxx"
#include "context.hxx"
#include "execvisitor.hxx"

extern "C"
{
#include <math.h>
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"

    extern int C2F(dprxc)(int*, double*, double*);
    extern int C2F(wprxc)(int*, double*, double*, double*, double*);
    extern double C2F(dasum)(int*, double*, int*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_poly(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblIn    = NULL;
    types::String* pStrName  = NULL;
    types::Polynom* pPolyOut = NULL;

    std::wstring wstrFlag = L"roots"; // roots (default), coeff

    if (in.size() < 2 || in.size() > 3)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "poly", 2, 3);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "poly", 1);
        return types::Function::Error;
    }

    if (in[0]->isDouble() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_poly";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pDblIn = in[0]->getAs<types::Double>();

    if (in.size() == 3)
    {
        if (in[2]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "poly", 3);
            return types::Function::Error;
        }

        wstrFlag = in[2]->getAs<types::String>()->get(0);
        if (wstrFlag != L"roots" && wstrFlag != L"coeff" && wstrFlag != L"r" && wstrFlag != L"c")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : ""%s"" or ""%s"" expected.\n"), "poly", 3, "roots", "coeff");
            return types::Function::Error;
        }
    }

    if (in[1]->isString() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "poly", 2);
        return types::Function::Error;
    }

    pStrName = in[1]->getAs<types::String>();
    if (pStrName->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), "poly", 2);
        return types::Function::Error;
    }

    std::wstring wstrName = pStrName->get(0);
    size_t badpos = wstrName.find_first_not_of(L"$abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    if (badpos != std::wstring::npos)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d : Valid variable name expected.\n"), "poly", 2);
        return types::Function::Error;
    }

    if (wstrFlag == L"roots" || wstrFlag == L"r") // roots
    {
        // [] case
        if (pDblIn->getSize() == 0)
        {
            int *iRank = new int[1];
            *iRank = 0;
            types::Polynom* pPolyOut = new types::Polynom(wstrName, 1, 1, iRank);
            delete[] iRank;
            double* pdblCoef = pPolyOut->get(0)->get();
            *pdblCoef = 1;
            out.push_back(pPolyOut);
            return types::Function::OK;
        }

        bool bDeleteInput = false;
        if (pDblIn->getSize() != 1 && pDblIn->getCols() == pDblIn->getRows())
        {
            //call spec
            types::typed_list tlInput;
            types::typed_list tlOutput;
            types::optional_list tlOpt;
            tlInput.push_back(pDblIn);
            types::Function *funcSpec = symbol::Context::getInstance()->get(symbol::Symbol(L"spec"))->getAs<types::Function>();
            funcSpec->call(tlInput, tlOpt, 1, tlOutput);
            pDblIn = tlOutput[0]->getAs<types::Double>();
            bDeleteInput = true;
        }

        int iOne = 1;
        double* pdblInReal = pDblIn->get();
        int piDimsArray[2] = {1, 1};
        int iSize = pDblIn->getSize();
        int *iRanks = new int[1];
        *iRanks = iSize;
        pPolyOut = new types::Polynom(wstrName, 2, piDimsArray, iRanks);
        double* pdblCoefReal = pPolyOut->get(0)->get();
        if (pDblIn->isComplex())
        {
            double dblEps = nc_eps_machine();
            pPolyOut->setComplex(true);
            double* pdblInImg   = pDblIn->getImg();
            double* pdblCoefImg = pPolyOut->get(0)->getImg();
            C2F(wprxc)(iRanks, pdblInReal, pdblInImg, pdblCoefReal, pdblCoefImg);
            bool bSetCplx = false;
            for (int k = 0; k < *iRanks; k++)
            {
                if ((pdblCoefReal[k] + pdblCoefImg[k]) != pdblCoefReal[k])
                {
                    bSetCplx = true;
                    break;
                }
            }
            pPolyOut->setComplex(bSetCplx);
        }
        else
        {
            C2F(dprxc)(iRanks, pdblInReal, pdblCoefReal);
        }
        delete[] iRanks;

        if (bDeleteInput)
        {
            delete pDblIn;
        }
    }
    else // coeff
    {
        // [] case
        if (pDblIn->getSize() == 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        if (pDblIn->getRows() != 1 && pDblIn->getCols() != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A vector expected.\n"), "poly", 1);
            return types::Function::Error;
        }

        int piDimsArray[2] = {1, 1};
        int *iRanks = new int[1];
        *iRanks = pDblIn->getSize() - 1;
        pPolyOut = new types::Polynom(wstrName, 2, piDimsArray, iRanks);
        delete[] iRanks;
        pPolyOut->setComplex(pDblIn->isComplex());
        pPolyOut->setCoef(0, pDblIn);
    }

    out.push_back(pPolyOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

