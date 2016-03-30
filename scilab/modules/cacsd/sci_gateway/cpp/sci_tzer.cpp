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
#include "numericconstants.hxx"

extern "C"
{
#include "Scierror.h"
#include "localization.h"
#include "elem_common.h"

    extern void C2F(sszer)( int*, int*, int*, double*, int*, double*, double*, int*, double*,
                            double*, double*, double*, int*, int*, double*, int*, double*, int*,
                            double*, double*, int*, double*, int*, int*);
}

/*--------------------------------------------------------------------------*/
types::Function::ReturnValue sci_tzer(types::typed_list &in, int _iRetCount, types::typed_list &out)
{
    int iRows[4] = {0, 0, 0, 0};
    int iCols[4] = {0, 0, 0, 0};
    double* pdblIn[4] = {NULL, NULL, NULL, NULL};

    double dblEps = NumericConstants::eps_machine;
    int iOne = 1;

    if (in.size() != 4)
    {
        Scierror(77, _("%s: Wrong number of input argument(s): %d expected.\n"), "tzer", 4);
        return types::Function::Error;
    }

    if (_iRetCount < 2 || _iRetCount > 3)
    {
        Scierror(78, _("%s: Wrong number of output argument(s): %d or %d expected.\n"), "tzer", 2, 3);
        return types::Function::Error;
    }

    /*** get inputs arguments ***/
    for (int i = 3; i >= 0; i--)
    {
        if (in[i]->isDouble() == false)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A matrix expected.\n"), "tzer", i + 1);
            return types::Function::Error;
        }

        types::Double* pDblIn = in[i]->getAs<types::Double>();

        if (pDblIn->isComplex())
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: A real matrix expected.\n"), "tzer", i + 1);
            return types::Function::Error;
        }

        if (pDblIn->isEmpty())
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A not empty matrix expected.\n"), "tzer", i + 1);
            return types::Function::Error;
        }

        iRows[i]    = pDblIn->getRows();
        iCols[i]    = pDblIn->getCols();
        pdblIn[i]   = pDblIn->get();
    }

    if (iRows[1] != iCols[0] || iCols[2] != iCols[0] || iCols[1] != iCols[3] || iRows[2] != iRows[3])
    {
        Scierror(999, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "tzer");
        return types::Function::Error;
    }

    /*** perform operations ***/
    // input
    int iSize1  = iCols[0] + iRows[2];
    int iSize2  = iCols[0] + iCols[1];
    int iNwrk1  = std::max(iCols[1], iRows[2]);
    int iNwrk2  = std::max(iNwrk1, iCols[0] + 1);

    // output
    int iNu     = 0;
    int iRank   = 0;
    int iErr    = 0;

    double* pdblR   = new double[iCols[0]];
    double* pdblI   = new double[iCols[0]];
    double* pdblAF  = new double[iSize1 * iSize2];
    double* pdblBF  = new double[iSize1 * iSize2];

    // workspace
    double* pdblWorkA  = new double[iCols[0] * iCols[0]];
    double* pdblWork1  = new double[iNwrk1];
    double* pdblWork2  = new double[iNwrk2];

    C2F(sszer)(iCols, iCols + 1, iRows + 2, pdblIn[0], iCols, pdblIn[1], pdblIn[2], iRows + 2, pdblIn[3],
               &dblEps, pdblR, pdblI, &iNu, &iRank, pdblAF, &iSize1, pdblBF, &iSize2,
               pdblWorkA, pdblWork1 , &iNwrk1, pdblWork2, &iNwrk2, &iErr);

    delete[] pdblAF;
    delete[] pdblBF;
    delete[] pdblWorkA;
    delete[] pdblWork1;

    if (iErr > 0)
    {
        if (iErr == 1)
        {
            Scierror(999, _("%s: Wrong size for argument: Incompatible dimensions.\n"), "tzer");
        }
        else if (iErr == 2)
        {
            Scierror(999, _("%s: attempt to divide by zero.\n"), "tzer");
        }
        else
        {
            Scierror(999, _("%s: ierr %d from qitz (eispack).\n"), "tzer", iErr);
        }

        delete[] pdblR;
        delete[] pdblI;
        delete[] pdblWork2;

        return types::Function::Error;
    }

    if (iRank == 0 && iNu > 0)
    {
        memset(pdblR, 0x00, iNu * sizeof(double));
        memset(pdblI, 0x00, iNu * sizeof(double));
        memset(pdblWork2, 0x00, iNu * sizeof(double));
    }

    /*** retrun output arguments ***/
    types::Double* pDblOut1 = new types::Double(iNu, std::min(iNu, 1), true);
    double* pdblReal = pDblOut1->get();
    double* pdblImg  = pDblOut1->getImg();
    C2F(dcopy)(&iNu, pdblR, &iOne, pdblReal, &iOne);
    C2F(dcopy)(&iNu, pdblI, &iOne, pdblImg, &iOne);
    out.push_back(pDblOut1);

    types::Double* pDblOut2 = new types::Double(iNu, std::min(iNu, 1));
    pdblReal = pDblOut2->get();
    C2F(dcopy)(&iNu, pdblWork2, &iOne, pdblReal, &iOne);
    out.push_back(pDblOut2);

    if (_iRetCount == 3)
    {
        out.push_back(new types::Double((double)iRank));
    }

    delete[] pdblR;
    delete[] pdblI;
    delete[] pdblWork2;

    return types::Function::OK;
}
/*--------------------------------------------------------------------------*/
