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
#include <string.h>
#include "gw_cacsd.h"
#include "sci_rankqr.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

extern int C2F(riccsl)();
extern int C2F(riccms)();
extern int C2F(ricdsl)();
extern int C2F(ricdmf)();

int sci_ricc(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrlA   = NULL;
    double* lA      = NULL;
    int* piAddrlD   = NULL;
    double* lD      = NULL;
    int* piAddrlC   = NULL;
    double* lC      = NULL;
    char* lTYPE     = NULL;
    char* lMETHOD   = NULL;
    double* lX      = NULL;
    double* lWR     = NULL;
    double* lWI     = NULL;
    double* lRCOND  = NULL;
    double* lFERR   = NULL;
    int* lIWORK     = NULL;
    int* lBWORK     = NULL;
    double* lDWORK  = NULL;

    int* piAddrlTYPE = NULL;
    int* piAddrlMETHOD = NULL;

    BOOL WANTC = 0, WANTD = 0, WSCHUR = 0, WSIGN = 0, WINVF = 0;

    int minrhs = 4;
    int maxrhs = 5;
    int minLhs = 1;
    int maxLhs = 3;

    int N = 0, LWORKMIN = 0, INFO = 0;
    int MA = 0, NA = 0;
    int MC = 0, NC = 0;
    int MD = 0, ND = 0;
    int M1 = 0, N1 = 0;

    int iOne = 1, iSize = 0, k = 0;

    CheckInputArgument(pvApiCtx, minrhs, maxrhs);
    CheckOutputArgument(pvApiCtx, minLhs, maxLhs);

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
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 1);
        return 1;
    }

    if (MA != NA)
    {
        Scierror(999, _("%s: A must be a square matrix.\n"), fname);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrlD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDouble(pvApiCtx, piAddrlD, &MD, &ND, &lD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 2);
        return 1;
    }

    if (MD != ND)
    {
        Scierror(999, _("%s: D must be a square matrix.\n"), fname);
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
        Scierror(202, _("%s: Wrong type for argument %d: A real expected.\n"), fname, 3);
        return 1;
    }

    if (MC != NC)
    {
        Scierror(999, _("%s: C must be a square matrix.\n"), fname);
        return 1;
    }

    if (MA != MC || MC != MD || MA != MD)
    {
        Scierror(999, _("%s: The matrices A, C and D must have the same order.\n"), fname);
        return 1;
    }

    N = MA;

    sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddrlTYPE);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 4.
    if (getAllocatedSingleString(pvApiCtx, piAddrlTYPE, &lTYPE))
    {
        Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 4);
        return 1;
    }

    WANTC = (strcmp((lTYPE), "cont") == 0 || strcmp((lTYPE), "CONT") == 0);
    WANTD = (strcmp((lTYPE), "disc") == 0 || strcmp((lTYPE), "DISC") == 0);
    freeAllocatedSingleString(lTYPE);

    if (WANTC == FALSE && WANTD == FALSE)
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: Type must be continuous or discrete.\n"), fname, 4);
        return 1;
    }

    k = 5;
    WSCHUR = TRUE;
    if (nbInputArgument(pvApiCtx) == 5)
    {
        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddrlMETHOD);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 1;
        }

        // Retrieve a matrix of double at position 5.
        if (getAllocatedSingleString(pvApiCtx, piAddrlMETHOD, &lMETHOD))
        {
            Scierror(202, _("%s: Wrong type for argument #%d: string expected.\n"), fname, 5);
            return 1;
        }

        WSCHUR = (strcmp((lMETHOD), "schr") == 0 || strcmp((lMETHOD), "SCHR") == 0);
        if (WANTC)
        {
            WSIGN  = (strcmp((lMETHOD), "sign") == 0 || strcmp((lMETHOD), "SIGN") == 0);
            if (WSCHUR == FALSE && WSIGN == FALSE)
            {
                freeAllocatedSingleString(lMETHOD);
                Scierror(999, _("%s: Wrong value for input argument #%d: Method must be schur or sign.\n"), fname, 5);
                return 1;
            }
        }
        else
        {
            WINVF  = (strcmp((lMETHOD), "invf") == 0 || strcmp((lMETHOD), "INVF") == 0);
            if (WSCHUR == FALSE && WINVF == FALSE)
            {
                freeAllocatedSingleString(lMETHOD);
                Scierror(999, _("%s: Wrong value for input argument #%d: Method must be schur or invf.\n"), fname, 5);
                return 1;
            }
        }

        k = 6;
        freeAllocatedSingleString(lMETHOD);
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, k, N, N, &lX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, k + 1, N, iOne, &lWR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, k + 2, N, iOne, &lWI);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, k + 3, iOne, iOne, &lRCOND);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, k + 4, iOne, iOne, &lFERR);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    iSize = Max(2 * N, N * N);
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, k + 5, iOne, iSize, &lIWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    iSize = 2 * N;
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, k + 6, iOne, iSize, &lBWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    if (WANTC)
    {
        if (WSCHUR)
        {
            LWORKMIN = 9 * N * N + 4 * N + Max(1, 6 * N);
        }
        else if (WSIGN)
        {
            LWORKMIN = 9 * N * N + 7 * N + 1;
        }
    }
    else
    {
        if (WSCHUR)
        {
            LWORKMIN = 12 * N * N + 22 * N + Max(16, 4 * N);
        }
        else if (WINVF)
        {
            LWORKMIN = 28 * N * N + 2 * N + Max(1, 2 * N);
        }
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, k + 7, iOne, LWORKMIN, &lDWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    if (WANTC)
    {
        if (WSCHUR)
        {
            C2F(riccsl)("N", &N, lA, &N, "U", lC, &N, lD,
                        &N, lX, &N, lWR, lWI, lRCOND,
                        lFERR, lDWORK, &LWORKMIN, lIWORK,
                        lBWORK, &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICCSL exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
        else if (WSIGN)
        {
            C2F(riccms)("N", &N, lA, &N, "U", lC, &N, lD,
                        &N, lX, &N, lWR, lWI, lRCOND,
                        lFERR, lDWORK, &LWORKMIN, lIWORK, &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICCMS exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
    }
    else
    {
        if (WSCHUR)
        {
            C2F(ricdsl)("N", &N, lA, &N, "U", lC, &N, lD,
                        &N, lX, &N, lWR, lWI, lRCOND,
                        lFERR, lDWORK, &LWORKMIN, lIWORK,
                        lBWORK, &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICDSL exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
        else if (WINVF)
        {
            C2F(ricdmf)("N", &N, lA, &N, "U", lC, &N, lD,
                        &N, lX, &N, lWR, lWI, lRCOND,
                        lFERR, lDWORK, &LWORKMIN, lIWORK, &INFO);

            if (INFO != 0)
            {
                Scierror(999, _("%s: RICDMF exit with info = %d.\n"), fname, INFO);
                return 1;
            }
        }
    }

    if (nbOutputArgument(pvApiCtx) == 1)
    {
        AssignOutputVariable(pvApiCtx, 1) = k;
    }
    else if (nbOutputArgument(pvApiCtx) == 2)
    {
        AssignOutputVariable(pvApiCtx, 1) = k;
        AssignOutputVariable(pvApiCtx, 2) = k + 3;
    }
    else if (nbOutputArgument(pvApiCtx) == 3)
    {
        AssignOutputVariable(pvApiCtx, 1) = k;
        AssignOutputVariable(pvApiCtx, 2) = k + 3;
        AssignOutputVariable(pvApiCtx, 3) = k + 4;
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
