/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - 2012 - INRIA - Allan CORNET
 * Copyright (C) 2011 - INRIA - Michael Baudin
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * This code is also published under the GPL v3 license.
 *
 */
#include <string.h>
#include "gw_spreadsheet.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "stringToComplex.h"
#include "csvDefault.h"
#include "csvRead.h"
#include "getRange.h"
#include "gw_csv_helpers.h"

// =============================================================================
#define CONVTOSTR "string"
#define CONVTODOUBLE "double"
// =============================================================================
int sci_csvTextScan(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iErr = 0;
    int i = 0;

    int *piAddressVarOne = NULL;
    int m1 = 0, n1 = 0;
    int iType1 = 0;

    char **text = NULL;
    int *lengthText = NULL;
    int nbLines = 0;

    char *separator = NULL;
    char *decimal = NULL;
    char *conversion = NULL;

    double * dRealValues = NULL;

    int *iRange = NULL;
    int haveRange = 0;

    csvResult *result = NULL;

    CheckRhs(1, 5);
    CheckLhs(1, 1);

    if (Rhs == 5)
    {
        int m5 = 0, n5 = 0;

        iRange = csv_getArgumentAsMatrixofIntFromDouble(pvApiCtx, 5, fname, &m5, &n5, &iErr);
        if (iErr)
        {
            return 0;
        }

        if ((m5 * n5 != SIZE_RANGE_SUPPORTED) )
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            Scierror(999, _("%s: Wrong size for input argument #%d: Four entries expected.\n"), fname, 5);
            return 0;
        }

        if ((m5 != 1) && (n5 != 1))
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            Scierror(999, _("%s: Wrong size for input argument #%d: A column or row vector expected.\n"), fname, 5);
            return 0;
        }

        if (isValidRange(iRange, m5 * n5))
        {
            haveRange = 1;
        }
        else
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            Scierror(999, _("%s: Wrong value for input argument #%d: Inconsistent range.\n"), fname, 5);
            return 0;
        }
    }

    if (Rhs >= 4)
    {
        conversion = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 4, fname, getCsvDefaultConversion(), &iErr);
        if (iErr)
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            return 0;
        }

        if (!((strcmp(conversion, CONVTOSTR) == 0) || (strcmp(conversion, CONVTODOUBLE) == 0)))
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            if (conversion)
            {
                FREE(conversion);
                conversion = NULL;
            }

            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' string expected.\n"), fname, 4, "double", "string");
            return 0;
        }
    }
    else
    {
        conversion = strdup(getCsvDefaultConversion());
    }

    if (Rhs >= 3)
    {
        decimal = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 3, fname, getCsvDefaultDecimal(), &iErr);
        if (iErr)
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            if (conversion)
            {
                FREE(conversion);
                conversion = NULL;
            }
            return 0;
        }

        if (decimal[0] != '.' && decimal[0] != ',')
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            if (conversion)
            {
                FREE(conversion);
                conversion = NULL;
            }

            Scierror(999, _("%s: Wrong value for input argument #%d: '%s' or '%s' string expected.\n"), fname, 3, ",", ".");
            return 0;
        }
    }
    else
    {
        decimal = strdup(getCsvDefaultDecimal());
    }

    if (Rhs >= 2)
    {
        separator = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 2, fname, getCsvDefaultSeparator(), &iErr);
        if (iErr)
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            if (decimal)
            {
                FREE(decimal);
                decimal = NULL;
            }
            if (conversion)
            {
                FREE(conversion);
                conversion = NULL;
            }
            return 0;
        }
    }
    else
    {
        separator = strdup(getCsvDefaultSeparator());
    }

    if (!csv_isRowVector(pvApiCtx, 1) &&
            !csv_isColumnVector(pvApiCtx, 1) &&
            !csv_isScalar(pvApiCtx, 1))
    {
        if (iRange)
        {
            FREE(iRange);
            iRange = NULL;
        }
        if (separator)
        {
            FREE(separator);
            separator = NULL;
        }
        if (decimal)
        {
            FREE(decimal);
            decimal = NULL;
        }
        if (conversion)
        {
            FREE(conversion);
            conversion = NULL;
        }
        Scierror(999, _("%s: Wrong size for input argument #%d: String vector expected.\n"), fname, 1);
        return 0;
    }

    text = csv_getArgumentAsMatrixOfString(pvApiCtx, 1, fname, &m1, &n1, &iErr);
    if (iErr)
    {
        if (iRange)
        {
            FREE(iRange);
            iRange = NULL;
        }
        if (separator)
        {
            FREE(separator);
            separator = NULL;
        }
        if (decimal)
        {
            FREE(decimal);
            decimal = NULL;
        }
        if (conversion)
        {
            FREE(conversion);
            conversion = NULL;
        }
        return 0;
    }

    nbLines = m1 * n1;
    result = csvTextScan((const char**)text, nbLines, separator, decimal);

    if (text)
    {
        if (separator)
        {
            FREE(separator);
            separator = NULL;
        }
        freeArrayOfString(text, nbLines);
        text = NULL;
    }

    if (separator)
    {
        FREE(separator);
        separator = NULL;
    }

    if (result)
    {
        switch (result->err)
        {
            case CSV_READ_SEPARATOR_DECIMAL_EQUAL:
            {
                Scierror(999, _("%s: separator and decimal must have different values.\n"), fname);
            }
            break;

            case CSV_READ_NO_ERROR:
            {
                if (strcmp(conversion, CONVTOSTR) == 0)
                {
                    if (haveRange)
                    {
                        int newM = 0;
                        int newN = 0;

                        char **pStrRange = getRangeAsString((const char**)result->pstrValues, result->m, result->n, iRange, &newM, &newN);
                        if (pStrRange)
                        {
                            sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, newM, newN, pStrRange);
                            freeArrayOfString(pStrRange, newM * newN);
                        }
                        else
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        }

                    }
                    else
                    {
                        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, result->m, result->n, result->pstrValues);
                    }
                }
                else /* to double */
                {
                    stringToComplexError ierr = STRINGTOCOMPLEX_ERROR;
                    csv_complexArray *ptrCsvComplexArray = stringsToCsvComplexArray((const char**)result->pstrValues, result->m * result->n, decimal, TRUE, &ierr);
                    if (ptrCsvComplexArray == NULL)
                    {
                        freeCsvResult(result);
                        if (decimal)
                        {
                            FREE(decimal);
                            decimal = NULL;
                        }
                        if (conversion)
                        {
                            FREE(conversion);
                            conversion = NULL;
                        }
                        if (iRange)
                        {
                            FREE(iRange);
                            iRange = NULL;
                        }
                        if (ierr == STRINGTOCOMPLEX_ERROR)
                        {
                            Scierror(999, _("%s: can not convert data.\n"), fname);
                        }
                        else
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        }
                        return 0;
                    }

                    switch (ierr)
                    {
                        case STRINGTOCOMPLEX_NOT_A_NUMBER:
                        case STRINGTOCOMPLEX_NO_ERROR:
                        {
                            if (haveRange)
                            {
                                int newM = 0;
                                int newN = 0;
                                csv_complexArray *csvComplexRange = getRangeAsCsvComplexArray(ptrCsvComplexArray, result->m, result->n, iRange, &newM, &newN);
                                if (csvComplexRange)
                                {
                                    if (csvComplexRange->isComplex)
                                    {
                                        sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 1, newM, newN, ptrCsvComplexArray->realPart, ptrCsvComplexArray->imagPart);
                                    }
                                    else
                                    {
                                        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, newM, newN, csvComplexRange->realPart);
                                    }
                                    freeCsvComplexArray(csvComplexRange);
                                    csvComplexRange = NULL;
                                }
                                else
                                {
                                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                }
                            }
                            else
                            {
                                if (ptrCsvComplexArray->isComplex)
                                {
                                    sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 1, result->m, result->n, ptrCsvComplexArray->realPart, ptrCsvComplexArray->imagPart);
                                }
                                else
                                {
                                    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, result->m, result->n, ptrCsvComplexArray->realPart);
                                }
                            }
                            freeCsvComplexArray(ptrCsvComplexArray);
                            ptrCsvComplexArray = NULL;
                        }
                        break;

                        case STRINGTOCOMPLEX_MEMORY_ALLOCATION:
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        }
                        default:
                        case STRINGTOCOMPLEX_ERROR:
                        {
                            Scierror(999, _("%s: can not convert data.\n"), fname);
                        }
                    }
                }

                if (sciErr.iErr)
                {
                    freeCsvResult(result);
                    if (decimal)
                    {
                        FREE(decimal);
                        decimal = NULL;
                    }
                    if (conversion)
                    {
                        FREE(conversion);
                        conversion = NULL;
                    }
                    if (iRange)
                    {
                        FREE(iRange);
                        iRange = NULL;
                    }
                    printError(&sciErr, 0);
                    Scierror(17, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
                else
                {
                    LhsVar(1) = Rhs + 1;
                    PutLhsVar();
                }
            }
            break;

            case CSV_READ_MEMORY_ALLOCATION:
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
            }
            break;

            case CSV_READ_COLUMNS_ERROR:
            {
                Scierror(999, _("%s: can not read text: Error in the column structure\n"), fname);
            }
            break;

            case CSV_READ_READLINES_ERROR:
            case CSV_READ_ERROR:
            {
                Scierror(999, _("%s: can not read text.\n"), fname);
            }
            break;
        }
    }
    else
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
    }
    freeCsvResult(result);
    if (decimal)
    {
        FREE(decimal);
        decimal = NULL;
    }
    if (conversion)
    {
        FREE(conversion);
        conversion = NULL;
    }
    if (iRange)
    {
        FREE(iRange);
        iRange = NULL;
    }

    return 0;
}
// =============================================================================
