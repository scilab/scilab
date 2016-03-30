/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Cedric DELAMARRE
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

#include <cmath>

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
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"
    extern int C2F(rpoly)(double*, int*, double*, double*, int*);
    extern int C2F(wscal)(int*, double*, double*, double*, double*, int*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_roots(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    std::wstring wstrAlgo   = L"e"; // e = eigen (default), f = fast
    types::Double* pDblIn   = NULL;
    types::Double* pDblOut  = NULL;
    types::Polynom* pPolyIn = NULL;

    double* pdblInReal   = NULL;
    double* pdblInImg    = NULL;
    double* pdblTempReal = NULL;
    double* pdblTempImg  = NULL;

    int iOne = 1;
    int imOne = -1;
    int iSize = 0;
    bool bComplex = false;
    types::Function::ReturnValue ret = types::Function::OK;

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "roots", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "roots", 1);
        return types::Function::Error;
    }

    // get algo type
    if (in.size() == 2)
    {
        if (in[1]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d : string expected.\n"), "roots", 2);
            return types::Function::Error;
        }

        types::String* pStrAlgo = in[1]->getAs<types::String>();
        if (pStrAlgo->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), "roots", 2);
            return types::Function::Error;
        }

        wstrAlgo = pStrAlgo->get(0);
        if (wstrAlgo != L"e" && wstrAlgo != L"f")
        {
            Scierror(999, _("%s: Wrong value for input argument #%d : ""%s"" or ""%s"" expected.\n"), "roots", 2, "e", "f");
            return types::Function::Error;
        }
    }

    if (in[0]->isDouble())
    {
        // for Matlab compatibility root of the vector of coefficients
        pDblIn = in[0]->getAs<types::Double>();
        if (pDblIn->isEmpty())
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        iSize = pDblIn->getSize();

        // old fortran function dtild
        // switch elements of a vector. [1 2 3] => [3 2 1]
        pdblInReal = new double[iSize];
        C2F(dcopy)(&iSize, pDblIn->get(), &iOne, pdblInReal, &imOne);
        if (pDblIn->isComplex())
        {
            bComplex = true;
            pdblInImg = new double[iSize];
            C2F(dcopy)(&iSize, pDblIn->getImg(), &iOne, pdblInImg, &imOne);
        }
    }
    else if (in[0]->isPoly())
    {
        pPolyIn = in[0]->getAs<types::Polynom>();

        if (pPolyIn->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d : A scalar expected.\n"), "roots", 1);
            return types::Function::Error;
        }

        iSize      = pPolyIn->getMaxRank() + 1;
        pdblInReal = pPolyIn->get(0)->get();

        if (pPolyIn->isComplex())
        {
            bComplex = true;
            pdblInImg = pPolyIn->get(0)->getImg();
        }
    }
    else
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_roots";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    // If "fast" algo was chosen and polynomial is complex,
    // then produce an error.
    if (wstrAlgo == L"f" && bComplex)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d : If algo is ""%s"", a real is expected.\n"), "roots", 2, "f");
        return types::Function::Error;
    }

    double t = 0;
    while (t == 0)
    {
        iSize--;
        if (iSize < 0)
        {
            out.push_back(types::Double::Empty());
            return types::Function::OK;
        }

        t = std::fabs(pdblInReal[iSize]);
        if (bComplex)
        {
            t += std::fabs(pdblInImg[iSize]);
        }
    }

    // If "fast" algo was chosen and polynomial has degree greater than 100,
    // then produce an error.
    if (wstrAlgo == L"f" && iSize > 100)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d : If algo is ""%s"", a degree less than %d expected.\n"), "roots", 2, "f", 100);
        return types::Function::Error;
    }

    if (wstrAlgo == L"f")
    {
        // real polynomial: rpoly algorithm
        // this alg is much more speedy, but it may happens that it gives
        // erroneous results without messages : example
        // roots(%s^31-8*%s^30+9*%s^29+0.995) should have two real roots near
        // 1.355 and 6.65 and the other ones inside a circle centered in 0
        // with radius 1

        pDblOut = new types::Double(iSize, 1, true);
        int iFail = 0;
        int iSizeP1 = iSize + 1;

        double* pdblTempReal = new double[iSize + 1];
        C2F(dcopy)(&iSizeP1, pdblInReal, &iOne, pdblTempReal, &imOne);
        C2F(rpoly)(pdblTempReal, &iSize, pDblOut->get(), pDblOut->getImg(), &iFail);
        delete[] pdblTempReal;

        if (iFail)
        {
            if (iFail == 1)
            {
                Scierror(999, _("%s: Convergence problem...\n"), "roots");
            }
            else if (iFail == 2)
            {
                Scierror(999, _("%s: Leading coefficient is zero.\n"), "roots");
            }
            else if (iFail == 3)
            {
                Scierror(999, _("%s: Too high degree (max 100).\n"), "roots");
            }

            return types::Function::Error;
        }

        out.push_back(pDblOut);
    }
    else // wstrAlgo == L"e"
    {
        // Companion matrix method
        int iSizeM1 = iSize - 1;
        int iSizeP1 = iSize + 1;
        double dblOne = 1;
        double* pdblTempReal = new double[iSize];
        double* pdblTempImg = NULL;
        double sr = pdblInReal[iSize];

        C2F(dcopy)(&iSize, pdblInReal, &iOne, pdblTempReal, &imOne);

        if (bComplex)
        {
            pdblTempImg = new double[iSize];
            C2F(dcopy)(&iSize, pdblInImg, &iOne, pdblTempImg, &imOne);

            double si = pdblInImg[iSize];
            double t = sr * sr + si * si;
            sr = -sr / t;
            si = si / t;

            C2F(wscal)(&iSize, &sr, &si, pdblTempReal, pdblTempImg, &iOne);
        }
        else
        {
            double dbl = -1 / sr;
            C2F(dscal)(&iSize, &dbl, pdblTempReal, &iOne);
        }

        pDblOut = new types::Double(iSize, iSize, bComplex);
        double* pdblOutReal = pDblOut->get();
        double* pdblOutImg = NULL;

        memset(pdblOutReal, 0x00, pDblOut->getSize() * sizeof(double));
        C2F(dset)(&iSizeM1, &dblOne, &pdblOutReal[iSize], &iSizeP1);
        C2F(dcopy)(&iSize, pdblTempReal, &iOne, pdblOutReal, &iOne);
        delete[] pdblTempReal;

        if (bComplex)
        {
            pdblOutImg = pDblOut->getImg();
            memset(pdblOutImg, 0x00, pDblOut->getSize() * sizeof(double));
            C2F(dcopy)(&iSize, pdblTempImg, &iOne, pdblOutImg, &iOne);
            delete[] pdblTempImg;
        }

        //call spec
        types::InternalType* pSpec = symbol::Context::getInstance()->get(symbol::Symbol(L"spec"));
        if (pSpec && pSpec->isFunction())
        {
            types::Function *funcSpec = pSpec->getAs<types::Function>();
            types::typed_list tlInput;
            types::optional_list tlOpt;
            tlInput.push_back(pDblOut);

            ret = funcSpec->call(tlInput, tlOpt, 1, out);
            pDblOut->killMe();
        }
        else
        {
            Scierror(999, _("%s: unable to find spec function\n"), "roots");
            return types::Function::Error;
        }
    }

    if (pDblIn)
    {
        delete[] pdblInReal;
        if (bComplex)
        {
            delete[] pdblInImg;
        }
    }

    return ret;
}
/*--------------------------------------------------------------------------*/

