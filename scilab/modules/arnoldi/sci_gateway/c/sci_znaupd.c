/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
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

#include <math.h>
#include <string.h>

#include "doublecomplex.h"
#include "api_scilab.h"
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
int sci_znaupd(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrpIDO     = NULL;
    int* pIDO           = NULL;
    int* piAddrpBMAT    = NULL;
    char* pBMAT         = NULL;
    int* piAddrpN       = NULL;
    int* pN             = NULL;
    int* piAddrpWHICH   = NULL;
    char* pWHICH        = NULL;
    int* piAddrpNEV     = NULL;
    int* pNEV           = NULL;
    int* piAddrpTOL     = NULL;
    double* pTOL        = NULL;
    int* piAddrpNCV     = NULL;
    int* pNCV           = NULL;
    int* piAddrpIPARAM  = NULL;
    int* pIPARAM        = NULL;
    int* piAddrpIPNTR   = NULL;
    int* pIPNTR         = NULL;
    int* piAddrpRWORK   = NULL;
    double* pRWORK      = NULL;
    int* piAddrpINFO    = NULL;
    int* pINFO          = NULL;
    int* piAddrpV       = NULL;

    doublecomplex* pV       = NULL;
    int* piAddrpRESID       = NULL;
    doublecomplex* pRESID   = NULL;
    int* piAddrpWORKD       = NULL;
    doublecomplex* pWORKD   = NULL;
    int* piAddrpWORKL       = NULL;
    doublecomplex* pWORKL   = NULL;

    int IDO,   mIDO,   nIDO;
    int mN,     nN;
    int mNEV,   nNEV;
    int mTOL,   nTOL;
    int RESID, mRESID, nRESID;
    int mNCV,   nNCV;
    int V,     mV,     nV;
    int IPARAM, mIPARAM, nIPARAM;
    int IPNTR, mIPNTR, nIPNTR;
    int WORKD, mWORKD, nWORKD;
    int WORKL, mWORKL, nWORKL;
    int RWORK, mRWORK, nRWORK;
    int INFO,  mINFO,  nINFO;

    int minlhs = 1, minrhs = 15, maxlhs = 9, maxrhs = 15;
    int LDV, LWORKL;
    int sizeWORKL = 0;

    CheckInputArgument(pvApiCtx, minrhs, maxrhs);
    CheckOutputArgument(pvApiCtx, minlhs, maxlhs);

    /*                                                  VARIABLE = NUMBER   */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrpIDO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpIDO, &mIDO, &nIDO, &pIDO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    IDO =  1;

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrpN);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpN, &mN, &nN, &pN);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrpNEV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpNEV, &mNEV, &nNEV, &pNEV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddrpTOL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 6.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpTOL, &mTOL, &nTOL, &pTOL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 6);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddrpRESID);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 7.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpRESID, &mRESID, &nRESID, &pRESID);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 7);
        return 1;
    }

    RESID =  7;
    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddrpNCV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 8.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpNCV, &mNCV, &nNCV, &pNCV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 8);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddrpV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 9.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpV, &mV, &nV, &pV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 9);
        return 1;
    }

    V =  9;
    sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddrpIPARAM);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 10.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpIPARAM, &mIPARAM, &nIPARAM, &pIPARAM);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 10);
        return 1;
    }

    IPARAM = 10;
    sciErr = getVarAddressFromPosition(pvApiCtx, 11, &piAddrpIPNTR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 11.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpIPNTR, &mIPNTR, &nIPNTR, &pIPNTR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 11);
        return 1;
    }

    IPNTR = 11;

    sciErr = getVarAddressFromPosition(pvApiCtx, 12, &piAddrpWORKD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 12.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpWORKD, &mWORKD, &nWORKD, &pWORKD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 12);
        return 1;
    }

    WORKD = 12;

    sciErr = getVarAddressFromPosition(pvApiCtx, 13, &piAddrpWORKL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 13.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpWORKL, &mWORKL, &nWORKL, &pWORKL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 13);
        return 1;
    }

    WORKL = 13;
    sciErr = getVarAddressFromPosition(pvApiCtx, 14, &piAddrpRWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 14.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpRWORK, &mRWORK, &nRWORK, &pRWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 14);
        return 1;
    }

    RWORK = 14;
    sciErr = getVarAddressFromPosition(pvApiCtx, 15, &piAddrpINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 15.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpINFO, &mINFO, &nINFO, &pINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 15);
        return 1;
    }

    INFO = 15;

    LWORKL = mWORKL * nWORKL;
    LDV = Max(1, pN[0]);

    /* Don't call dnaupd if ido == 99 */
    if (pIDO[0] == 99)
    {
        Scierror(999, _("%s: the computation is already terminated\n"), fname);
        return 0;
    }

    /* Check some sizes */
    if (mIPARAM * nIPARAM != 11)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "IPARAM", 11);
        return 0;
    }

    if (mIPNTR * nIPNTR != 14)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "IPNTR", 14);
        return 0;
    }

    if (mRESID * nRESID != pN[0])
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "RESID", pN[0]);
        return 0;
    }

    if ((mV != pN[0]) || (nV != pNCV[0]))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "V", pN[0], pNCV[0]);
        return 0;
    }

    if (mWORKD * nWORKD < 3 * pN[0])
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKD", 3 * pN[0]);
        return 0;
    }

    sizeWORKL = 3 * pNCV[0] * pNCV[0] + 5 * pNCV[0];

    if (mWORKL * nWORKL < sizeWORKL)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKL", sizeWORKL);
        return 0;
    }


    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrpBMAT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    if (getAllocatedSingleString(pvApiCtx, piAddrpBMAT, &pBMAT))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrpWHICH);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(pBMAT);
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    if (getAllocatedSingleString(pvApiCtx, piAddrpWHICH, &pWHICH))
    {
        freeAllocatedSingleString(pBMAT);
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 4);
        return 1;
    }

    C2F(znaupd)(pIDO, pBMAT, pN,
                pWHICH, pNEV, pTOL,
                pRESID, pNCV, pV, &LDV,
                pIPARAM, pIPNTR, pWORKD,
                pWORKL, &LWORKL, pRWORK, pINFO);

    freeAllocatedSingleString(pBMAT);
    freeAllocatedSingleString(pWHICH);

    if (pINFO[0] < 0)
    {
        Scierror(998, _("%s: internal error, info=%d.\n"), fname, *pINFO);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = IDO;
    AssignOutputVariable(pvApiCtx, 2) = RESID;
    AssignOutputVariable(pvApiCtx, 3) = V;
    AssignOutputVariable(pvApiCtx, 4) = IPARAM;
    AssignOutputVariable(pvApiCtx, 5) = IPNTR;
    AssignOutputVariable(pvApiCtx, 6) = WORKD;
    AssignOutputVariable(pvApiCtx, 7) = WORKL;
    AssignOutputVariable(pvApiCtx, 8) = RWORK;
    AssignOutputVariable(pvApiCtx, 9) = INFO;

    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
