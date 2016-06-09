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
#include "localization.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "csvRead.h"
#include "stringToComplex.h"
#include "csvDefault.h"
#include "complex_array.h"
#include "gw_csv_helpers.h"
#include "getRange.h"
#include "os_string.h"

static void freeVar(char** filename, char** separator, char** decimal, char** conversion, int** iRange, char*** toreplace, int sizeReplace, char** regexp);
/* ==================================================================== */
#define CONVTOSTR "string"
#define CONVTODOUBLE "double"
/* ==================================================================== */
/* csvRead(filename, separator, decimal, conversion, substitute, range)*/
/* ==================================================================== */
int sci_csvRead(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iErr = 0;
    int iErrEmpty = 0;

    char *filename = NULL;
    char *separator = NULL;
    char *decimal = NULL;
    char *conversion = NULL;
    int *iRange = NULL;
    int haveRange = 0;
    int header = 0;

    char **toreplace = NULL;
    int nbElementsToReplace = 0;

    char *regexp = NULL;
    int haveRegexp = 0;

    csvResult *result = NULL;

    double *dRealValues = NULL;

    sciErr.iErr = 0;

    CheckRhs(1, 8);
    CheckLhs(1, 2);

    if (Rhs == 8)
    {
        header = (int) csv_getArgumentAsScalarDouble(pvApiCtx, 8, fname, &iErr);
        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, 0, &regexp);
            return 0;
        }
    }

    if (Rhs == 7)
    {
        int m7 = 0, n7 = 0;

        iRange = csv_getArgumentAsMatrixofIntFromDouble(pvApiCtx, 7, fname, &m7, &n7, &iErr);
        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, 0, &regexp);
            return 0;
        }

        if ((m7 * n7 != SIZE_RANGE_SUPPORTED) )
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, 0, &regexp);
            Scierror(999, _("%s: Wrong size for input argument #%d: Four entries expected.\n"), fname, 7);
            return 0;
        }

        if ((m7 != 1) && (n7 != 1))
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, 0, &regexp);
            Scierror(999, _("%s: Wrong size for input argument #%d: A column or row vector expected.\n"), fname, 7);
            return 0;
        }

        if (isValidRange(iRange, m7 * n7))
        {
            haveRange = 1;
        }
        else
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, 0, &regexp);
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
            else
            {
                haveRegexp = 1;
            }
        }

        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, 0, &regexp);
            return 0;
        }
    }
    else
    {
        regexp = os_strdup(getCsvDefaultCommentsRegExp());
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
                freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, m5 * n5, &regexp);
                return 0;
            }

            if (n5 != 2)
            {
                freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, m5 * n5, &regexp);
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
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
            return 0;
        }

        if (!((strcmp(conversion, CONVTOSTR) == 0) || (strcmp(conversion, CONVTODOUBLE) == 0)))
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
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
            conversion = os_strdup(getCsvDefaultConversion());
        }
    }

    if (Rhs >= 3)
    {
        int iErr = 0;
        decimal = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 3, fname, getCsvDefaultDecimal(), &iErr);
        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
            return 0;
        }
    }
    else
    {
        decimal = os_strdup(getCsvDefaultDecimal());
    }

    if (Rhs >= 2)
    {
        int iErr = 0;
        separator = csv_getArgumentAsStringWithEmptyManagement(pvApiCtx, 2, fname, getCsvDefaultSeparator(), &iErr);
        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
            return 0;
        }
    }
    else
    {
        separator = os_strdup(getCsvDefaultSeparator());
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
        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
        return 0;
    }

    result = csvRead(filename, separator, decimal, (const char**)toreplace, nbElementsToReplace * 2, regexp, header);
    freeVar(NULL, &separator, &decimal, NULL, NULL, &toreplace, nbElementsToReplace, &regexp);

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
                            if ((newM == 0) || (newN == 0))
                            {
                                Scierror(999, _("%s: Range row or/and column left indice(s) out of bounds.\n"), fname);
                            }
                            else
                            {
                                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            }

                            freeCsvResult(result);
                            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
                            return 0;
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
                    complexArray *ptrComplexArray = stringsToComplexArray((const char**)result->pstrValues, result->m * result->n, decimal, TRUE, &ierr);

                    if (ptrComplexArray == NULL)
                    {
                        freeCsvResult(result);
                        if (ierr == STRINGTOCOMPLEX_ERROR)
                        {
                            Scierror(999, _("%s: can not convert data.\n"), fname);
                        }
                        else
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                        }

                        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
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
                                complexArray *complexRange = getRangeAsComplexArray(ptrComplexArray, result->m, result->n, iRange, &newM, &newN);
                                if (complexRange)
                                {
                                    if (complexRange->isComplex)
                                    {
                                        sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 1, newM, newN, ptrComplexArray->realPart, ptrComplexArray->imagPart);
                                    }
                                    else
                                    {
                                        sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, newM, newN, complexRange->realPart);
                                    }
                                    freeComplexArray(complexRange);
                                    complexRange = NULL;
                                }
                                else
                                {
                                    if ((newM == 0) || (newN == 0))
                                    {
                                        Scierror(999, _("%s: Range row or/and column left indice(s) out of bounds.\n"), fname);
                                    }
                                    else
                                    {
                                        Scierror(999, _("%s: Memory allocation error.\n"), fname);
                                    }

                                    freeCsvResult(result);
                                    freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
                                    freeComplexArray(ptrComplexArray);
                                    ptrComplexArray = NULL;
                                    return 0;
                                }
                            }
                            else
                            {
                                if (ptrComplexArray->isComplex)
                                {
                                    sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 1, result->m, result->n, ptrComplexArray->realPart, ptrComplexArray->imagPart);
                                }
                                else
                                {
                                    sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, result->m, result->n, ptrComplexArray->realPart);
                                }
                            }
                            freeComplexArray(ptrComplexArray);
                            ptrComplexArray = NULL;
                        }
                        break;

                        case STRINGTOCOMPLEX_MEMORY_ALLOCATION:
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            freeCsvResult(result);
                            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
                            return 0;
                        }
                        default:
                        case STRINGTOCOMPLEX_ERROR:
                        {
                            Scierror(999, _("%s: can not convert data.\n"), fname);
                            freeCsvResult(result);
                            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
                            return 0;
                        }
                    }
                }

                if (sciErr.iErr)
                {
                    Scierror(999, _("%s: Memory allocation error.\n"), fname);
                    freeCsvResult(result);
                    freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
                    return 0;
                }
                else
                {
                    LhsVar(1) = Rhs + 1;

                    if (Lhs == 2)
                    {
                        if (haveRegexp == 0)
                        {
                            char **emptyStringMatrix = NULL;
                            emptyStringMatrix = (char**)MALLOC(sizeof(char*));
                            emptyStringMatrix[0] = "";
                            sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, 1, 1, emptyStringMatrix);
                            FREE(emptyStringMatrix);
                        }
                        else
                        {
                            if (result->nbComments > 0)
                            {
                                sciErr = createMatrixOfString(pvApiCtx, Rhs + 2, result->nbComments, 1, result->pstrComments);
                            }
                            else
                            {
                                iErrEmpty = createEmptyMatrix(pvApiCtx, Rhs + 2);
                                sciErr.iErr = iErrEmpty;
                            }
                        }
                        if (sciErr.iErr)
                        {
                            Scierror(999, _("%s: Memory allocation error.\n"), fname);
                            freeCsvResult(result);
                            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);
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

    freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace, &regexp);

    return 0;
}
/* ==================================================================== */
static void freeVar(char** filename, char** separator, char** decimal, char** conversion, int** iRange, char*** toreplace, int sizeReplace, char** regexp)
{
    if (filename && *filename)
    {
        FREE(*filename);
        *filename = NULL;
    }

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

    if (conversion && *conversion)
    {
        FREE(*conversion);
        *conversion = NULL;
    }

    if (iRange && *iRange)
    {
        FREE(*iRange);
        *iRange = NULL;
    }

    if (toreplace && *toreplace)
    {
        freeArrayOfString(*toreplace, sizeReplace);
        *toreplace = NULL;
    }

    if (regexp && *regexp)
    {
        FREE(*regexp);
        *regexp = NULL;
    }
}
