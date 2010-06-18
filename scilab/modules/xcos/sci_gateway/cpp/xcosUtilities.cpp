/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

extern "C"
{
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "MALLOC.h"
}

/**
 * Read a single boolean on the stack.
 *
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readSingleBoolean(int rhsPosition, bool* out, const char* fname)
{
    int* argumentPointer = NULL;
    int rowsArgument = 0;
    int colsArgument = 0;
    int* value = NULL;

    *out = false;
    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, rhsPosition, &argumentPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    sciErr = getMatrixOfBoolean(pvApiCtx, argumentPointer,
            &rowsArgument, &colsArgument, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    if (rowsArgument != 1 || colsArgument != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, rhsPosition);
        return -1;
    }

    sciErr = getMatrixOfBoolean(pvApiCtx, argumentPointer,
            &rowsArgument, &colsArgument, &value);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    *out = (*value == 0);
    return 0;
}

/**
 * Read a single string on the stack.
 *
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readSingleString(int rhsPosition, char** out, const char* fname)
{
    int* argumentPointer = NULL;
    int rowsArgument = 0;
    int colsArgument = 0;
    int lenArgument = 0;
    char* value = NULL;

    *out = NULL;
    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, rhsPosition, &argumentPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument,
            &colsArgument, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    if (rowsArgument != 1 || colsArgument != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string expected.\n"), fname, rhsPosition);
        return -1;
    }

    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument,
            &colsArgument, &lenArgument, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    value = (char*) MALLOC(sizeof(char) * (lenArgument + 1)); //+ 1 for null termination
    value[lenArgument] = '\0';
    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument,
            &colsArgument, &lenArgument, &value);
    if (sciErr.iErr)
    {
        FREE(value);
        printError(&sciErr, 0);
        return -1;
    }

    *out = value;
    return 0;
}

/**
 * Read a single string on the stack.
 *
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param[out] vectorLength the length of the vector.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readVectorString(int rhsPosition, char*** out, int* vectorLength, char* fname)
{
    int* argumentPointer = NULL;
    int rowsArgument = 0;
    int colsArgument = 0;
    int* lenArgument = 0;
    char** value = NULL;

    *out = NULL;
    *vectorLength = 0;
    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, rhsPosition, &argumentPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument,
            &colsArgument, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    if (rowsArgument != 1 && colsArgument != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A string vector expected.\n"), fname, rhsPosition);
        return -1;
    }

    lenArgument = (int*) MALLOC(sizeof(int) * rowsArgument * colsArgument);
    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument,
            &colsArgument, lenArgument, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        return -1;
    }

    value = (char**) MALLOC(sizeof(char*) * rowsArgument * colsArgument);
    for (int i = 0; i < rowsArgument * colsArgument; ++i)
    {
        value[i] = (char*) MALLOC(sizeof(char) * (lenArgument[i] + 1)); // +1 for null termination
        value[lenArgument[i]] = '\0';
    }

    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument,
            &colsArgument, lenArgument, value);
    if (sciErr.iErr)
    {
        FREE(lenArgument);
        for (int i = 0; i < rowsArgument * colsArgument; ++i)
        {
            FREE(value[i]);
        }
        FREE(value);
        printError(&sciErr, 0);
        return -1;
    }

    *out = value;
    *vectorLength = rowsArgument * colsArgument;
    
    return 0;
}
