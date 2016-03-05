/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Cedric DELAMARRE
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
#include "cacsd_gw.hxx"
#include "function.hxx"
#include "double.hxx"
#include "polynom.hxx"
#include "string.hxx"

extern "C"
{
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "common_structure.h"
#include "elem_common.h"

    extern void C2F(arl2)(double*, int*, double*, double*, int*, int*, double*, double*, int*, int*, int*, int*);
    extern void C2F(arl2a)(double*, int*, double*, int*, int*, int*, int*, int*, int*, double*, int*);
    extern void C2F(lq)(int*, double*, double*, double*, int*);
    extern double C2F(phi)(double*, int*, double*, int*, double*);
    extern void C2F(idegre)(double*, int*, int*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_arl2_ius(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    types::Double* pDblY        = NULL;
    types::Polynom* pPolyY      = NULL;
    types::Polynom* pPolyDen    = NULL;
    types::Double* pDblErr      = NULL;

    double* pdblY   = NULL;
    double* pdblDen = NULL;

    int iOne        = 1;
    int iVol1       = 0;
    int iN          = 0;
    bool bAll       = false;
    int iRankDen    = 0;
    double dErrl2   = 0;
    int lunit       = 6;

    C2F(arl2c).info = 0;

    if (in.size() < 3 || in.size() > 5)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d to %d expected.\n"), "arl2_ius", 3, 5);
        return types::Function::Error;
    }

    if (_iRetCount != 1 && _iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d or %d expected.\n"), "arl2_ius", 1, 3);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    // get Y
    if (in[0]->isDouble())
    {
        pDblY = in[0]->getAs<types::Double>();
        if (pDblY->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "arl2_ius", 1);
            return types::Function::Error;
        }

        iVol1 = pDblY->getSize();
        pdblY = pDblY->get();
    }
    else if (in[0]->isPoly())
    {
        pPolyY = in[0]->getAs<types::Polynom>();
        if (pPolyY->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A single polynom expected.\n"), "arl2_ius", 1);
            return types::Function::Error;
        }

        if (pPolyY->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real polynom expected.\n"), "arl2_ius", 1);
            return types::Function::Error;
        }

        types::SinglePoly* pSPCoefY = pPolyY->get(0);
        iVol1 = pSPCoefY->getSize();
        pdblY = pSPCoefY->get();
    }
    else
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A Matrix or polynom expected.\n"), "arl2_ius", 1);
        return types::Function::Error;
    }

    // get den <= useless in case "all" but it was does like that.
    if (in[1]->isPoly() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A polynom expected.\n"), "arl2_ius", 2);
        return types::Function::Error;
    }

    pPolyDen = in[1]->getAs<types::Polynom>();

    if (pPolyDen->isScalar() == false)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A single polynom expected.\n"), "arl2_ius", 2);
        return types::Function::Error;
    }

    if (pPolyDen->isComplex())
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: A real polynom expected.\n"), "arl2_ius", 2);
        return types::Function::Error;
    }

    pPolyDen->getRank(&iRankDen);
    pdblDen = pPolyDen->get(0)->get();
    C2F(idegre)(pdblDen, &iRankDen, &iRankDen);
    int iSize = iRankDen + 1;
    double dblScal = 1.0 / pdblDen[iRankDen];
    C2F(dscal)(&iSize, &dblScal, pdblDen, &iOne);

    // get n
    if (in[2]->isDouble() == false)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "arl2_ius", 3);
        return types::Function::Error;
    }

    types::Double* pDblN = in[2]->getAs<types::Double>();
    iN = (int)pDblN->get(0);
    if (iN < 1)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: More or equal to %d expected.\n"), "arl2_ius", 3, 1);
        return types::Function::Error;
    }

    if (iN < iRankDen)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: More than degree of input argument #%d expected.\n"), "arl2_ius", 3, 2);
        return types::Function::Error;
    }

    // get "all" and/or imp
    if (in.size() == 4)
    {
        if (in[3]->isString()) // get "all"
        {
            types::String* pStrAll = in[3]->getAs<types::String>();
            if (pStrAll->isScalar() == false)
            {
                Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "arl2_ius", 4);
                return types::Function::Error;
            }

            if (wcscmp(pStrAll->get(0), L"all") != 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: 'all' expected.\n"), "arl2_ius", 4);
                return types::Function::Error;
            }

            bAll = true;
        }
        else if (in[3]->isDouble()) // get imp
        {
            types::Double* pDblImp = in[3]->getAs<types::Double>();
            C2F(arl2c).info = (int)pDblImp->get(0);
            if (C2F(arl2c).info < 0)
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: Positive value expected.\n"), "arl2_ius", 4);
                return types::Function::Error;
            }
        }
        else
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar or string expected.\n"), "arl2_ius", 4);
            return types::Function::Error;
        }
    }
    else if (in.size() == 5)
    {
        // get imp
        if (in[3]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "arl2_ius", 4);
            return types::Function::Error;
        }

        types::Double* pDblImp = in[3]->getAs<types::Double>();
        C2F(arl2c).info = (int)pDblImp->get(0);
        if (C2F(arl2c).info < 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: Positive value expected.\n"), "arl2_ius", 4);
            return types::Function::Error;
        }

        // get "all"
        if (in[4]->isString() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), "arl2_ius", 5);
            return types::Function::Error;
        }

        types::String* pStrAll = in[4]->getAs<types::String>();
        if (pStrAll->isScalar() == false)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar string expected.\n"), "arl2_ius", 5);
            return types::Function::Error;
        }

        if (wcscmp(pStrAll->get(0), L"all") != 0)
        {
            Scierror(999, _("%s: Wrong value for input argument #%d: 'all' expected.\n"), "arl2_ius", 5);
            return types::Function::Error;
        }

        bAll = true;
    }

    /*** perform operations ***/
    int iNg = iVol1 - 1;
    if (bAll)
    {
        // look for "all" solutions
        int iNsol = 0;
        int iMxsol = 20;
        double* pdblDenTemp = new double[iMxsol * (iN + 1)];
        int iWorkSize = 34 + 34 * iN + 7 * iNg + iN * iNg + iN * iN * (iNg + 2) + 4 * (iN + 1) * iMxsol;
        double* pdblWork = new double[iWorkSize];
        iWorkSize = 29 + iN * iN + 4 * iN + 2 * iMxsol;
        int* piWork = new int[iWorkSize];

        C2F(arl2a)(pdblY, &iVol1, pdblDenTemp, &iMxsol, &iNsol, &iN,
                   &(C2F(arl2c).info), &(C2F(arl2c).ierr), &lunit, pdblWork, piWork);

        delete[] pdblWork;
        delete[] piWork;

        if (C2F(arl2c).ierr)
        {
            if (C2F(arl2c).ierr == 3)
            {
                Scierror(999, _("%s: Loop on two orders detected.\n"), "arl2a");
                return types::Function::Error;
            }
            else if (C2F(arl2c).ierr == 4)
            {
                Scierror(999, _("%s: Impossible to reach required order.\n"), "arl2a");
                return types::Function::Error;
            }
            else if (C2F(arl2c).ierr == 5)
            {
                Scierror(999, _("%s: Failure when looking for the intersection with domains bounds.\n"), "arl2a");
                return types::Function::Error;
            }
            else if (C2F(arl2c).ierr == 7)
            {
                Scierror(999, _("%s: Too many solutions found.\n"), "arl2a");
                return types::Function::Error;
            }
        }

        /*** retrun output arguments ***/
        // retrun denominators
        double** pdblAllCoeff = new double*[iNsol];
        int iRank = iN + 1;
        int* piRank = new int[iNsol];
        for (int i = 0; i < iNsol; i++)
        {
            piRank[i] = iRank;
        }

        types::Polynom* pPolyDenOut = new types::Polynom(pPolyDen->getVariableName(), iNsol, 1, piRank);
        for (int i = 0; i < iNsol; i++)
        {
            double* pdblDenOut = pPolyDenOut->get(i)->get();
            C2F(dcopy)(&iN, pdblDenTemp + i, &iMxsol, pdblDenOut, &iOne);
            pdblDenOut[iN] = 1;
            pdblAllCoeff[i] = pdblDenOut;
        }

        delete[] pdblDenTemp;

        out.push_back(pPolyDenOut);

        // retrun numerators
        if (_iRetCount > 1)
        {
            for (int i = 0; i < iNsol; i++)
            {
                piRank[i] = iN;
            }

            C2F(no2f).gnrm = sqrt(C2F(no2f).gnrm);

            types::Polynom* pPolyNumOut = new types::Polynom(pPolyDen->getVariableName(), iNsol, 1, piRank);
            for (int i = 0; i < iNsol; i++)
            {
                double* pdblNumOut = pPolyNumOut->get(i)->get();
                double* pdblWork = new double[iN + iNg + 1];
                C2F(lq)(&iN, pdblAllCoeff[i], pdblWork, pdblY, &iNg);
                C2F(dscal)(&iN, &(C2F(no2f).gnrm), pdblWork, &iOne);
                C2F(dcopy)(&iN, pdblWork, &iOne, pdblNumOut, &iOne);
                delete[] pdblWork;
            }

            out.push_back(pPolyNumOut);
        }

        // return error
        if (_iRetCount > 2)
        {
            pDblErr = new types::Double(iNsol, 1);
            double* pdblerr = pDblErr->get();
            double* pdblWork = new double[iN + iNg + 1];
            for (int i = 0; i < iNsol; i++)
            {
                pdblerr[i] = sqrt(C2F(phi)(pdblAllCoeff[i], &iN, pdblY, &iNg, pdblWork)) * C2F(no2f).gnrm;
            }

            delete[] pdblWork;

            out.push_back(pDblErr);
        }

        delete[] piRank;
        delete[] pdblAllCoeff;
    }
    else
    {
        // look for a solution
        int iSizeNum = std::max(iN, iRankDen);
        double* pdblNum = new double[iSizeNum];
        int iWorkSize = 32 + 32 * iN + 7 * iNg + iN * iNg + iN * iN * (iNg + 2);
        double* pdblWork = new double[iWorkSize];
        iWorkSize = 29 + iN * iN + 4 * iN;
        int* piWork = new int[iWorkSize];

        int iSizeTemp = std::max(iRankDen, iN) + 1;
        double* pDblDenTemp = new double[iSizeTemp];
        memset(pDblDenTemp, 0x00, iSizeTemp * sizeof(double));
        C2F(dcopy)(&iSize, pdblDen, &iOne, pDblDenTemp, &iOne);

        C2F(arl2)(pdblY, &iVol1, pdblNum, pDblDenTemp, &iRankDen, &iN, &dErrl2, pdblWork, piWork,
                  &(C2F(arl2c).info), &(C2F(arl2c).ierr), &lunit);


        delete[] pdblWork;
        delete[] piWork;

        if (C2F(arl2c).ierr != 0)
        {
            if (C2F(arl2c).ierr == 3)
            {
                sciprint(_("%s: Loop on two orders detected.\n"), "arl2");
            }
            else if (C2F(arl2c).ierr == 4)
            {
                sciprint(_("%s: Impossible to reach required order.\n   previous order computed solution returned.\n"), "arl2");
            }
            else if (C2F(arl2c).ierr == 5)
            {
                sciprint(_("%s: Failure when looking for the intersection with domains boundaries.\n   previous order computed solution returned.\n"), "arl2");
            }
            else if (C2F(arl2c).ierr == 7)
            {
                Scierror(999, _("%s: too many solutions found\n"), "arl2");
                delete[] pdblNum;
                delete[] pDblDenTemp;
                return types::Function::Error;
            }
        }

        /*** retrun output arguments ***/
        // retrun denominator
        int iRank = iN + 1;
        types::Polynom* pPolyDenOut = new types::Polynom(pPolyDen->getVariableName(), 1, 1, &iRank);
        double* pdblDenOut = pPolyDenOut->get(0)->get();
        C2F(dcopy)(&iRank, pDblDenTemp, &iOne, pdblDenOut, &iOne);
        out.push_back(pPolyDenOut);

        // retrun numerator
        if (_iRetCount > 1)
        {
            types::Polynom* pPolyNumOut = new types::Polynom(pPolyDen->getVariableName(), 1, 1, &iN);
            double* pdblNumOut = pPolyNumOut->get(0)->get();
            C2F(dcopy)(&iN, pdblNum, &iOne, pdblNumOut, &iOne);
            out.push_back(pPolyNumOut);
        }

        // return error
        if (_iRetCount > 2)
        {
            out.push_back(new types::Double(dErrl2));
        }

        delete[] pDblDenTemp;
        delete[] pdblNum;
    }

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
