/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (//) 2012 - Scilab Enterprises - Cedric DELAMARRE
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
#include "gw_cacsd.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "numericconstants_interface.h"

extern int C2F(sb10fd)();

// [Ak,Bk,Ck,Dk,RCOND]=hinf(A,B,C,D,ncon,nmeas,gamma)
int sci_hinf(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrlA   = NULL;
    double* lA      = NULL;
    int* piAddrlB   = NULL;
    double* lB      = NULL;
    int* piAddrlC   = NULL;
    double* lC      = NULL;
    int* piAddrlD   = NULL;
    double* lD      = NULL;
    double* lAK     = NULL;
    double* lBK     = NULL;
    double* lCK     = NULL;
    double* lDK     = NULL;
    double* lRCOND  = NULL;
    int* lNCON      = NULL;
    int* lNMEAS     = NULL;
    double* lGAMMA  = NULL;
    int* lIWORK     = NULL;
    int* lBWORK     = NULL;
    double* lDWORK  = NULL;

    int* piAddrlGAMMA   = NULL;
    int* piAddrlNCON    = NULL;
    int* piAddrlNMEAS   = NULL;

    double GAMMA = 0;
    double TOL   = 0;
    double EPS   = 0;

    int NCON     = 0;
    int NMEAS    = 0;
    int LINTWORK = 0;
    int LWORKMIN = 0;

    int MA = 0, NA = 0;
    int MB = 0, NB = 0;
    int MC = 0, NC = 0;
    int MD = 0, ND = 0;
    int M1 = 0, N1 = 0;
    int M2 = 0, N2 = 0;
    int M3 = 0, N3 = 0;

    int N;
    int M;
    int R;
    int Q;

    int minrhs = 7;
    int maxrhs = 7;
    int minlhs = 4;
    int maxlhs = 5;

    int iZero = 0;
    int iOne  = 1;
    int iFor  = 4;
    int i2N   = 0;
    int info  = 0;

    CheckInputArgument(pvApiCtx, minrhs, maxrhs);
    CheckOutputArgument(pvApiCtx, minlhs, maxlhs);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrlA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrlA, &MA, &NA, &lA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 1);
        return 1;
    }

    if (MA != NA)
    {
        Scierror(999, _("%s: A must be a square matrix.\n"), fname);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrlB);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrlB, &MB, &NB, &lB);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    if (MA != MB)
    {
        Scierror(999, _("%s: A and B must have equal number of rows.\n"), fname);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrlC);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrlC, &MC, &NC, &lC);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        return 1;
    }

    if (NA != NC)
    {
        Scierror(999, _("%s: A and C must have equal number of columns.\n"), fname);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrlD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrlD, &MD, &ND, &lD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 4);
        return 1;
    }

    if (NB != ND)
    {
        Scierror(999, _("%s: B and D must have equal number of columns.\n"), fname);
        return 1;
    }

    if (MC != MD)
    {
        Scierror(999, _("%s: C and D must have equal number of rows.\n"), fname);
        return 1;
    }

    N = MA;
    M = NB;
    R = MC;

    if (N == 0 || M == 0 || R == 0)
    {
        sciErr = allocMatrixOfDouble(pvApiCtx, 5, iZero, iZero, &lAK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, 6, iZero, iZero, &lBK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, 7, iZero, iZero, &lCK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, 8, iZero, iZero, &lDK);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }

        sciErr = allocMatrixOfDouble(pvApiCtx, 9, iZero, iZero, &lRCOND);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 1;
        }


        AssignOutputVariable(pvApiCtx, 1) = 5;
        AssignOutputVariable(pvApiCtx, 2) = 6;
        AssignOutputVariable(pvApiCtx, 3) = 7;
        AssignOutputVariable(pvApiCtx, 4) = 8;
        AssignOutputVariable(pvApiCtx, 5) = 9;
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrlNCON);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 5.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrlNCON, &M1, &N1, &lNCON);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 5);
        return 1;
    }


    if (M1 != 1 || N1 != 1)
    {
        Scierror(999, _("%s: NCON must be a scalar.\n"), fname);
        return 1;
    }

    NCON = *lNCON;

    sciErr = getVarAddressFromPosition(pvApiCtx, 6, &piAddrlNMEAS);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 6.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrlNMEAS, &M2, &N2, &lNMEAS);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 6);
        return 1;
    }

    if (M2 != 1 || N2 != 1)
    {
        Scierror(999, _("%s: NMEAS must be a scalar.\n"), fname);
        return 1;
    }

    NMEAS = *lNMEAS;

    sciErr = getVarAddressFromPosition(pvApiCtx, 7, &piAddrlGAMMA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 7.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrlGAMMA, &M3, &N3, &lGAMMA);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 7);
        return 1;
    }

    if (M3 != 1 || N3 != 1)
    {
        Scierror(999, _("%s: GAMMA must be a scalar.\n"), fname);
        return 1;
    }

    GAMMA = *lGAMMA;
    EPS = nc_eps();
    TOL = sqrt(EPS);

    sciErr = allocMatrixOfDouble(pvApiCtx, 8, N, N, &lAK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, 9, N, NMEAS, &lBK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, 10, NCON, N, &lCK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, 11, NCON, NMEAS, &lDK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, 12, iFor, iOne, &lRCOND);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    LINTWORK = Max(2 * Max(N, Max(M - NCON, Max(R - NMEAS, NCON))), N * N);
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 13, LINTWORK, iOne, &lIWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    i2N = 2 * N;
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 14, i2N, iOne, &lBWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }


    Q = Max(M - NCON, Max(NCON, Max(R - NMEAS, NMEAS)));

    LWORKMIN =  2 * Q * ( 3 * Q + 2 * N ) +
                Max(1,
                    Max(( N + Q ) * ( N + Q + 6),
                        Max(Q * ( Q + Max(N, Max(Q, 5)) + 1),
                            2 * N * (N + 2 * Q) + Max( 1,
                                    Max(4 * Q * Q + Max(2 * Q, 3 * N * N + Max( 2 * N * Q, 10 * N * N + 12 * N + 5 )),
                                        Q * ( 3 * N + 3 * Q + Max( 2 * N, 4 * Q + Max(N, Q))))))));

    sciErr = allocMatrixOfDouble(pvApiCtx, 15, iOne, LWORKMIN, &lDWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    C2F(sb10fd)(&N, &M, &R, &NCON, &NMEAS, &GAMMA, lA, &N,
                lB, &N, lC, &R, lD, &R, lAK,
                &N, lBK, &N, lCK, &NCON, lDK,
                &NCON, lRCOND, &TOL, lIWORK,
                lDWORK, &LWORKMIN, lBWORK, &info);

    if (info != 0)
    {
        Scierror(999, _("%s: SB10FD exit with info = %d.\n"), fname, info);
        return 1;
    }

    AssignOutputVariable(pvApiCtx, 1) = 8;
    AssignOutputVariable(pvApiCtx, 2) = 9;
    AssignOutputVariable(pvApiCtx, 3) = 10;
    AssignOutputVariable(pvApiCtx, 4) = 11;
    if (nbOutputArgument(pvApiCtx) == 5)
    {
        AssignOutputVariable(pvApiCtx, 5) = 14;
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
