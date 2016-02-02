/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - 2010-2010 - Clément DAVID <clement.david@scilab.org>
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

extern "C"
{
#include "api_scilab.h"
#include "localization.h"
#include "Scierror.h"
#include "sci_malloc.h"
}

/**
 * Read a single boolean on the stack.
 *
 * @param pvApiCtx private api context (opaque structure)
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readSingleBoolean(void* pvApiCtx, int rhsPosition, bool* out, const char* fname)
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
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    sciErr = getMatrixOfBoolean(pvApiCtx, argumentPointer, &rowsArgument, &colsArgument, NULL);

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    if (rowsArgument != 1 || colsArgument != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: A boolean expected.\n"), fname, rhsPosition);
        return -1;
    }

    sciErr = getMatrixOfBoolean(pvApiCtx, argumentPointer, &rowsArgument, &colsArgument, &value);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    *out = (*value == 0);
    return 0;
}

/**
 * Read a single string on the stack.
 *
 * @param pvApiCtx private api context (opaque structure)
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readSingleString(void* pvApiCtx, int rhsPosition, char** out, const char* fname)
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
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument, &colsArgument, NULL, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    if (rowsArgument != 1 || colsArgument != 1)
    {
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, rhsPosition);
        return -1;
    }

    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument, &colsArgument, &lenArgument, NULL);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    value = (char*) MALLOC(sizeof(char) * (lenArgument + 1)); //+ 1 for null termination
    value[lenArgument] = '\0';
    sciErr = getMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument, &colsArgument, &lenArgument, &value);
    if (sciErr.iErr)
    {
        FREE(value);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    *out = value;
    return 0;
}

/**
 * Read a vector of string on the stack.
 *
 * @param pvApiCtx private api context (opaque structure)
 * @param rhsPosition the position on the stack.
 * @param[out] out the read value.
 * @param[out] vectorLength the length of the vector.
 * @param fname the function name used for the call.
 * @return status of the operation (<> 0 on error)
 */
int readVectorString(void* pvApiCtx, int rhsPosition, char*** out, int* vectorLength, char* fname)
{
    int* argumentPointer = NULL;
    int rowsArgument = 0;
    int colsArgument = 0;

    SciErr sciErr;

    sciErr = getVarAddressFromPosition(pvApiCtx, rhsPosition, &argumentPointer);
    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, rhsPosition);
        return -1;
    }

    if (getAllocatedMatrixOfString(pvApiCtx, argumentPointer, &rowsArgument, &colsArgument, out))
    {
        return -1;
    }
    *vectorLength = rowsArgument * colsArgument;

    return 0;
}

void releaseVectorString(char** str, int len)
{
    for (int i = 0; i < len; ++i)
    {
        FREE(str[i]);
    }
    FREE(str);
}

