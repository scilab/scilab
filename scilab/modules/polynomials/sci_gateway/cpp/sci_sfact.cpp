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
        int n       = 1 + iDegD2;

        if (2 * iDegD2 != iDegree)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: A symmetric polynomial expected.\n"), "sfact", 1);
            return types::Function::Error;
        }

        for (int i = 0; i < n; i++)
        {
            if (pdblCoef[i] != pdblCoef[iDegree - i])
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A symmetric polynomial expected.\n"), "sfact", 1);
                return types::Function::Error;
            }
        }

        // create result
        pPolyOut = new types::Polynom(pPolyIn->getVariableName(), 1, 1);
        double* pdblCoefOut = NULL;
        types::SinglePoly* pSP = new types::SinglePoly(&pdblCoefOut, iDegD2);
        C2F(dcopy)(&n, pdblCoef, &iOne, pdblCoefOut, &iOne);

        // perform operation
        double* pdblWork = new double[7 * n];
        C2F(sfact1)(pdblCoefOut, &iDegD2, pdblWork, &iMaxIt, &iErr);
        delete[] pdblWork;
        if (iErr == 2)
        {
            delete pSP;
            delete pPolyOut;
            Scierror(999, _("%s: Wrong value for input argument #%d: Non negative or null value expected at degree %d.\n"), "sfact", 1, n);
            return types::Function::Error;
        }
        else if (iErr == 1)
        {
            delete pSP;
            delete pPolyOut;
            Scierror(999, _("%s: Wrong value for input argument #%d: Convergence problem.\n"), "sfact", 1);
            return types::Function::Error;
        }
        else if (iErr < 0)
        {
            sciprint("%s: warning: Convergence at 10^%d near.\n", "sfact", iErr);
        }

        // return result
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
        int n       = 1 + iDegD2;

        double* pdblOut  = new double[iSize * n];
        double* pdblWork = new double[(n + 1) * iRows * ((n + 1)*iRows) + 1];

        memset(pdblOut, 0x00, iSize * n * sizeof(double));

        for (int i = 0; i < iSize; i++)
        {
            double* pdblIn = pPolyIn->get(i)->get();
            int iSizeToCpy = 2 + pPolyIn->get(i)->getSize() - 1 - n;
            if (iSizeToCpy > 0)
            {
                C2F(dcopy)(&iSizeToCpy, pdblIn + n - 1, &iOne, pdblOut + i, &iSize);
            }
        }

        int nm1 = n - 1;
        iMaxIt += n;
        C2F(sfact2)(pdblOut, &iRows, &nm1, pdblWork, &iMaxIt, &iErr);
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
            types::SinglePoly* pSP = new types::SinglePoly(&pdblCoefOut, nm1);
            C2F(dcopy)(&n, pdblOut + i, &iSize, pdblCoefOut, &iOne);
            pPolyOut->set(i, pSP);
            delete pSP;
        }

        delete[] pdblOut;
    }

    out.push_back(pPolyOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

