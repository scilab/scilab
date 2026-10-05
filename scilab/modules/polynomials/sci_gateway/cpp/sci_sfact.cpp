/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
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
#include "polynomials_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "overload.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "Sciwarning.h"
#include "elem_common.h"
    extern int C2F(sfact1)(double*, int*, double*, int*, int*);
    extern int C2F(sfact2)(double*, int*, int*, double*, int*, int*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_sfact(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Polynom* pPolyIn  = NULL;
    types::Polynom* pPolyOut = NULL;

    int iMaxIt = 100;
    int iErr   = 0;
    int iOne   = 1;

    if (in.size() != 1)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "sfact", 1);
        return types::Function::Error;
    }

    if (_iRetCount > 1)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "sfact", 1);
        return types::Function::Error;
    }

    if (in[0]->isPoly() == false)
    {
        std::wstring wstFuncName = L"%" + in[0]->getShortTypeStr() + L"_sfact";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }

    pPolyIn = in[0]->getAs<types::Polynom>();

    if (pPolyIn->isComplex())
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A real matrix expected.\n"), "sfact", 1);
        return types::Function::Error;
    }

    if (pPolyIn->isScalar())
    {
        double* pdblCoef = pPolyIn->get(0)->get();

        // check symmetry
        int iDegree = pPolyIn->get(0)->getRank();
        int iDegD2  = (int)(iDegree / 2);
        int iSizeD2 = 1 + iDegD2;

        if (2 * iDegD2 != iDegree)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Maximum degree must be even.\n"), "sfact", 1);
            return types::Function::Error;
        }

        for (int i = 0; i < iDegD2; i++)
        {
            if (pdblCoef[i] != pdblCoef[iDegree - i])
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A palindromic polynomial is expected.\n"), "sfact", 1);
                return types::Function::Error;
            }
        }

        // create result
        double* pdblCoefOut = NULL;
        types::SinglePoly* pSP = new types::SinglePoly(&pdblCoefOut, iDegD2);
        C2F(dcopy)(&iSizeD2, pdblCoef, &iOne, pdblCoefOut, &iOne);

        // perform operation
        double* pdblWork = new double[7 * iSizeD2];
        C2F(sfact1)(pdblCoefOut, &iDegD2, pdblWork, &iMaxIt, &iErr);
        delete[] pdblWork;
        if (iErr == 2)
        {
            delete pSP;
            Scierror(999, _("%s: Wrong value for input argument #%d: Non negative or null value expected at degree %d.\n"), "sfact", 1, iDegD2);
            return types::Function::Error;
        }
        else if (iErr == 1)
        {
            delete pSP;
            Scierror(999, _("%s: Wrong value for input argument #%d: Convergence problem.\n"), "sfact", 1);
            return types::Function::Error;
        }
        else if (iErr < 0)
        {
            Sciwarning("%s: warning: Convergence at 10^%d near.\n", "sfact", iErr);
        }

        // return result
        pPolyOut = new types::Polynom(pPolyIn->getVariableName(), 1, 1);
        pPolyOut->set(0, pSP);
        delete pSP;
    }
    else
    {
        if (pPolyIn->getRows() != pPolyIn->getCols())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Square matrix expected.\n"), "sfact", 1);
            return types::Function::Error;
        }

        int iSize   = pPolyIn->getSize();
        int iRows   = pPolyIn->getRows();
        int iDegree = pPolyIn->getMaxRank();
        int iDegD2  = (int)(iDegree / 2);
        int iSizeD2 = 1 + iDegD2;

        if (2 * iDegD2 != iDegree)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Maximum degree must be even.\n"), "sfact", 1);
            return types::Function::Error;
        }

        double* pdblOut  = new double[iSize * iSizeD2];
        int q = iSizeD2 * iRows;
        double* pdblWork = new double[q * (q + 1) / 2];

        memset(pdblOut, 0x00, iSize * iSizeD2 * sizeof(double));

        for (int i = 0; i < iSize; i++)
        {
            // Copy all coeff from the middle to the end of the current polynom.
            // The middle is computed using the max degree of all polynoms.
            // ie: with max degree equal to 6, copy all coeff from deg 3 to the current poly coeff.
            //     for polynoms with a degree < 6, missing coeff are considered as zero. 
            //     s^2 + s^3 + s^4 considered as 0 + 0*s + s^2 + s^3 + s^4 + 0*s^5 + 0*s^6 by coping only the coeff of s^4.
            //     pdblOut will contains degree 3 coeff of all polynoms, following by degree 4 coeff of all polynoms, ect...
            double* pdblIn = pPolyIn->get(i)->get();
            int iSizeToCpy = pPolyIn->get(i)->getSize() - iDegD2;
            if (iSizeToCpy > 0)
            {
                C2F(dcopy)(&iSizeToCpy, pdblIn + iSizeD2 - 1, &iOne, pdblOut + i, &iSize);
            }
        }

        iMaxIt += iSizeD2;
        C2F(sfact2)(pdblOut, &iRows, &iDegD2, pdblWork, &iMaxIt, &iErr);
        delete[] pdblWork;

        if (iErr < 0)
        {
            delete[] pdblOut;
            Scierror(999, _("%s: Wrong value for input argument #%d: Convergence problem.\n"), "sfact", 1);
            return types::Function::Error;
        }
        else if (iErr > 0)
        {
            delete[] pdblOut;
            Scierror(999, _("%s: Wrong value for input argument #%d: singular or asymmetric problem.\n"), "sfact", 1);
            return types::Function::Error;
        }

        pPolyOut = new types::Polynom(pPolyIn->getVariableName(), pPolyIn->getDims(), pPolyIn->getDimsArray());
        for (int i = 0; i < iSize; i++)
        {
            double* pdblCoefOut = NULL;
            types::SinglePoly* pSP = new types::SinglePoly(&pdblCoefOut, iDegD2);
            C2F(dcopy)(&iSizeD2, pdblOut + i, &iSize, pdblCoefOut, &iOne);
            pPolyOut->set(i, pSP);
            delete pSP;
        }

        delete[] pdblOut;
    }

    out.push_back(pPolyOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

