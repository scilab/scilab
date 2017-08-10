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
extern int C2F(dneupd)(int *rvec, char *howmny, int *select, double *dr,
                       double *di, double *z, int *ldz, double *sigmar,
                       double *sigmai, double *workev, char *bmat, int *n,
                       char *which, int *nev, double *tol, double *resid,
                       int *ncv, double *v, int *ldv, int *iparam, int *ipntr,
                       double *workd, double *workl, int *lworkl, int *info);
/*--------------------------------------------------------------------------*/
int sci_dneupd(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrpRVEC    = NULL;
    int* pRVEC          = NULL;
    int* piAddrpHOWMANY = NULL;
    char* pHOWMANY      = NULL;
    int* piAddrpSELECT  = NULL;
    int* pSELECT        = NULL;
    int* piAddrpDr      = NULL;
    double* pDr         = NULL;
    int* piAddrpDi      = NULL;
    double* pDi         = NULL;
    int* piAddrpZ       = NULL;
    double* pZ          = NULL;
    int* piAddrpSIGMAr  = NULL;
    double* pSIGMAr     = NULL;
    int* piAddrpSIGMAi  = NULL;
    double* pSIGMAi     = NULL;
    int* piAddrpWORKev  = NULL;
    double* pWORKev     = NULL;
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

    int mRVEC,     nRVEC;
    int mSELECT,   nSELECT;
    int Dr,        mDr,       nDr;
    int Di,        mDi,       nDi;
    int Z,         mZ,        nZ;
    int mSIGMAr,   nSIGMAr;
    int mSIGMAi,   nSIGMAi;
    int mWORKev,   nWORKev;
    int mN,        nN;
    int mNEV,      nNEV;
    int mTOL,      nTOL;
    int RESID,    mRESID,    nRESID;
    int mNCV,      nNCV;
    int V,        mV,        nV;
    int IPARAM,   mIPARAM,   nIPARAM;
    int IPNTR,    mIPNTR,    nIPNTR;
    int WORKD,    mWORKD,    nWORKD;
    int WORKL,    mWORKL,    nWORKL;
    int INFO,     mINFO,     nINFO;

    int minlhs = 1, minrhs = 22, maxlhs = 10, maxrhs = 22;
    int LDZ, LDV, LWORKL;
    int sizeWORKL = 0;

    CheckInputArgument(pvApiCtx, minrhs, maxrhs);
    CheckOutputArgument(pvApiCtx, minlhs, maxlhs);

    /*                                                  VARIABLE = NUMBER   */
    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrpRVEC);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpRVEC, &mRVEC, &nRVEC, &pRVEC);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrpSELECT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpSELECT, &mSELECT, &nSELECT, &pSELECT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrpDr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpDr, &mDr, &nDr, &pDr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        return 1;
    }

    Dr =  4;
    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrpDi);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpDi, &mDi, &nDi, &pDi);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        return 1;
    }

    Di =  5;
    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddrpZ);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 6.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpZ, &mZ, &nZ, &pZ);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 6);
        return 1;
    }

    Z =  6;
    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddrpSIGMAr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 7.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpSIGMAr, &mSIGMAr, &nSIGMAr, &pSIGMAr);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 7);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddrpSIGMAi);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 8.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpSIGMAi, &mSIGMAi, &nSIGMAi, &pSIGMAi);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 8);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddrpWORKev);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 9.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpWORKev, &mWORKev, &nWORKev, &pWORKev);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 9);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 11, &piAddrpN);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 11.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpN, &mN, &nN, &pN);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 11);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 13, &piAddrpNEV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 13.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpNEV, &mNEV, &nNEV, &pNEV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 13);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 14, &piAddrpTOL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 14.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpTOL, &mTOL, &nTOL, &pTOL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 14);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 15, &piAddrpRESID);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 15.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpRESID, &mRESID, &nRESID, &pRESID);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 15);
        return 1;
    }

    RESID = 15;
    sciErr = getVarAddressFromPosition(pvApiCtx, 16, &piAddrpNCV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 16.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpNCV, &mNCV, &nNCV, &pNCV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 16);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 17, &piAddrpV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 17.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpV, &mV, &nV, &pV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 17);
        return 1;
    }

    V = 17;
    sciErr = getVarAddressFromPosition(pvApiCtx, 18, &piAddrpIPARAM);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 18.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpIPARAM, &mIPARAM, &nIPARAM, &pIPARAM);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 18);
        return 1;
    }

    IPARAM = 18;
    sciErr = getVarAddressFromPosition(pvApiCtx, 19, &piAddrpIPNTR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 19.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpIPNTR, &mIPNTR, &nIPNTR, &pIPNTR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 19);
        return 1;
    }

    IPNTR = 19;
    sciErr = getVarAddressFromPosition(pvApiCtx, 20, &piAddrpWORKD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 20.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpWORKD, &mWORKD, &nWORKD, &pWORKD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 20);
        return 1;
    }

    WORKD = 20;
    sciErr = getVarAddressFromPosition(pvApiCtx, 21, &piAddrpWORKL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 21.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpWORKL, &mWORKL, &nWORKL, &pWORKL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 21);
        return 1;
    }

    WORKL = 21;
    sciErr = getVarAddressFromPosition(pvApiCtx, 22, &piAddrpINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 22.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpINFO, &mINFO, &nINFO, &pINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 22);
        return 1;
    }

    INFO = 22;

    LWORKL = mWORKL * nWORKL;
    LDV = Max(1, *(int*)(pN));
    LDZ = LDV;

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

    if (mRESID * nRESID != *(int*)(pN))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "RESID", *(int*)(pN));
        return 0;
    }

    if (mWORKD * nWORKD < 3 * *(int*)(pN))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKD", 3 * *(int*)(pN));
        return 0;
    }

    if (mSELECT * nSELECT != *(int*)(pNCV))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "SELECT", *(int*)(pNCV));
        return 0;
    }

    if (mDr * nDr != (*(int*)(pNEV) + 1))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "Dr", *(int*)(pNEV) + 1);
        return 0;
    }

    if (mDi * nDi != (*(int*)(pNEV) + 1))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "Di", *(int*)(pNEV) + 1);
        return 0;
    }

    if ((mZ != *(int*)(pN)) || (nZ != *(int*)(pNEV) + 1))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "Z", *(int*)(pN), *(int*)(pNEV) + 1);
        return 0;
    }

    if (mWORKev * nWORKev != 3 * *(int*)(pNCV))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKev", 3 * *(int*)(pNCV));
        return 0;
    }

    if ((mV != *(int*)(pN)) || (nV != *(int*)(pNCV)))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "V", *(int*)(pN), *(int*)(pNCV));
        return 0;
    }

    sizeWORKL = 3 * *(int*)(pNCV) * *(int*)(pNCV) + 6 * *(int*)(pNCV);

    if ((mWORKL * nWORKL < sizeWORKL))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKL", sizeWORKL);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrpHOWMANY);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    if (getAllocatedSingleString(pvApiCtx, piAddrpHOWMANY, &pHOWMANY))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddrpBMAT);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(pHOWMANY);
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 10.
    if (getAllocatedSingleString(pvApiCtx, piAddrpBMAT, &pBMAT))
    {
        freeAllocatedSingleString(pHOWMANY);
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 10);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 12, &piAddrpWHICH);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(pHOWMANY);
        freeAllocatedSingleString(pBMAT);
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 12.
    if (getAllocatedSingleString(pvApiCtx, piAddrpWHICH, &pWHICH))
    {
        freeAllocatedSingleString(pHOWMANY);
        freeAllocatedSingleString(pBMAT);
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 12);
        return 1;
    }

    C2F(dneupd)(pRVEC, pHOWMANY, pSELECT,
                pDr, pDi, pZ, &LDZ,
                pSIGMAr, pSIGMAi, pWORKev,
                pBMAT, pN, pWHICH,
                pNEV, pTOL, pRESID,
                pNCV, pV, &LDV,
                pIPARAM, pIPNTR,
                pWORKD, pWORKL, &LWORKL,
                pINFO);

    freeAllocatedSingleString(pHOWMANY);
    freeAllocatedSingleString(pBMAT);
    freeAllocatedSingleString(pWHICH);

    if (pINFO[0] < 0)
    {
        Scierror(998, _("%s: internal error, info=%d.\n"), fname, *pINFO);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1)  = Dr;
    AssignOutputVariable(pvApiCtx, 2)  = Di;
    AssignOutputVariable(pvApiCtx, 3)  = Z;
    AssignOutputVariable(pvApiCtx, 4)  = RESID;
    AssignOutputVariable(pvApiCtx, 5)  = V;
    AssignOutputVariable(pvApiCtx, 6)  = IPARAM;
    AssignOutputVariable(pvApiCtx, 7)  = IPNTR;
    AssignOutputVariable(pvApiCtx, 8)  = WORKD;
    AssignOutputVariable(pvApiCtx, 9)  = WORKL;
    AssignOutputVariable(pvApiCtx, 10) = INFO;

    ReturnArguments(pvApiCtx);

    return 0;
}
/*--------------------------------------------------------------------------*/
