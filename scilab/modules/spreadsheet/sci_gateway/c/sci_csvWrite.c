/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
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
 * This code is also published under the GPL v3 license.
 *
 */
#include <string.h>
#include <stdio.h>

#include "gw_spreadsheet.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "csvWrite.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "csvDefault.h"
#include "checkCsvWriteFormat.h"
#include "gw_csv_helpers.h"
#include "os_string.h"


static void freeVar(char** separator, char** decimal, char** filename, char** precisionFormat, char*** pHeadersLines, int sizeHeader);
// =============================================================================
// csvWrite(M, filename[, separator, decimal, precision]) */
// with M string or double (not complex)
// =============================================================================
int sci_csvWrite(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iErr = 0;
    csvWriteError csvError = CSV_WRITE_ERROR;

    char *separator = NULL;
    char *decimal = NULL;
    char *filename = NULL;
    char *precisionFormat = NULL;
    char **pHeadersLines = NULL;
    int nbHeadersLines = 0;

    char **pStringValues = NULL;
    double *pDoubleValuesReal = NULL;
    double *pDoubleValuesImag = NULL;
    int bIsComplex = 0;
    int mValues = 0;
    int nValues = 0;

    int *piAddressVarTwo = NULL;
    int m2 = 0, n2 = 0;
    int iType2 = 0;

    int *piAddressVarOne = NULL;
    int m1 = 0, n1 = 0;
    int iType1 = 0;

    CheckRhs(2, 6);
    CheckLhs(1, 1);

    if (Rhs > 5)
    {
        int isOnlyRowOrCol = 0;
        int m6 = 0;
        int n6 = 0;
        pHeadersLines = csv_getArgumentAsMatrixOfString(pvApiCtx, 6, fname, &m6, &n6, &iErr);
        if (iErr)
        {
            freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
            return 0;
        }
        isOnlyRowOrCol = ((m6 > 1) && (n6 == 1)) || ((m6 == 1) && (n6 > 1)) || ((m6 == 1) && (n6 == 1));
        if (!isOnlyRowOrCol)
        {
            freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
            Scierror(999, _("%s: Wrong size for input argument #%d: A 1-by-n or m-by-1 array of strings expected.\n"), fname, 6);
            return 0;
        }
        nbHeadersLines = m6 * n6;
    }

    if (Rhs > 4)
    {
        if (csv_isDoubleScalar(pvApiCtx, 5))
        {
#define FORMAT_FIELDVALUESTR "%%.%dlg"
            int iFormatValue = (int) csv_getArgumentAsScalarDouble(pvApiCtx, 5, fname, &iErr);
            if (iErr)
            {
                freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
                return 0;
            }

            if ((iFormatValue < 1) || (iFormatValue > 17))
            {
                Scierror(999, _("%s: Wrong value for input argument #%d: A double (value 1 to 17) expected.\n"), fname, 5);
                freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
                return 0;
            }

            precisionFormat = (char*)MALLOC(sizeof(char) * ((int)strlen(FORMAT_FIELDVALUESTR) + 1));
            if (precisionFormat == NULL)
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
                return 0;
            }
            sprintf(precisionFormat, FORMAT_FIELDVALUESTR, iFormatValue);
        }
        else
        {
            precisionFormat = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 5, fname, getCsvDefaultPrecision(), &iErr);
            if (iErr)
            {
                freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
                return 0;
            }

            if (checkCsvWriteFormat(precisionFormat))
            {
                Scierror(999, _("%s: Not supported format %s.\n"), fname, precisionFormat);
                freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
                return 0;
            }
        }
    }
    else
    {
        precisionFormat = os_strdup(getCsvDefaultPrecision());
    }

    if (Rhs > 3)
    {
        decimal = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 4, fname, getCsvDefaultDecimal(), &iErr);
        if (iErr)
        {
            freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
            return 0;
        }

        if (strcmp(decimal, ".") && strcmp(decimal, ","))
        {
            //invalid value
            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' expected.\n"), "write_csv", 4, ".", ",");
            freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
            return 1;
        }
    }
    else
    {
        decimal = os_strdup(getCsvDefaultDecimal());
    }

    if (Rhs > 2)
    {
        separator = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 3, fname, getCsvDefaultSeparator(), &iErr);
        if (iErr)
        {
            freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
            return 0;
        }
    }
    else
    {
        separator = os_strdup(getCsvDefaultSeparator());
    }

    filename = csv_getArgumentAsString(pvApiCtx, 2, fname, &iErr);
    if (iErr)
    {
        freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
        return 0;
    }

    sciErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddressVarOne);
    if (sciErr.iErr)
    {
        freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
        printError(&sciErr, 0);
        return 0;
    }

    sciErr = getVarType(pvApiCtx, piAddressVarOne, &iType1);
    if (sciErr.iErr)
    {
        freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
        printError(&sciErr, 0);
        return 0;
    }

    if (iType1 == sci_strings)
    {
        pStringValues = csv_getArgumentAsMatrixOfString(pvApiCtx, 1, fname, &m1, &n1, &iErr);
        if (iErr)
        {
            freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
            return 0;
        }
    }
    else if (iType1 == sci_matrix)
    {
        if (isVarComplex(pvApiCtx, piAddressVarOne))
        {
            bIsComplex = 1;
            sciErr = getComplexMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &pDoubleValuesReal, &pDoubleValuesImag);
        }
        else
        {
            sciErr = getMatrixOfDouble(pvApiCtx, piAddressVarOne, &m1, &n1, &pDoubleValuesReal);
        }

        if (sciErr.iErr)
        {
            freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
            printError(&sciErr, 0);
            return 0;
        }
    }
    else
    {
        freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
        Scierror(999, _("%s: Wrong type for input argument #%d: A matrix of string or a matrix of real expected.\n"), fname, 1);
        return 0;
    }

    if (pStringValues)
    {
        csvError = csvWrite_string(filename,
                                   (const char**)pStringValues, m1, n1,
                                   separator,
                                   decimal,
                                   (const char**)pHeadersLines, nbHeadersLines);
        freeAllocatedMatrixOfString(m1, n1, pStringValues);
    }
    else
    {
        if (bIsComplex)
        {
            csvError = csvWrite_complex(filename,
                                        pDoubleValuesReal,
                                        pDoubleValuesImag,
                                        m1, n1,
                                        separator,
                                        decimal,
                                        precisionFormat,
                                        (const char**)pHeadersLines, nbHeadersLines);
        }
        else
        {
            csvError = csvWrite_double(filename,
                                       pDoubleValuesReal, m1, n1,
                                       separator,
                                       decimal,
                                       precisionFormat,
                                       (const char**)pHeadersLines, nbHeadersLines);
        }
    }

    switch (csvError)
    {
        case CSV_WRITE_SEPARATOR_DECIMAL_EQUAL:
        {
            Scierror(999, _("%s: separator and decimal must have different values.\n"), fname);
        }
        break;
        case CSV_WRITE_NO_ERROR:
        {
            LhsVar(1) = 0;
            PutLhsVar();
        }
        break;

        case CSV_WRITE_FOPEN_ERROR:
        {
            Scierror(999, _("%s: can not open file %s.\n"), fname, filename);
        }
        break;
        default:
        case CSV_WRITE_ERROR:
        {
            Scierror(999, _("%s: error.\n"), fname);
        }
        break;
    }

    freeVar(&separator, &decimal, &filename, &precisionFormat, &pHeadersLines, nbHeadersLines);
    return 0;
}
// =============================================================================
static void freeVar(char** separator, char** decimal, char** filename, char** precisionFormat, char*** pHeadersLines, int sizeHeader)
{
    if (separator && *separator)
    {
        FREE(*separator);
        *separator = NULL;
    }

    if (decimal && *decimal)
    {
        FREE(*decimal);
        *decimal = NULL;
    }

    if (filename && *filename)
    {
        FREE(*filename);
        *filename = NULL;
    }

    if (precisionFormat && *precisionFormat)
    {
        FREE(*precisionFormat);
        *precisionFormat = NULL;
    }

    if (pHeadersLines && *pHeadersLines)
    {
        freeArrayOfString(*pHeadersLines, sizeHeader);
        *pHeadersLines = NULL;
    }

}

