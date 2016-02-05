/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
#include "api_scilab.h"
#include "sci_malloc.h"
#include "gw_fileio.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "expandPathVariable.h"
#include "os_string.h"
#include "fprintfMat.h"

static void freeVar(char** filename, char** expandedFilename, char*** textAdded, int m4n4, char** Format, char** separator);
/*--------------------------------------------------------------------------*/
int sci_fprintfMat(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int *piAddressVarOne = NULL;
    int m1 = 0, n1 = 0;
    int iType1 = 0;

    int *piAddressVarTwo = NULL;
    int m2 = 0, n2 = 0;
    int iType2 = 0;

    fprintfMatError ierr = FPRINTFMAT_ERROR;

    char *filename = NULL;
    char *expandedFilename = NULL;
    char **textAdded = NULL;
    char *Format = NULL;
    double *dValues = NULL;
    char *separator = NULL;
    int m4n4 = 0;
    int i = 0;

    CheckRhs(2, 5);
    CheckLhs(1, 1);

    if (Rhs >= 3)
    {
        int *piAddressVarThree = NULL;
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

        if (getAllocatedSingleString(pvApiCtx, piAddressVarThree, &Format))
        {
            if (Format)
            {
                freeAllocatedSingleString(Format);
            }

            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        Format = os_strdup(DEFAULT_FPRINTFMAT_FORMAT);
    }

    if ( Rhs >= 4 )
    {
        int *piAddressVarFour = NULL;
        int m4 = 0;
        int n4 = 0;

        sciErr = getVarAddressFromPosition(pvApiCtx, 4, &piAddressVarFour);
        if (sciErr.iErr)
        {
            freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarFour) == FALSE ||
                (isScalar(pvApiCtx, piAddressVarFour) == FALSE && isVector(pvApiCtx, piAddressVarFour) == FALSE))
        {
            freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
            Scierror(999, _("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedMatrixOfString(pvApiCtx, piAddressVarFour, &m4, &n4, &textAdded))
        {
            freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 4);
            return 0;
        }

        m4n4 = m4 * n4;
    }

    if (Rhs >= 5)
    {
        int *piAddressVarFive = NULL;
        sciErr = getVarAddressFromPosition(pvApiCtx, 5, &piAddressVarFive);
        if (sciErr.iErr)
        {
            freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
            printError(&sciErr, 0);
            Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 5);
            return 0;
        }

        if (isStringType(pvApiCtx, piAddressVarFive) == 0 ||
                isScalar(pvApiCtx, piAddressVarFive) == 0)
        {
            freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
            Scierror(999, _("%s: Wrong type for input argument #%d: string expected.\n"), fname, 1);
            return 0;
        }

        if (getAllocatedSingleString(pvApiCtx, piAddressVarFive, &separator))
        {
            freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
            return 0;
        }
    }
    else
    {
        separator = os_strdup(DEFAULT_FPRINTFMAT_SEPARATOR);
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 2, &piAddressVarTwo);
    if (sciErr.iErr)
    {
        freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    if (isDoubleType(pvApiCtx, piAddressVarTwo) == 0 || isVarComplex(pvApiCtx, piAddressVarTwo))
    {
        freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
        Scierror(999, _("%s: Wrong type for input argument #%d: Matrix of floating point numbers expected.\n"), fname, 2);
        return 0;
    }

    sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarTwo, &m2, &n2, &dValues);
    if (sciErr.iErr)
    {
        freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 2);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
        printError(&sciErr, 0);
        Scierror(999, _("%s: Can not read input argument #%d.\n"), fname, 1);
        return 0;
    }

    if (isStringType(pvApiCtx, piAddressVarOne) == 0 || isScalar(pvApiCtx, piAddressVarOne) == 0)
    {
        freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
        Scierror(999, _("%s: Wrong size for input argument #%d: string expected.\n"), fname, 1);
        return 0;
    }

    if (getAllocatedSingleString(pvApiCtx, piAddressVarOne, &filename))
    {
        freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
        return 0;
    }

    expandedFilename = expandPathVariable(filename);
    ierr = fprintfMat(expandedFilename, Format, separator, dValues, m2, n2, textAdded, m4n4);

    switch (ierr)
    {
        case FPRINTFMAT_NO_ERROR:
        {
            LhsVar(1) = 0;
            PutLhsVar();
        }
        break;
        case FPRINTFMAT_FOPEN_ERROR:
        {
            Scierror(999, _("%s: can not open file %s.\n"), fname, filename);
        }
        break;
        case FPRINTMAT_FORMAT_ERROR:
        {
            Scierror(999, _("%s: Invalid format.\n"), fname);
        }
        break;
        default:
        case FPRINTFMAT_ERROR:
        {
            Scierror(999, _("%s: error.\n"), fname);
        }
        break;
    }

    freeVar(&filename, &expandedFilename, &textAdded, m4n4, &Format, &separator);
    return 0;
}
/*--------------------------------------------------------------------------*/
static void freeVar(char** filename, char** expandedFilename, char*** textAdded, int m4n4, char** Format, char** separator)
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

    if (textAdded && *textAdded)
    {
        freeArrayOfString(*textAdded, m4n4);
        *textAdded = NULL;
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

