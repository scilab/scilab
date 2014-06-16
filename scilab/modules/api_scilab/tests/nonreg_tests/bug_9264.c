// =============================================================================
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) 2011 - Calixte DENIZET
//
//  This file is distributed under the same license as the Scilab package.
// =============================================================================

#include <stdlib.h>
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "localization.h"

// =============================================================================
// The aim of this function is just to retrieve from the stack an array of strings
// to test the speed of the function getMatrixOfString.
// =============================================================================
int sci_bug9264(char *fname, void* pvApiCtx)
{
    SciErr err;
    int *piAddressVarOne = NULL;
    int rows = 0, cols = 0;

    CheckInputArgument(pvApiCtx, 1, 1);
    CheckOutputArgument(pvApiCtx, 1, 1);

    err = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (err.iErr)
    {
        printError(&err, 0);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne))
    {
        int i           = 0;
        int size        = 0;
        int* lengths    = NULL;
        char **str      = NULL;

        err = getVarDimension(pvApiCtx, piAddressVarOne, &rows, &cols);
        if (err.iErr)
        {
            printError(&err, 0);
            return -1;
        }

        if (rows < 1 || cols < 1)
        {
            Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 1);
            return 0;
        }

        size = rows * cols;
        lengths = (int*)MALLOC(sizeof(int) * size);
        if (!lengths)
        {
            Scierror(999, _("%s: No more memory.\n"), fname);
            return 0;
        }

        err = getMatrixOfString(pvApiCtx, piAddressVarOne, &rows, &cols, lengths, NULL);
        if (err.iErr)
        {
            FREE(lengths);
            printError(&err, 0);
            return 0;
        }

        str = (char**)MALLOC(sizeof(char*) * size);
        for (; i < size; i++)
        {
            str[i] = (char*)MALLOC(sizeof(char) * (lengths[i] + 1));
            if (str[i] == NULL)
            {
                freeArrayOfString(str, i);
                Scierror(999, _("%s: No more memory.\n"), fname);
                return 0;
            }
        }

        err = getMatrixOfString(pvApiCtx, piAddressVarOne, &rows, &cols, lengths, str);

        FREE(lengths);
        freeArrayOfString(str, size);
    }

    AssignOutputVariable(pvApiCtx, 1) = 0;
    return 0;
}
