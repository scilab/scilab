/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <math.h>
#include <string.h>
#include "stack-c.h"
#include "core_math.h"
#include "gw_arnoldi.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(znaupd)(int * ido, char * bmat, int * n, char * which,
                       int * nev, double * tol, doublecomplex * resid,
                       int * ncv, doublecomplex * v, int * ldv, int * iparam,
                       int * ipntr, doublecomplex * workd,
                       doublecomplex * workl, int * lworkl, double * rwork,
                       int * info);
/*--------------------------------------------------------------------------*/
int sci_znaupd(char *fname, unsigned long fname_len)
{
    int IDO,   mIDO,   nIDO,    pIDO;
    int mBMAT,  nBMAT,   pBMAT;
    int mN,     nN,      pN;
    int mWHICH, nWHICH,  pWHICH;
    int mNEV,   nNEV,    pNEV;
    int mTOL,   nTOL,    pTOL;
    int RESID, mRESID, nRESID,  pRESID;
    int mNCV,   nNCV,    pNCV;
    int V,     mV,     nV,      pV;
    int IPARAM, mIPARAM, nIPARAM, pIPARAM;
    int IPNTR, mIPNTR, nIPNTR,  pIPNTR;
    int WORKD, mWORKD, nWORKD,  pWORKD;
    int WORKL, mWORKL, nWORKL,  pWORKL;
    int RWORK, mRWORK, nRWORK,  pRWORK;
    int INFO,  mINFO,  nINFO,   pINFO;

    int minlhs = 1, minrhs = 15, maxlhs = 9, maxrhs = 15;
    int LDV, LWORKL;
    int sizeWORKL = 0;

    CheckRhs(minrhs, maxrhs);
    CheckLhs(minlhs, maxlhs);

    /*                                                  VARIABLE = NUMBER   */
    GetRhsVar( 1, MATRIX_OF_INTEGER_DATATYPE, &mIDO,    &nIDO,    &pIDO);
    IDO =  1;
    GetRhsVar( 2, STRING_DATATYPE,            &mBMAT,   &nBMAT,   &pBMAT);
    GetRhsVar( 3, MATRIX_OF_INTEGER_DATATYPE, &mN,      &nN,      &pN);
    GetRhsVar( 4, STRING_DATATYPE,            &mWHICH,  &nWHICH,  &pWHICH);
    GetRhsVar( 5, MATRIX_OF_INTEGER_DATATYPE, &mNEV,    &nNEV,    &pNEV);
    GetRhsVar( 6, MATRIX_OF_DOUBLE_DATATYPE,  &mTOL,    &nTOL,    &pTOL);
    GetRhsVar( 7, MATRIX_OF_COMPLEX_DATATYPE, &mRESID,  &nRESID,  &pRESID);
    RESID =  7;
    GetRhsVar( 8, MATRIX_OF_INTEGER_DATATYPE, &mNCV,    &nNCV,    &pNCV);
    GetRhsVar( 9, MATRIX_OF_COMPLEX_DATATYPE, &mV,      &nV,      &pV);
    V =  9;
    GetRhsVar(10, MATRIX_OF_INTEGER_DATATYPE, &mIPARAM, &nIPARAM, &pIPARAM);
    IPARAM = 10;
    GetRhsVar(11, MATRIX_OF_INTEGER_DATATYPE, &mIPNTR,  &nIPNTR,  &pIPNTR);
    IPNTR = 11;
    GetRhsVar(12, MATRIX_OF_COMPLEX_DATATYPE, &mWORKD,  &nWORKD,  &pWORKD);
    WORKD = 12;
    GetRhsVar(13, MATRIX_OF_COMPLEX_DATATYPE, &mWORKL,  &nWORKL,  &pWORKL);
    WORKL = 13;
    GetRhsVar(14, MATRIX_OF_DOUBLE_DATATYPE,  &mRWORK,  &nRWORK,  &pRWORK);
    RWORK = 14;
    GetRhsVar(15, MATRIX_OF_INTEGER_DATATYPE, &mINFO,   &nINFO,   &pINFO);
    INFO = 15;

    LWORKL = mWORKL * nWORKL;
    LDV = Max(1, *istk(pN));

    /* Don't call dnaupd if ido == 99 */
    if (*istk(pIDO) == 99)
    {
        Scierror(999, _("%s: the computation is already terminated\n"), fname);
        return 0;
    }

    /* Check some sizes */
    if (mIPARAM*nIPARAM != 11)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "IPARAM", 11);
        return 0;
    }

    if (mIPNTR*nIPNTR != 14)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "IPNTR", 14);
        return 0;
    }

    if (mRESID*nRESID != *istk(pN))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "RESID", *istk(pN));
        return 0;
    }

    if ((mV != *istk(pN)) || (nV != *istk(pNCV)))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "V", *istk(pN), *istk(pNCV));
        return 0;
    }

    if (mWORKD * nWORKD < 3 * *istk(pN))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKD", 3 * *istk(pN));
        return 0;
    }

    sizeWORKL = 3 * *istk(pNCV) * *istk(pNCV) + 5 * *istk(pNCV);

    if (mWORKL * nWORKL < sizeWORKL)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKL", sizeWORKL);
        return 0;
    }

    C2F(znaupd)(istk(pIDO), cstk(pBMAT), istk(pN),
                cstk(pWHICH), istk(pNEV), stk(pTOL),
                zstk(pRESID), istk(pNCV), zstk(pV), &LDV,
                istk(pIPARAM), istk(pIPNTR), zstk(pWORKD),
                zstk(pWORKL), &LWORKL, stk(pRWORK), istk(pINFO));

    if (*istk(pINFO) < 0)
    {
        C2F(errorinfo)("znaupd", istk(pINFO), 6L);
        return 0;
    }

    LhsVar(1) = IDO;
    LhsVar(2) = RESID;
    LhsVar(3) = V;
    LhsVar(4) = IPARAM;
    LhsVar(5) = IPNTR;
    LhsVar(6) = WORKD;
    LhsVar(7) = WORKL;
    LhsVar(8) = RWORK;
    LhsVar(9) = INFO;

    PutLhsVar();

    return 0;
}
/*--------------------------------------------------------------------------*/
