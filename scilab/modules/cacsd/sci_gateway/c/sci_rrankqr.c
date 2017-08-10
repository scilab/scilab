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
#include "sci_rankqr.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

extern int C2F(icopy)();
extern int C2F(dcopy)();
extern int C2F(mb03od)();
extern int C2F(dorgqr)();

int sci_rrankqr(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrptrA     = NULL;
    double* ptrA        = NULL;
    int* piAddrptrRCOND = NULL;
    double* ptrRCOND    = NULL;
    int* ptrJPVT        = NULL;
    int* piAddrptrJPVT  = NULL;
    double* ptrSVAL     = NULL;
    double* ptrTAU      = NULL;
    int* ptrRANK        = NULL;
    double* ptrR        = NULL;
    double* ptrQ        = NULL;
    double* ptrDWORK    = NULL;

    int minrhs = 1;
    int maxrhs = 3;
    int minlhs = 1;
    int maxlhs = 5;

    char JOBQR      = 'Q';
    double SLVMAX   = 0.;
    double RCONDdef = 0.;
    double ZERO     = 0.;

    int A       = 1;
    int RCOND   = 2;
    int JPVT    = 3;
    int SVAL    = 4;
    int TAU     = 5;
    int RANK    = 6;
    int R       = 7;
    int Q       = 8;
    int DWORK   = 9;

    int LDWORKMIN = 0, LDA = 0, INFO = 0, IRANK = 0, IR = 0;
    int II = 0, NB = 0, NB1 = 0, IR1 = 0;

    int mR = 0, nR = 0;
    int mJ = 0, nJ = 0;
    int M  = 0, N  = 0;

    int iZero = 0, iOne = 1, iThree = 3;
    int i = 0, iMin = 0, iSize = 0;

    CheckOutputArgument(pvApiCtx, minlhs, maxlhs);
    CheckInputArgument(pvApiCtx, minrhs, maxrhs);

    sciErr = getVarAddressFromPosition(pvApiCtx, A, &piAddrptrA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position A.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrptrA, &M, &N, &ptrA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, A);
        return 1;
    }


    if (nbInputArgument(pvApiCtx) == 1)
    {
        // rankqr(A)
        // create RCOND=0.0,JPVT=[0,..,0]
        sciErr = allocMatrixOfDouble(pvApiCtx, RCOND, iOne, iOne, &ptrRCOND);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        *ptrRCOND = RCONDdef;
        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, JPVT, iOne, N, &ptrJPVT);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        C2F(icopy)(&N, &iZero, &iZero, ptrJPVT, &iOne);
    }
    else if (nbInputArgument(pvApiCtx) == 2)
    {
        // rankqr(A,RCOND)
        // get RCOND, create JPVT=[0,..,0]
        sciErr = getVarAddressFromPosition(pvApiCtx, RCOND, &piAddrptrRCOND);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position RCOND.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrptrRCOND, &mR, &nR, &ptrRCOND);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, RCOND);
            return 1;
        }

        if (mR * nR != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, RCOND);
            return 1;
        }

        sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, JPVT, iOne, N, &ptrJPVT);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        C2F(icopy)(&N, &iZero, &iZero, ptrJPVT, &iOne);
    }
    else if (nbInputArgument(pvApiCtx) == 3)
    {
        // rankqr(A,RCOND,JPVT)
        // get  RCOND, JPVT
        sciErr = getVarAddressFromPosition(pvApiCtx, RCOND, &piAddrptrRCOND);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position RCOND.
        sciErr = getMatrixOfDouble(pvApiCtx, piAddrptrRCOND, &mR, &nR, &ptrRCOND);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, RCOND);
            return 1;
        }

        if (mR * nR != 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: A scalar expected.\n"), fname, RCOND);
            return 1;
        }

        sciErr = getVarAddressFromPosition(pvApiCtx, JPVT, &piAddrptrJPVT);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position JPVT.
        sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrptrJPVT, &mJ, &nJ, &ptrJPVT);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, JPVT);
            return 1;
        }

        if (mJ * nJ != N)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d: Same column dimension as input argument #%d expected.\n"), fname, JPVT, A);
            return 1;
        }
    }

    // Creating  SVAL,TAU,RANK,R,Q,DWORK
    sciErr = allocMatrixOfDouble(pvApiCtx, SVAL, iOne, iThree, &ptrSVAL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    iMin = Min(M, N);
    sciErr = allocMatrixOfDouble(pvApiCtx, TAU, iOne, iMin, &ptrTAU);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, RANK, iOne, iOne, &ptrRANK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, R, M, N, &ptrR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, Q, M, M, &ptrQ);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    LDWORKMIN = Max(1, 3 * N);
    sciErr = allocMatrixOfDouble(pvApiCtx, DWORK, iOne, LDWORKMIN, &ptrDWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    LDA = Max(1, M);

    C2F(mb03od)(&JOBQR, &M, &N, ptrA, &LDA, ptrJPVT,
                ptrRCOND, &SLVMAX, ptrTAU, ptrRANK,
                ptrSVAL, ptrDWORK, &INFO);

    if (INFO != 0)
    {
        Scierror(999, _("%s: MB03OD exit with info = %d.\n"), fname, INFO);
        return 1;
    }

    //  Save R (=current A)
    iSize = M * N;
    C2F(dcopy)(&iSize, (ptrA), &iOne, (ptrR), &iOne);

    //  Set to zero lower trapezoidal part of R:
    IRANK = *ptrRANK;
    IR = IRANK - M;
    for (i = 0; i < N; i++)
    {
        II = II + 1;
        NB = M - IRANK;
        IR = IR + M;
        NB1 = NB;
        IR1 = IR;
        if ((i + 1) < IRANK)
        {
            IR1 = IR - (IRANK - (i + 1));
            NB1 = NB + (IRANK - (i + 1));
        }

        C2F(dcopy)(&NB1, &ZERO, &iZero, (ptrR + IR1), &iOne);
    }

    // Make Q
    if (M <= N)
    {
        // A is fat
        C2F(dorgqr)(&M, &M, &M, ptrA, &LDA, ptrTAU, ptrDWORK, &LDWORKMIN, &INFO);
        iSize = M * M;
        C2F(dcopy)(&iSize, ptrA, &iOne, ptrQ, &iOne);
    }
    else
    {
        // A is tall => Q=[A,0]
        iSize = M * N;
        C2F(dcopy)(&iSize, ptrA, &iOne, ptrQ, &iOne);
        iSize = M * (M - N);
        C2F(dcopy)(&iSize, &ZERO, &iZero, (ptrQ + M * N), &iOne);
        iMin = Min(M, N);
        C2F(dorgqr)(&M, &M, &iMin, ptrQ, &LDA, ptrTAU, ptrDWORK, &LDWORKMIN, &INFO);
    }

    if (INFO != 0)
    {
        Scierror(999, _("%s: DORGQR exit with info = %d.\n"), fname, INFO);
        return 1;
    }

    // [Q,R,JPVT,RANK,SVAL]=rankqr(A,[RCOND,JPVT])
    AssignOutputVariable(pvApiCtx, 1) = Q;
    AssignOutputVariable(pvApiCtx, 2) = R;
    AssignOutputVariable(pvApiCtx, 3) = JPVT;
    AssignOutputVariable(pvApiCtx, 4) = RANK;
    AssignOutputVariable(pvApiCtx, 5) = SVAL;

    ReturnArguments(pvApiCtx);
    return 0;
}
