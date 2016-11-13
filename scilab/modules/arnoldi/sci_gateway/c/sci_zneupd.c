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
#include "gw_arnoldi.h"
#include "core_math.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(zneupd)(int * rvec, char * howmny, int * select,
                       doublecomplex * d, doublecomplex * z, int * ldz,
                       doublecomplex * sigma, doublecomplex * workev,
                       char * bmat, int * n, char * which, int * nev,
                       double *  tol, doublecomplex * resid, int * ncv,
                       doublecomplex * v, int * ldv, int * iparam, int * ipntr,
                       doublecomplex * workd, doublecomplex * workl,
                       int * lworkl, double * rwork, int * info);
/*--------------------------------------------------------------------------*/
int sci_zneupd(char *fname, void *pvApiCtx)
{
    SciErr sciErr;

    int* piAddrpRVEC    = NULL;
    int* pRVEC          = NULL;
    int* piAddrpHOWMANY = NULL;
    char* pHOWMANY      = NULL;
    int* piAddrpSELECT  = NULL;
    int* pSELECT        = NULL;
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

    int* piAddrpD           = NULL;
    doublecomplex* pD       = NULL;
    int* piAddrpZ           = NULL;
    doublecomplex* pZ       = NULL;
    int* piAddrpSIGMA       = NULL;
    doublecomplex* pSIGMA   = NULL;
    int* piAddrpWORKev      = NULL;
    doublecomplex* pWORKev  = NULL;
    int* piAddrpRESID       = NULL;
    doublecomplex* pRESID   = NULL;
    int* piAddrpWORKD       = NULL;
    doublecomplex* pV       = NULL;
    int* piAddrpV           = NULL;
    doublecomplex* pWORKD   = NULL;
    int* piAddrpWORKL       = NULL;
    doublecomplex* pWORKL   = NULL;

    int mRVEC,     nRVEC;
    int mSELECT,   nSELECT;
    int D,        mD,        nD;
    int Z,        mZ,        nZ;
    int mSIGMA,    nSIGMA;
    int mWORKev,   nWORKev;
    int mN,        nN;
    int mNEV,      nNEV;
    int mTOL,      nTOL;
    int RESID,    mRESID,    nRESID;
    int mNCV,      nNCV;
    int mV,        nV;
    int IPARAM,   mIPARAM,   nIPARAM;
    int IPNTR,    mIPNTR,    nIPNTR;
    int WORKD,    mWORKD,    nWORKD;
    int WORKL,    mWORKL,    nWORKL;
    int RWORK,    mRWORK,    nRWORK;
    int INFO,     mINFO,     nINFO;

    int minlhs = 1, minrhs = 21, maxlhs = 9, maxrhs = 21;
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

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrpD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpD, &mD, &nD, &pD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        return 1;
    }
    D =  4;

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrpZ);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpZ, &mZ, &nZ, &pZ);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        return 1;
    }
    Z =  5;

    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddrpSIGMA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 6.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpSIGMA, &mSIGMA, &nSIGMA, &pSIGMA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 6);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddrpWORKev);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 7.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpWORKev, &mWORKev, &nWORKev, &pWORKev);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 7);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 9, &piAddrpN);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 9.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpN, &mN, &nN, &pN);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 9);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 11, &piAddrpNEV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 11.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpNEV, &mNEV, &nNEV, &pNEV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 11);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 12, &piAddrpTOL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 12.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpTOL, &mTOL, &nTOL, &pTOL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 12);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 13, &piAddrpRESID);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 13.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpRESID, &mRESID, &nRESID, &pRESID);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 13);
        return 1;
    }
    RESID = 13;

    sciErr = getVarAddressFromPosition(pvApiCtx, 14, &piAddrpNCV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 14.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpNCV, &mNCV, &nNCV, &pNCV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 14);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 15, &piAddrpV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 15.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpV, &mV, &nV, &pV);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 15);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 16, &piAddrpIPARAM);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 16.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpIPARAM, &mIPARAM, &nIPARAM, &pIPARAM);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 16);
        return 1;
    }

    IPARAM = 16;
    sciErr = getVarAddressFromPosition(pvApiCtx, 17, &piAddrpIPNTR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 17.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpIPNTR, &mIPNTR, &nIPNTR, &pIPNTR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 17);
        return 1;
    }

    IPNTR = 17;

    sciErr = getVarAddressFromPosition(pvApiCtx, 18, &piAddrpWORKD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 18.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpWORKD, &mWORKD, &nWORKD, &pWORKD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 18);
        return 1;
    }
    WORKD = 18;

    sciErr = getVarAddressFromPosition(pvApiCtx, 19, &piAddrpWORKL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 19.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrpWORKL, &mWORKL, &nWORKL, &pWORKL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 19);
        return 1;
    }
    WORKL = 19;

    sciErr = getVarAddressFromPosition(pvApiCtx, 20, &piAddrpRWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 20.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrpRWORK, &mRWORK, &nRWORK, &pRWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 20);
        return 1;
    }

    RWORK = 20;
    sciErr = getVarAddressFromPosition(pvApiCtx, 21, &piAddrpINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 21.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrpINFO, &mINFO, &nINFO, &pINFO);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 21);
        return 1;
    }

    INFO = 21;

    LWORKL = mWORKL * nWORKL;
    LDV = Max(1, pN[0]);
    LDZ = LDV;

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
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "RESID", pN[0]);
        return 1;
    }

    if (mWORKD * nWORKD < 3 * pN[0])
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKD", 3 * pN[0]);
        return 1;
    }

    if (mSELECT * nSELECT != pNCV[0])
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "SELECT", pNCV[0]);
        return 1;
    }

    if (mD * nD != (pNEV[0] + 1))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "D", pNEV[0] + 1);
        return 1;
    }

    if ((mZ != pN[0]) || (nZ != pNEV[0]))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "Z", pN[0], pNEV[0]);
        return 1;
    }

    if (mWORKev * nWORKev != 2 * pNCV[0])
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKev", 2 * pNCV[0]);
        return 1;
    }

    if ((mV != pN[0]) || (nV != pNCV[0]))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: A matrix of size %dx%d expected.\n"), fname, "V", pN[0], pNCV[0]);
        return 1;
    }

    sizeWORKL = 3 * pNCV[0] * pNCV[0] + 5 * pNCV[0];

    if ((mWORKL * nWORKL < sizeWORKL))
    {
        Scierror(999, _("%s: Wrong size for input argument %s: An array of size %d expected.\n"), fname, "WORKL", sizeWORKL);
        return 1;
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

    sciErr = getVarAddressFromPosition(pvApiCtx, 8, &piAddrpBMAT);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(pHOWMANY);
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 8.
    if (getAllocatedSingleString(pvApiCtx, piAddrpBMAT, &pBMAT))
    {
        freeAllocatedSingleString(pHOWMANY);
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 8);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 10, &piAddrpWHICH);
    if (sciErr.iErr)
    {
        freeAllocatedSingleString(pHOWMANY);
        freeAllocatedSingleString(pBMAT);
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 10.
    if (getAllocatedSingleString(pvApiCtx, piAddrpWHICH, &pWHICH))
    {
        freeAllocatedSingleString(pHOWMANY);
        freeAllocatedSingleString(pBMAT);
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 10);
        return 1;
    }

    C2F(zneupd)(pRVEC, pHOWMANY, pSELECT, pD,
                pZ, &LDZ, pSIGMA, pWORKev,
                pBMAT, pN, pWHICH, pNEV,
                pTOL, pRESID, pNCV, pV,
                &LDV, pIPARAM, pIPNTR, pWORKD,
                pWORKL, &LWORKL, pRWORK, pINFO);

    freeAllocatedSingleString(pHOWMANY);
    freeAllocatedSingleString(pBMAT);
    freeAllocatedSingleString(pWHICH);

    if (pINFO[0] < 0)
    {
        Scierror(998, _("%s: internal error, info=%d.\n"), fname, *pINFO);
        return 0;
    }

    AssignOutputVariable(pvApiCtx, 1) = D;
    AssignOutputVariable(pvApiCtx, 2) = Z;
    AssignOutputVariable(pvApiCtx, 3) = RESID;
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
