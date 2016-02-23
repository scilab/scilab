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
#include "api_scilab.h"
#include "core_math.h"
#include "gw_arnoldi.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(dnaupd)(int *ido, char *bmat, int *n, char *which, int *nev,
                       double *tol, double *resid, int *ncv, double *v,
                       int *ldv, int *iparam, int *ipntr, double *workd,
                       double *workl, int *lworkl, int *info,
                       unsigned long bmat_len, unsigned long which_len);
/*--------------------------------------------------------------------------*/
int sci_dnaupd(char *fname, void *pvApiCtx)
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
    int* piAddrpRESID   = NULL;
    double* pRESID      = NULL;
    int* piAddrpNCV     = NULL;
    int* pNCV           = NULL;
    int* piAddrpV       = NULL;
    double* pV          = NULL;
    int* piAddrpIPARAM  = NULL;
    int* pIPARAM        = NULL;
    int* piAddrpIPNTR   = NULL;
    int* pIPNTR         = NULL;
    int* piAddrpWORKD   = NULL;
    double* pWORKD      = NULL;
    int* piAddrpWORKL   = NULL;
    double* pWORKL      = NULL;
    int* piAddrpINFO    = NULL;
    int* pINFO          = NULL;

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
    int INFO,  mINFO,  nINFO;

    int minlhs = 1, minrhs = 14, maxlhs = 8, maxrhs = 14;
    int LDV, LWORKL;
    int sizeWORKL = 0;

    /* [IDO,RESID,V,IPARAM,IPNTR,WORKD,WORKL,INFO]=dnaupd...
       (ID0,BMAT,N,WHICH,NEV,TOL,RESID,NCV,V,IPARAM,IPNTR,WORKD,WORKL,INFO) */

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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpRESID, &mRESID, &nRESID, &pRESID);
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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpV, &mV, &nV, &pV);
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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpWORKD, &mWORKD, &nWORKD, &pWORKD);
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
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpWORKL, &mWORKL, &nWORKL, &pWORKL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 13);
        return 1;
    }

    WORKL = 13;
    sciErr = getVarAddressFromPosition(pvApiCtx, 14, &piAddrpINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 14.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpINFO, &mINFO, &nINFO, &pINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 14);
        return 1;
    }

    INFO = 14;

    LWORKL = mWORKL * nWORKL;
    LDV = Max(1, pN[0]);

    /* Don't call dnaupd if ido == 99 */
    if (pIDO[0] == 99)
    {
        Scierror(999, _("%s: the computation is already terminated\n"), fname);
        return 1;
    }

    /* Check some sizes */
    if (mIPARAM * nIPARAM != 11)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "IPARAM", 11);
        return 1;
    }

    if (mIPNTR * nIPNTR != 14)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "IPNTR", 14);
        return 1;
    }

    if (mRESID * nRESID != pN[0])
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "RESID", *(int*)(pN));
        return 1;
    }

    if ((mV != pN[0]) || (nV != pNCV[0]))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "V", *(int*)(pN), *(int*)(pNCV));
        return 1;
    }

    if (mWORKD * nWORKD < 3 * pN[0])
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKD", 3 * *(int*)(pN));
        return 1;
    }

    sizeWORKL = 3 * pNCV[0] * pNCV[0] + 6 * pNCV[0];

    if (mWORKL * nWORKL < sizeWORKL)
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKL", sizeWORKL);
        return 1;
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

    C2F(dnaupd)(pIDO, pBMAT, pN,
                pWHICH, pNEV, pTOL,
                pRESID, pNCV, pV, &LDV,
                pIPARAM, pIPNTR, pWORKD,
                pWORKL, &LWORKL, pINFO, 1L, 2L);

    freeAllocatedSingleString(pBMAT);
    freeAllocatedSingleString(pWHICH);

    if (*pINFO < 0)
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
    AssignOutputVariable(pvApiCtx, 8) = INFO;

    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
