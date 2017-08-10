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
#include "gw_spreadsheet.h"
#include "api_scilab.h"
#include "Scierror.h"
#include "sci_malloc.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "stringToComplex.h"
#include "csvDefault.h"
#include "gw_csv_helpers.h"
#include "complex_array.h"

// =============================================================================
int sci_csvStringToDouble(char *fname, void* pvApiCtx)
{
    SciErr sciErr;
    int iErr = 0;
    int m1 = 0, n1 = 0;
    char **pStringValues = NULL;

    BOOL bConvertToNan = TRUE;

    complexArray *ptrComplexArray = NULL;
    stringToComplexError ierr = STRINGTOCOMPLEX_ERROR;

    CheckRhs(1, 2);
    CheckLhs(1, 1);

    if (Rhs == 1)
    {
        bConvertToNan = TRUE;
    }
    else /* Rhs == 2 */
    {
        bConvertToNan = (BOOL)csv_getArgumentAsScalarBoolean(pvApiCtx, 2, fname, &iErr);
        if (iErr)
        {
            return 0;
        }
    }

    pStringValues = csv_getArgumentAsMatrixOfString(pvApiCtx, 1, fname, &m1, &n1, &iErr);
    if (iErr)
    {
        return 0;
    }

    ptrComplexArray = stringsToComplexArray((const char**)pStringValues, m1 * n1, getCsvDefaultDecimal(), bConvertToNan, &ierr);

    freeArrayOfString(pStringValues, m1 * n1);
    pStringValues = NULL;

    if (ptrComplexArray == NULL)
    {
        switch (ierr)
        {
            case STRINGTOCOMPLEX_NOT_A_NUMBER:
            case STRINGTOCOMPLEX_ERROR:
                Scierror(999, _("%s: can not convert data.\n"), fname);
                return 0;

            default:
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
        }
    }

    switch (ierr)
    {
        case STRINGTOCOMPLEX_NOT_A_NUMBER:
        case STRINGTOCOMPLEX_NO_ERROR:
        {
            if (ptrComplexArray->isComplex)
            {
                sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, ptrComplexArray->realPart, ptrComplexArray->imagPart);
            }
            else
            {
                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, ptrComplexArray->realPart);
            }
            freeComplexArray(ptrComplexArray);
            ptrComplexArray = NULL;
        }
        break;

        case STRINGTOCOMPLEX_MEMORY_ALLOCATION:
        {
            Scierror(999, _("%s: Memory allocation error.\n"), fname);
        }
        break;

        default:
        case STRINGTOCOMPLEX_ERROR:
        {
            Scierror(999, _("%s: can not convert data.\n"), fname);
        }
    }

    if (sciErr.iErr)
    {
        printError(&sciErr, 0);
    }
    else
    {
        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }

    return 0;
}
// =============================================================================

