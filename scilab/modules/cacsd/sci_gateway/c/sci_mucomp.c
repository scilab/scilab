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
#include "gw_cacsd.h"

#include "doublecomplex.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"

extern int C2F(ab13md)();

//     [bound,D,G] = mucomp(Z,K,T)
//     [bound,D] = mucomp(Z,K,T)
//     bound = mucomp(Z,K,T)

int sci_mucomp(char *fname, void* pvApiCtx)
{
    SciErr sciErr;

    int* piAddrlK   = NULL;
    int* lK         = NULL;
    int* piAddrlT   = NULL;
    int* lT         = NULL;
    double* lBOUND  = NULL;
    double* lD      = NULL;
    double* lG      = NULL;
    double* lX      = NULL;
    int* lIWORK     = NULL;
    double* lRWORK  = NULL;

    int* piAddrlZ               = NULL;
    doublecomplex* lZ           = NULL;
    const doublecomplex* lZWORK = NULL;

    int LRWORK = 0, LZWRKMIN = 0;
    int M  = 0, N  = 0;
    int M1 = 0, N1 = 0;
    int M2 = 0, N2 = 0;
    int INFO = 0;

    int minrhs = 3;
    int maxrhs = 3;
    int minLhs = 1;
    int maxLhs = 3;

    int iOne  = 1;
    int iSize = 0;

    CheckInputArgument(pvApiCtx, minrhs, maxrhs);
    CheckOutputArgument(pvApiCtx, minLhs, maxLhs);

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddrlZ);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 1.
    sciErr = getComplexZMatrixOfDouble(pvApiCtx, piAddrlZ, &M, &N, &lZ);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A complex expected.\n"), fname, 1);
        return 1;
    }

    if (M != N)
    {
        Scierror(999, _("%s: Wrong size for input argument #%d: A square matrix expected.\n"), fname, 2);
        return 1;
    }

    if (N == 0)
    {
        if (nbOutputArgument(pvApiCtx) == 1)
        {
            sciErr = allocMatrixOfDouble(pvApiCtx, 2, N, iOne, &lBOUND);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = 2;
            return 0;
        }
        else if (nbOutputArgument(pvApiCtx) == 2)
        {
            sciErr = allocMatrixOfDouble(pvApiCtx, 2, N, iOne, &lBOUND);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            sciErr = allocMatrixOfDouble(pvApiCtx, 3, N, iOne, &lD);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = 2;
            AssignOutputVariable(pvApiCtx, 2) = 3;
            return 0;
        }
        else if (nbOutputArgument(pvApiCtx) == 3)
        {
            sciErr = allocMatrixOfDouble(pvApiCtx, 2, N, iOne, &lBOUND);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            sciErr = allocMatrixOfDouble(pvApiCtx, 3, N, iOne, &lD);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            sciErr = allocMatrixOfDouble(pvApiCtx, 4, N, iOne, &lG);
            if (sciErr.iErr)
            {
                printError(&sciErr, 0);
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 1;
            }

            AssignOutputVariable(pvApiCtx, 1) = 2;
            AssignOutputVariable(pvApiCtx, 2) = 3;
            AssignOutputVariable(pvApiCtx, 3) = 4;
            return 0;
        }
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddrlK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 2.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrlK, &M1, &N1, &lK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 2);
        return 1;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddrlT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return 1;
    }

    // Retrieve a matrix of double at position 3.
    sciErr = getMatrixOfDoubleAsInteger(pvApiCtx, piAddrlT, &M2, &N2, &lT);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(202, _("%s: Wrong type for argument #%d: A real expected.\n"), fname, 3);
        return 1;
    }

    if (M1 * N1 != M2 * N2)
    {
        Scierror(999, _("%s: Wrong size for input arguments #%d and #%d: Same size expected.\n"), fname, 2);
        return 1;
    }

    M = M1 * N1;

    sciErr = allocMatrixOfDouble(pvApiCtx, 4, iOne, iOne, &lBOUND);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, 5, N, iOne, &lD);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    sciErr = allocMatrixOfDouble(pvApiCtx, 6, N, iOne, &lG);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    iSize = 2 * N - 1;
    sciErr = allocMatrixOfDouble(pvApiCtx, 7, iSize, iOne, &lX);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    iSize = 4 * N - 2;
    sciErr = allocMatrixOfDoubleAsInteger(pvApiCtx, 8, iSize, iOne, &lIWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    LRWORK = 2 * N * N * N + 9 * N * N +  44 * N - 11;
    sciErr = allocMatrixOfDouble(pvApiCtx, 9, LRWORK, iOne, &lRWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    LZWRKMIN = 6 * N * N * N + 12 * N * N + 12 * N - 3;
    sciErr = allocComplexZMatrixOfDouble(pvApiCtx, 10, iOne, LZWRKMIN, &lZWORK);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 1;
    }

    C2F(ab13md)("N", &N, lZ, &N, &M, lK, lT,
                lX, lBOUND, lD, lG, lIWORK,
                lRWORK, &LRWORK, lZWORK, &LZWRKMIN, &INFO);

    if (INFO != 0)
    {
        Scierror(999, _("%s: AB13MD exit with info = %d.\n"), fname, INFO);
        return 1;
    }

    if (nbOutputArgument(pvApiCtx) == 1)
    {
        AssignOutputVariable(pvApiCtx, 1) = 4;
    }
    else if (nbOutputArgument(pvApiCtx) == 2)
    {
        AssignOutputVariable(pvApiCtx, 1) = 4;
        AssignOutputVariable(pvApiCtx, 2) = 5;
    }
    else if (nbOutputArgument(pvApiCtx) == 3)
    {
        AssignOutputVariable(pvApiCtx, 1) = 4;
        AssignOutputVariable(pvApiCtx, 2) = 5;
        AssignOutputVariable(pvApiCtx, 3) = 6;
    }

    ReturnArguments(pvApiCtx);
    return 0;
}
