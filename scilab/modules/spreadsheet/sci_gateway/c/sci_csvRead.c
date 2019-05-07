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
#include <wchar.h>
#include "gw_spreadsheet.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "charEncoding.h"
#include "freeArrayOfString.h"
#include "sci_malloc.h"
#include "csvRead.h"
#include "stringToComplex.h"
#include "csvDefault.h"
#include "complex_array.h"
#include "gw_csv_helpers.h"
#include "getRange.h"
#include "os_string.h"

static void freeVar(wchar_t** filename, wchar_t** separator, wchar_t** decimal, wchar_t** conversion, int** iRange, wchar_t*** toreplace, int sizeReplace, wchar_t** regexp);
/* ==================================================================== */
#define CONVTOSTR L"string"
#define CONVTODOUBLE L"double"
/* ==================================================================== */
/* csvRead(filename, separator, decimal, conversion, substitute, range)*/
/* ==================================================================== */
int sci_csvRead(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iErr = 0;
    int iErrEmpty = 0;

    wchar_t *filename = NULL;
    wchar_t *separator = NULL;
    wchar_t *decimal = NULL;
    wchar_t *conversion = NULL;
    int *iRange = NULL;
    int haveRange = 0;
    int header = 0;

    wchar_t **toreplace = NULL;
    int nbElementsToReplace = 0;

    wchar_t *regexp = NULL;
    int haveRegexp = 0;

    csvResult *result = NULL;
    sciErr.iErr = 0;

    CheckRhs(1, 8);
    CheckLhs(0, 2);

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
        regexp = csv_getArgumentAsWideStringWithEmptyManagement(pvApiCtx, 6, fname, getCsvDefaultCommentsRegExp(), &iErr);
        if (regexp)
        {
            if (wcscmp(regexp, L"") == 0)
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
        regexp = to_wide_string(getCsvDefaultCommentsRegExp());
        if (regexp)
        {
            if (wcscmp(regexp, L"") == 0)
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
            toreplace = csv_getArgumentAsMatrixOfWideString(pvApiCtx, 5, fname, &m5, &n5, &iErr);
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
        conversion = csv_getArgumentAsWideStringWithEmptyManagement(pvApiCtx, 4, fname, getCsvDefaultConversion(), &iErr);
        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
            return 0;
        }

        if (!((wcscmp(conversion, CONVTOSTR) == 0) || (wcscmp(conversion, CONVTODOUBLE) == 0)))
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
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
            conversion = to_wide_string("string");
        }
        else
        {
            conversion = to_wide_string(getCsvDefaultConversion());
        }
    }

    if (Rhs >= 3)
    {
        int iErr = 0;
        decimal = csv_getArgumentAsWideStringWithEmptyManagement(pvApiCtx, 3, fname, getCsvDefaultDecimal(), &iErr);
        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
            return 0;
        }
    }
    else
    {
        decimal = to_wide_string(getCsvDefaultDecimal());
    }

    if (Rhs >= 2)
    {
        int iErr = 0;
        separator = csv_getArgumentAsWideStringWithEmptyManagement(pvApiCtx, 2, fname, getCsvDefaultSeparator(), &iErr);
        if (iErr)
        {
            freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
            return 0;
        }
    }
    else
    {
        separator = to_wide_string(getCsvDefaultSeparator());
    }

    if (wcscmp(separator, L"\\t") == 0)
    {
        /* In Scilab, if the user is providing \t as separator, transform it to a real
           tab. Example: read_csv(filename,"\t");
        */
        wcscpy(separator, L"\t");
    }


    filename = csv_getArgumentAsWideString(pvApiCtx, 1, fname, &iErr);
    if (iErr)
    {
        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
        return 0;
    }

    result = csvRead(filename, separator, decimal, toreplace, nbElementsToReplace * 2, regexp, header);
    freeVar(NULL, NULL, NULL, NULL, NULL, &toreplace, nbElementsToReplace * 2, &regexp);

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
                int m1 = 0;
                int n1 = 0;

                wchar_t** pstrValues = NULL;
                double* pDblRealValues = NULL;
                double* pDblImgValues = NULL;

                char* errorMsg = NULL;

                errorMsg = csvTextScanSize(result->pwstrValues, &result->nbLines, separator, &m1, &n1, haveRange, iRange);
                if (errorMsg != NULL)
                {
                    Scierror(999, errorMsg, fname);
                    break;
                }

                /*
                 * Allocate some memory to store the decoded content
                 */

                if (wcscmp(conversion, CONVTOSTR) == 0)
                {
                    // allocMatrixOfWideString (non existing yet) here will avoid an extra copy
                    if (haveRange)
                    {
                        pstrValues = CALLOC((iRange[2] - iRange[0] + 1) * (iRange[3] - iRange[1] + 1), sizeof(wchar_t*));
                    }
                    else
                    {
                        pstrValues = CALLOC(m1 * n1, sizeof(wchar_t*));
                    }
                }
                else
                {
                    if (haveRange)
                    {
                        sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, (iRange[2] - iRange[0] + 1), (iRange[3] - iRange[1] + 1), &pDblRealValues);
                    }
                    else
                    {
                        sciErr = allocMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, &pDblRealValues);
                    }
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        freeCsvResult(result);
                        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
                        return 0;
                    }
                }


                if (csvTextScanInPlace(result->pwstrValues, result->nbLines, separator, decimal, haveRange, iRange, m1, n1, pstrValues, pDblRealValues, &pDblImgValues))
                {
                    Scierror(17, _("%s: Memory allocation error.\n"), fname);
                    freeCsvResult(result);
                    freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
                    return 0;
                }

                // push or recreate Scilab variables
                if (pDblRealValues != NULL && pDblImgValues != NULL)
                {
                    // assign the re-allocated complex array
                    if (haveRange)
                    {
                        sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 2, (iRange[2] - iRange[0] + 1), (iRange[3] - iRange[1] + 1), pDblRealValues, pDblImgValues);
                    }
                    else
                    {
                        sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 2, m1, n1, pDblRealValues, pDblImgValues);
                    }
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        freeCsvResult(result);
                        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
                        return 0;
                    }
                    LhsVar(1) = Rhs + 2;
                    // release the allocated on-demand array
                    FREE(pDblImgValues);
                }
                else if (pDblRealValues != NULL)
                {
                    // data have been copied in place, just assign !
                    LhsVar(1) = Rhs + 1;
                }
                else
                {
                    // string conversion, copy the data back to Scilab
                    if (haveRange)
                    {
                        sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, (iRange[2] - iRange[0] + 1), (iRange[3] - iRange[1] + 1), pstrValues);
                    }
                    else
                    {
                        sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 1, m1, n1, pstrValues);
                    }
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        freeCsvResult(result);
                        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
                        FREE(pstrValues);
                        return 0;
                    }
                    LhsVar(1) = Rhs + 1;

                    FREE(pstrValues);
                }

                if (result->nbComments > 0)
                {
                    sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 3, result->nbComments, 1, result->pwstrComments);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        freeCsvResult(result);
                        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
                        return 0;
                    }

                    LhsVar(2) = Rhs + 3;
                }

                if (result->nbHeader > 0)
                {

                    sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 4, result->nbHeader, 1, result->pwstrHeader);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        freeCsvResult(result);
                        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
                        return 0;
                    }

                    if (result->nbComments > 0)
                    {
                        LhsVar(3) = Rhs + 4;
                    }
                    else
                    {
                        LhsVar(2) = Rhs + 4;
                    }
                }

                if (*getNbOutputArgument(pvApiCtx) > 1 && result->nbComments == 0 && result->nbHeader == 0)
                {
                    wchar_t* emptystr = L"";
                    sciErr = createMatrixOfWideString(pvApiCtx, Rhs + 3, 1, 1, &emptystr);
                    if (sciErr.iErr)
                    {
                        printError(&sciErr, 0);
                        Scierror(17, _("%s: Memory allocation error.\n"), fname);
                        freeCsvResult(result);
                        freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);
                        return 0;
                    }
                    LhsVar(2) = Rhs + 3;
                }

            }
            break;

            case CSV_READ_FILE_NOT_EXIST:
            {
                char* f = wide_string_to_UTF8(filename);
                Scierror(999, _("%s: %s does not exist.\n"), fname, f);
                FREE(f);
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

    freeVar(&filename, &separator, &decimal, &conversion, &iRange, &toreplace, nbElementsToReplace * 2, &regexp);

    return 0;
}
/* ==================================================================== */
static void freeVar(wchar_t** filename, wchar_t** separator, wchar_t** decimal, wchar_t** conversion, int** iRange, wchar_t*** toreplace, int sizeReplace, wchar_t** regexp)
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
        freeArrayOfWideString(*toreplace, sizeReplace);
        *toreplace = NULL;
    }

    if (regexp && *regexp)
    {
        FREE(*regexp);
        *regexp = NULL;
    }
}
