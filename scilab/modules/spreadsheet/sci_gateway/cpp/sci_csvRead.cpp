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
#include <stdio.h>

#include "gw_csv_tools.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
extern "C" {
#include "freeArrayOfString.h"
};
#include "MALLOC.h"
#include "csvRead.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "stringToComplex.h"
#include "csvDefault.h"
#include "csv_complex.h"
#include "gw_csv_helpers.h"
#include "getRange.h"
/* ==================================================================== */
#define CONVTOSTR "string"
#define CONVTODOUBLE "double"
/* ==================================================================== */
/* csvRead(filename, separator, decimal, conversion, substitute, range)*/
/* ==================================================================== */
int sci_csvRead(char *fname)
{
    SciErr sciErr;
    int iErr = 0;

    char *filename = NULL;
    char *separator = NULL;
    char *decimal = NULL;
    char *conversion = NULL;
    int *iRange = NULL;
    int haveRange = 0;

    char **toreplace = NULL;
    int nbElementsToReplace = 0;

    char *regexp = NULL;

    csvResult *result = NULL;

    double *dRealValues = NULL;

    CheckRhs(1, 7);
    CheckLhs(1, 2);

    if (Rhs == 7)
    {
        int m7 = 0, n7 = 0;

        iRange = csv_getArgumentAsMatrixofIntFromDouble(pvApiCtx, 7, fname, &m7, &n7, &iErr);
        if (iErr) return 0;

        if ((m7 * n7 != SIZE_RANGE_SUPPORTED) )
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            Scierror(999, _("%s: Wrong size for input argument #%d: Four entries expected.\n"), fname, 7);
            return 0;
        }

        if ((m7 != 1) && (n7 != 1))
        {
            if (iRange)
            {
                FREE(iRange);
                iRange = NULL;
            }
            Scierror(999, _("%s: Wrong size for input argument #%d: A column or row vector expected.\n"), fname, 7);
            return 0;
        }

        if (isValidRange(iRange, m7 * n7))
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
            Scierror(999, _("%s: Wrong value for input argument #%d: Inconsistent range.\n"), fname, 7);
            return 0;
        }
    }


    if (Rhs >= 6)
    {
        regexp = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 6, fname, getCsvDefaultCommentsRegExp(), &iErr);
        if (regexp)
        {
            if (strcmp(regexp, "") == 0)
            {
                FREE(regexp);
                regexp = NULL;
            }
        }
        if (iErr) return 0;
    }
    else
    {
        regexp = strdup(getCsvDefaultCommentsRegExp());
        if (regexp)
        {
            if (strcmp(regexp, "") == 0)
            {
                FREE(regexp);
                regexp = NULL;
            }
        }
    }

    if (Rhs >= 5)
    {
        if (csv_isEmpty(pvApiCtx, 5))
        {
            toreplace = NULL;
            nbElementsToReplace = 0;
        }
        else
        {
            int m5 = 0, n5 = 0;
            toreplace = csv_getArgumentAsMatrixOfString(pvApiCtx, 5, fname, &m5, &n5, &iErr);
            if (iErr)
            {
                if (regexp)
                {
                    FREE(regexp);
                    regexp = NULL;
                }
                return 0;
            }

            if (n5 != 2)
            {
                if (regexp)
                {
                    FREE(regexp);
                    regexp = NULL;
                }
                freeArrayOfString(toreplace, m5 * n5);
                toreplace = NULL;
                Scierror(999, _("%s: Wrong size for input argument #%d.\n"), fname, 5);
                return 0;
            }
            nbElementsToReplace = m5;
        }
    }
    else
    {
        toreplace = NULL;
        nbElementsToReplace = 0;
    }

    if (Rhs >= 4)
    {
        int iErr = 0;
        conversion = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 4, fname, getCsvDefaultConversion(), &iErr);
        if (iErr)
        {
            FREE(regexp);
            return 0;
        }
        if (!((strcmp(conversion, CONVTOSTR) == 0) || (strcmp(conversion, CONVTODOUBLE) == 0)))
        {
            if (regexp)
            {
                FREE(regexp);
                regexp = NULL;
            }
            if (toreplace)
            {
                freeArrayOfString(toreplace, nbElementsToReplace * 2);
                toreplace = NULL;
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
        /* read_csv is using a 'string' conversion while csvRead is doing
           a 'double' conversion */
        if (strcmp(fname, "read_csv") == 0)
        {
            conversion = (char*)MALLOC((strlen("string") + 1) * sizeof(char));
            strcpy(conversion, "string");
        }
        else
        {
            conversion = strdup(getCsvDefaultConversion());
        }
    }

    if (Rhs >= 3)
    {
        int iErr = 0;
        decimal = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 3, fname, getCsvDefaultDecimal(), &iErr);
        if (iErr)
        {
            if (regexp)
            {
                FREE(regexp);
                regexp = NULL;
            }
            if (conversion)
            {
                FREE(conversion);
                conversion = NULL;
            }
            if (toreplace)
            {
                freeArrayOfString(toreplace, nbElementsToReplace * 2);
                toreplace = NULL;
            }
            return 0;
        }
    }
    else
    {
        decimal = strdup(getCsvDefaultDecimal());
    }

    if (Rhs >= 2)
    {
        int iErr = 0;
        separator = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 2, fname, getCsvDefaultSeparator(), &iErr);
        if (iErr)
        {
            if (regexp)
            {
                FREE(regexp);
                regexp = NULL;
            }
            if (toreplace)
            {
                freeArrayOfString(toreplace, nbElementsToReplace * 2);
                toreplace = NULL;
            }
            if (conversion)
            {
                FREE(conversion);
                conversion = NULL;
            }
            if (decimal)
            {
                FREE(decimal);
                decimal = NULL;
            }
            return 0;
        }
    }
    else
    {
        separator = strdup(getCsvDefaultSeparator());
    }

    if (strcmp(separator, "\\t") == 0)
    {
        /* In Scilab, if the user is providing \t as separator, transform it to a real
           tab. Example: read_csv(filename,"\t");
        */
        strcpy(separator, "\t");
    }


    filename = csv_getArgumentAsString(pvApiCtx, 1, fname, &iErr);
    if (iErr)
    {
        if (regexp)
        {
            FREE(regexp);
            regexp = NULL;
        }
        if (toreplace)
        {
            freeArrayOfString(toreplace, nbElementsToReplace * 2);
            toreplace = NULL;
        }
        if (separator)
        {
            FREE(separator);
            separator = NULL;
        }
        if (conversion)
        {
            FREE(conversion);
            conversion = NULL;
        }
        if (decimal)
        {
            FREE(decimal);
            decimal = NULL;
        }
        return 0;
    }

    result = csvRead(filename, separator, decimal, (const char**)toreplace, nbElementsToReplace * 2, regexp);
    if (regexp)
    {
        FREE(regexp);
        regexp = NULL;
    }
    if (toreplace)
    {
        freeArrayOfString(toreplace, nbElementsToReplace * 2);
        toreplace = NULL;
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

    if (result)
    {
        switch (result->err)
        {
            case CSV_READ_REGEXP_ERROR:
            {
                Scierror(999, _("%s: Wrong value for input argument #%d.\n"), fname, 6);
            }
            break;

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
                        if (filename)
                        {
                            FREE(filename);
                            filename = NULL;
                        }
                        if (conversion)
                        {
                            FREE(conversion);
                            conversion = NULL;
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
                                freeCsvComplexArray(ptrCsvComplexArray);
                                ptrCsvComplexArray = NULL;
                            }
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
                    if (filename)
                    {
                        FREE(filename);
                        filename = NULL;
                    }
                    if (conversion)
                    {
                        FREE(conversion);
                        conversion = NULL;
                    }
                    printError(&sciErr, 0);
                    Scierror(17, _("%s: Memory allocation error.\n"), fname);
                    return 0;
                }
                else
                {
                    LhsVar(1) = Rhs + 1;

                    if (Lhs == 2)
                    {
                        sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, result->nbComments, 1, result->pstrComments);
                        if (sciErr.iErr)
                        {
                            freeCsvResult(result);
                            if (filename)
                            {
                                FREE(filename);
                                filename = NULL;
                            }
                            if (conversion)
                            {
                                FREE(conversion);
                                conversion = NULL;
                            }
                            printError(&sciErr, 0);
                            Scierror(17, _("%s: Memory allocation error.\n"), fname);
                            return 0;
                        }
                        LhsVar(2) = Rhs + 2;
                    }
                    PutLhsVar();
                }
            }
            break;

            case CSV_READ_FILE_NOT_EXIST:
            {
                Scierror(999, _("%s: %s does not exist.\n"), fname, filename);
            }
            break;

            case CSV_READ_MOPEN_ERROR:
            {
                Scierror(999, _("%s: can not open file %s.\n"), fname, filename);
            }
            break;

            case CSV_READ_MEMORY_ALLOCATION:
            {
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
            }
            break;

            case CSV_READ_COLUMNS_ERROR:
            {
                Scierror(999, _("%s: can not read file %s: Error in the column structure\n"), fname, filename);
            }
            break;

            case CSV_READ_READLINES_ERROR:
            case CSV_READ_ERROR:
            {
                Scierror(999, _("%s: can not read file %s.\n"), fname, filename);
            }
            break;
        }
    }
    else
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
    }
    freeCsvResult(result);
    if (filename)
    {
        FREE(filename);
        filename = NULL;
    }
    if (conversion)
    {
        FREE(conversion);
        conversion = NULL;
    }

    return 0;
}
/* ==================================================================== */
