/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2010 - 2011 - DIGITEO - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "api_scilab.h"
#include "sci_malloc.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#include "os_string.h"
#include "fscanfMat.h"

/*--------------------------------------------------------------------------*/
static void freeVar(char** filename, char** expandedFilename, char** Format, char** separator);
/*--------------------------------------------------------------------------*/
int sci_fscanfMat(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int m1 = 0, n1 = 0;
    int iType1 = 0;

    char *filename = NULL;
    char *expandedFilename = NULL;
    char *Format = NULL;
    char *separator = NULL;
    BOOL bIsDefaultSeparator = TRUE;

    fscanfMatResult *results = NULL;

    CheckRhs(1, 3);
    CheckLhs(1, 2);

    if (Rhs == 3)
    {
        int *piAddressVarThree = NULL;
        int m3 = 0, n3 = 0;
        int iType3 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 3, &piAddressVarThree);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 3);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarThree) == 0 || isScalar(pvApiCtx, piAddressVarThree) == 0)
        {
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 3);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &separator))
        {
            freeVar(&filename, &expandedFilename, &Format, &separator);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }

        bIsDefaultSeparator = FALSE;
    }

    if (Rhs >= 2)
    {
        int *piAddressVarTwo = NULL;
        int m2 = 0, n2 = 0;
        int iType2 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
        if (sciErr.iErr)
        {
            freeVar(&filename, &expandedFilename, &Format, &separator);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        sciErr = getVarType(pvApiCtx, piAddressVarTwo, &iType2);
        if (sciErr.iErr)
        {
            freeVar(&filename, &expandedFilename, &Format, &separator);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarTwo) == 0 || isScalar(pvApiCtx, piAddressVarTwo) == 0)
        {
            freeVar(&filename, &expandedFilename, &Format, &separator);
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 2);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarTwo, &Format))
        {
            freeVar(&filename, &expandedFilename, &Format, &separator);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Format = os_strdup(DEFAULT_FSCANFMAT_FORMAT);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        freeVar(&filename, &expandedFilename, &Format, &separator);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        freeVar(&filename, &expandedFilename, &Format, &separator);
        Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &filename))
    {
        freeVar(&filename, &expandedFilename, &Format, &separator);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    expandedFilename = expandPathVariable(filename);
    if (bIsDefaultSeparator)
    {
#define NB_DEFAULT_SUPPORTED_SEPARATORS 2

        /* bug 8148 */
        /* default separator can be a space or a tabulation */
        char *supportedSeparators[NB_DEFAULT_SUPPORTED_SEPARATORS] = {DEFAULT_FSCANFMAT_SEPARATOR, "\t"};
        int i = 0;

        for (i = 0; i < NB_DEFAULT_SUPPORTED_SEPARATORS; i++)
        {
            if (results)
            {
                freeFscanfMatResult(results);
            }
            results = fscanfMat(expandedFilename, Format, supportedSeparators[i]);
            if (results && results->err == FSCANFMAT_NO_ERROR)
            {
                break;
            }
        }
    }
    else
    {
        results = fscanfMat(expandedFilename, Format, separator);
        if (results && results->err != FSCANFMAT_NO_ERROR)
        {
            freeFscanfMatResult(results);
            results = NULL;
        }
    }

    if (results == NULL)
    {
        freeVar(&filename, &expandedFilename, &Format, &separator);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    freeVar(NULL, &expandedFilename, &Format, &separator);
    switch (results->err)
    {
        case FSCANFMAT_NO_ERROR:
        {
            if ( (results->values) && (results->m > 0) && (results->n > 0))
            {
                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, results->m, results->n, results->values);
                if (sciErr.iErr)
                {
                    FREE(filename);
                    freeFscanfMatResult(results);
                    printError(&sciErr, 0);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }
            else
            {
                if (createEmptyMatrix(pvApiCtx, Rhs + 1))
                {
                    FREE(filename);
                    freeFscanfMatResult(results);
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
            }

            LhsVar(1) = Rhs + 1;

            if (Lhs == 2)
            {
                if (results->text)
                {
                    sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, results->sizeText, 1, (char const * const*) results->text);
                    if (sciErr.iErr)
                    {
                        FREE(filename);
                        freeFscanfMatResult(results);
                        printError(&sciErr, 0);
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        return 0;
                    }
                }
                else
                {
                    if (createSingleString(pvApiCtx, Rhs + 2, ""))
                    {
                        FREE(filename);
                        freeFscanfMatResult(results);
                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        return 0;
                    }
                }

                LhsVar(2) = Rhs + 2;
            }

            freeFscanfMatResult(results);
            FREE(filename);
            PutLhsVar();
            return 0;
        }
        case FSCANFMAT_MOPEN_ERROR:
        {
            Scierror(999, _("%s: can not open file %s.\n"), fname, filename);
            FREE(filename);
            return 0;
        }
        case FSCANFMAT_READLINES_ERROR:
        {
            Scierror(999, _("%s: can not read file %s.\n"), fname, filename);
            FREE(filename);
            return 0;
        }
        case FSCANFMAT_FORMAT_ERROR:
        {
            Scierror(999, _("%s: Invalid format.\n"), fname);
            FREE(filename);
            return 0;
        }
        case FSCANFMAT_MEMORY_ALLOCATION:
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            FREE(filename);
            return 0;
        }
        default:
        case FSCANFMAT_ERROR:
        {
            Scierror(999, _("%s: error.\n"), fname);
            FREE(filename);
            return 0;
        }
    }
}
/*--------------------------------------------------------------------------*/
static void freeVar(char** filename, char** expandedFilename, char** Format, char** separator)
{
    if (filename && *filename)
    {
        FREE(*filename);
        *filename = NULL;
    }

    if (expandedFilename && *expandedFilename)
    {
        FREE(*expandedFilename);
        *expandedFilename = NULL;
    }

    if (Format && *Format)
    {
        FREE(*Format);
        *Format = NULL;
    }

    if (separator && *separator)
    {
        FREE(*separator);
        *separator = NULL;
    }
}
