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

#include <algorithm>

#include "polynomials_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "overload.hxx"
#include "configvariable.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
    extern int C2F(dpsimp)(double*, int*, double*, int*, double*, int*, double*, int*, double*, int*);
}
/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_simp(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::InternalType* pNumOut = NULL;
    types::InternalType* pDenOut = NULL;

    bool bComplex   = false;
    int iDouble     = 0;
    int iSize       = 0;
    int iMaxDegrNum = 0;
    int iMaxDegrDen = 0;
    int iErr        = 0;

    std::wstring wstrName = L"";

    if (in.size() < 1 || in.size() > 2)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "simp", 1, 2);
        return types::Function::Error;
    }

    if (_iRetCount > 2)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d to %d expected.\n"), "simp", 1, 2);
        return types::Function::Error;
    }

    if (ConfigVariable::getSimpMode() == 0)
    {
        std::copy(in.begin(), in.end(), std::back_inserter(out));
        return types::Function::OK;
    }

    if (in.size() == 1)
    {
        // rational case
        std::wstring wstFuncName = L"%r_simp";
        return Overload::call(wstFuncName, in, _iRetCount, out);
    }
    else // simp(num, den)
    {
        if (_iRetCount != 2)
        {
            Scierror(78, _("%s: Wrong number of output argument(s): %d expected.\n"), "simp", 2);
            return types::Function::Error;
        }

        for (int i = 0; i < in.size(); i++)
        {
            if (in[i]->isPoly() == false && in[i]->isDouble() == false)
            {
                Scierror(999, _("%s: Wrong type for input argument #%d: A polynomial expected.\n"), "simp", i + 1);
                return types::Function::Error;
            }

            types::GenericType* pGT = in[i]->getAs<types::GenericType>();
            bComplex |= pGT->isComplex();
            iDouble  += in[i]->isDouble() ? i + 1 : 0;

            if (i == 1 && pGT->getSize() != iSize)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: Same size expected.\n"), "simp", 2);
                return types::Function::Error;
            }

            iSize = pGT->getSize();
        }

        if (bComplex)
        {
            return Overload::call(L"%p_simp", in, _iRetCount, out);
        }

        if (iDouble == 3) // simp(double, double)
        {
            return Overload::call(L"%s_simp", in, _iRetCount, out);
        }

        switch (iDouble)
        {
            case 0 : // sim(poly, poly)
            {
                types::Polynom* pNum = in[0]->clone()->getAs<types::Polynom>();
                types::Polynom* pDen = in[1]->clone()->getAs<types::Polynom>();

                wstrName = pNum->getVariableName();

                if (wstrName != pDen->getVariableName())
                {
                    Scierror(999, _("%s: Wrong value for input argument #%d: A polynomial '%ls' expected.\n"), "simp", 2, wstrName.c_str());
                    return types::Function::Error;
                }

                types::Polynom* pPolyNumOut = new types::Polynom(wstrName, pNum->getDims(), pNum->getDimsArray());
                types::Polynom* pPolyDenOut = new types::Polynom(wstrName, pNum->getDims(), pNum->getDimsArray());

                iMaxDegrNum = pNum->getMaxRank();
                iMaxDegrDen = pDen->getMaxRank();

                int iMax = std::max(iMaxDegrNum, iMaxDegrDen) + 1;
                int iSizeWork = 2 * (iMaxDegrNum + iMaxDegrDen) +
                                std::min(iMaxDegrNum, iMaxDegrDen) +
                                10 * iMax + 3 * iMax * iMax + 4;
                double* pdblWork = new double[iSizeWork];

                for (int i = 0; i < iSize; i++)
                {
                    double* pdblNum = pNum->get(i)->get();
                    double* pdblDen = pDen->get(i)->get();
                    int iDegreeNum  = pNum->get(i)->getRank();
                    int iDegreeDen  = pDen->get(i)->getRank();

                    double* pdblNumOut = NULL;
                    double* pdblDenOut = NULL;

                    double* pdblNumTmp = new double[iDegreeNum + 1];
                    double* pdblDenTmp = new double[iDegreeDen + 1];

                    int iRankNumOut = 0;
                    int iRankDenOut = 0;

                    iErr = iSizeWork;

                    C2F(dpsimp)(pdblNum, &iDegreeNum, pdblDen, &iDegreeDen,
                                pdblNumTmp, &iRankNumOut, pdblDenTmp, &iRankDenOut,
                                pdblWork, &iErr);

                    if (iErr)
                    {
                        delete[] pdblNumTmp;
                        delete[] pdblDenTmp;
                        break;
                    }

                    types::SinglePoly* pSPNum = new types::SinglePoly(&pdblNumOut, iRankNumOut - 1);
                    types::SinglePoly* pSPDen = new types::SinglePoly(&pdblDenOut, iRankDenOut - 1);

                    memcpy(pdblNumOut, pdblNumTmp, iRankNumOut * sizeof(double));
                    memcpy(pdblDenOut, pdblDenTmp, iRankDenOut * sizeof(double));

                    pPolyNumOut->set(i, pSPNum);
                    pPolyDenOut->set(i, pSPDen);

                    delete[] pdblNumTmp;
                    delete[] pdblDenTmp;
                    delete pSPNum;
                    delete pSPDen;
                }

                delete[] pdblWork;
                delete pNum;
                delete pDen;

                pNumOut = pPolyNumOut;
                pDenOut = pPolyDenOut;
                break;
            }
            case 1 : // sim(double, poly)
            case 2 : // sim(poly, double)
            {
                pNumOut = in[0];
                pDenOut = in[1];
                break;
            }
        }
    }

    if (iErr)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A non null denominator expected.\n"), "simp", 2);
        return types::Function::Error;
    }

    out.push_back(pNumOut);
    out.push_back(pDenOut);
    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/

