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
#include "gw_csv_helpers.h"
#include "csv_complex.h"

// =============================================================================
int sci_csvStringToDouble(char *fname, unsigned long fname_len)
{
    SciErr sciErr;
    int iErr = 0;
    int m1 = 0, n1 = 0;
    char **pStringValues = NULL;

    BOOL bConvertToNan = TRUE;

    csv_complexArray *ptrCsvComplexArray = NULL;
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

    ptrCsvComplexArray = stringsToCsvComplexArray((const char**)pStringValues, m1 * n1, getCsvDefaultDecimal(), bConvertToNan, &ierr);

    freeArrayOfString(pStringValues, m1 * n1);
    pStringValues = NULL;

    if (ptrCsvComplexArray == NULL)
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
            if (ptrCsvComplexArray->isComplex)
            {
                sciErr = createComplexMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, ptrCsvComplexArray->realPart, ptrCsvComplexArray->imagPart);
            }
            else
            {
                sciErr = createMatrixOfDouble(pvApiCtx, Rhs + 1, m1, n1, ptrCsvComplexArray->realPart);
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

