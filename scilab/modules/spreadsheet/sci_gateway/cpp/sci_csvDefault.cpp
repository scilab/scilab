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
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
extern "C" {
#include "freeArrayOfString.h"
};
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "csvDefault.h"
#include "gw_csv_helpers.h"
// =============================================================================
#define SEPARATOR_FIELDNAME "separator"
#define DECIMAL_FIELDNAME "decimal"
#define CONVERSION_FIELDNAME "conversion"
#define PRECISION_FIELDNAME "precision"
#define COMMENTSREGEXP_FIELDNAME "regexp"
#define EOL_FIELDNAME "eol"
#define ENCODING_FIELDNAME "encoding"
#define RESET_PARAMATERS "reset"
#define BLANK_FIELDNAME "blank"
// =============================================================================
#define MACOS9_EOL_STRING "macos9"
#define MACOS9_EOL "\r"
#define WINDOWS_EOL_STRING "windows"
#define WINDOWS_EOL "\r\n"
#define LINUX_EOL_STRING "linux"
#define LINUX_EOL "\n"
// =============================================================================
#define NUMBER_FIELD 8
// =============================================================================
static int sci_csvDefault_no_rhs(char *fname);
static int sci_csvDefault_one_rhs(char *fname);
static int sci_csvDefault_two_rhs(char *fname);
// =============================================================================
int sci_csvDefault(char *fname)
{
    Rhs = Max(0, Rhs);
    CheckRhs(0, 2);
    CheckLhs(1, 1);

    switch (Rhs)
    {
        case 0:
            return sci_csvDefault_no_rhs(fname);
        case 1:
            return sci_csvDefault_one_rhs(fname);
        case 2:
            return sci_csvDefault_two_rhs(fname);
    }
    return 0;
}
// =============================================================================
static int sci_csvDefault_no_rhs(char *fname)
{
    int sizeArray = NUMBER_FIELD * 2;
    char **arrayOut = (char**)MALLOC(sizeof(char*) * sizeArray);

    if (arrayOut)
    {
        SciErr sciErr;

        int nbRows = NUMBER_FIELD;
        int nbCols = 2;
        const char *currentEol = getCsvDefaultEOL();

        arrayOut[0] = strdup(SEPARATOR_FIELDNAME);
        arrayOut[1] = strdup(DECIMAL_FIELDNAME);
        arrayOut[2] = strdup(CONVERSION_FIELDNAME);
        arrayOut[3] = strdup(PRECISION_FIELDNAME);
        arrayOut[4] = strdup(COMMENTSREGEXP_FIELDNAME);
        arrayOut[5] = strdup(EOL_FIELDNAME);
        arrayOut[6] = strdup(ENCODING_FIELDNAME);
        arrayOut[7] = strdup(BLANK_FIELDNAME);

        arrayOut[8] = strdup(getCsvDefaultSeparator());
        arrayOut[9] = strdup(getCsvDefaultDecimal());
        arrayOut[10] = strdup(getCsvDefaultConversion());
        arrayOut[11] = strdup(getCsvDefaultPrecision());
        arrayOut[12] = strdup(getCsvDefaultCommentsRegExp());

        if (currentEol)
        {
            if (strcmp(currentEol, MACOS9_EOL) == 0)
            {
                arrayOut[13] = strdup(MACOS9_EOL_STRING);
            }
            else if (strcmp(currentEol, WINDOWS_EOL) == 0)
            {
                arrayOut[13] = strdup(WINDOWS_EOL_STRING);
            }
            else if (strcmp(currentEol, LINUX_EOL) == 0)
            {
                arrayOut[13] = strdup(LINUX_EOL_STRING);
            }
            else
            {
                arrayOut[13] = strdup("ERROR");
            }
        }
        else
        {
            arrayOut[13] = strdup("ERROR");
        }

        arrayOut[14] = strdup(getCsvDefaultEncoding());
        arrayOut[15] = strdup(getCsvDefaultCsvIgnoreBlankLine());

        sciErr = createMatrixOfString(pvApiCtx, Rhs + 1, nbRows, nbCols, arrayOut);
        freeArrayOfString(arrayOut, sizeArray);
        if (sciErr.iErr)
        {
            printError(&sciErr, 0);
            return 0;
        }

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
    }
    else
    {
        Scierror(999, _("%s: Memory allocation error.\n"), fname);
    }

    return 0;
}
// =============================================================================
static int sci_csvDefault_one_rhs(char *fname)
{
    int iErr = 0;

    char *fieldname = NULL;
    char *fieldvalue = NULL;

    fieldname = csv_getArgumentAsString(pvApiCtx, 1, fname, &iErr);
    if (iErr) return 0;

    if (strcmp(fieldname, SEPARATOR_FIELDNAME) == 0)
    {
        fieldvalue = strdup(getCsvDefaultSeparator());
    }
    else if (strcmp(fieldname, DECIMAL_FIELDNAME) == 0)
    {
        fieldvalue = strdup(getCsvDefaultDecimal());
    }
    else if (strcmp(fieldname, CONVERSION_FIELDNAME) == 0)
    {
        fieldvalue = strdup(getCsvDefaultConversion());
    }
    else if (strcmp(fieldname, PRECISION_FIELDNAME) == 0)
    {
        fieldvalue = strdup(getCsvDefaultPrecision());
    }
    else if (strcmp(fieldname, COMMENTSREGEXP_FIELDNAME) == 0)
    {
        fieldvalue = strdup(getCsvDefaultCommentsRegExp());
    }
    else if (strcmp(fieldname, EOL_FIELDNAME) == 0)
    {
        const char *currentEol = getCsvDefaultEOL();
        if (currentEol)
        {
            if (strcmp(currentEol, MACOS9_EOL) == 0)
            {
                fieldvalue = strdup(MACOS9_EOL_STRING);
            }
            else if (strcmp(currentEol, WINDOWS_EOL) == 0)
            {
                fieldvalue = strdup(WINDOWS_EOL_STRING);
            }
            else if (strcmp(currentEol, LINUX_EOL) == 0)
            {
                fieldvalue = strdup(LINUX_EOL_STRING);
            }
            else
            {
                fieldvalue = strdup("ERROR");
            }
        }
        else
        {
            fieldvalue = strdup("ERROR");
        }
    }
    else if (strcmp(fieldname, ENCODING_FIELDNAME) == 0)
    {
        fieldvalue = strdup(getCsvDefaultEncoding());
    }
    else if (strcmp(fieldname, BLANK_FIELDNAME) == 0)
    {
        fieldvalue = strdup(getCsvDefaultCsvIgnoreBlankLine());
    }
    else if (strcmp(fieldname, RESET_PARAMATERS) == 0)
    {
        if (fieldname)
        {
            FREE(fieldname);
            fieldname = NULL;
        }
        if (fieldvalue)
        {
            FREE(fieldvalue);
            fieldvalue = NULL;
        }

        setCsvDefaultReset();

        createEmptyMatrix(pvApiCtx, Rhs + 1);

        LhsVar(1) = Rhs + 1;
        PutLhsVar();
        return 0;
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' , '%s', '%s' '%s' or '%s' expected.\n"), fname, 1, SEPARATOR_FIELDNAME, DECIMAL_FIELDNAME, CONVERSION_FIELDNAME, COMMENTSREGEXP_FIELDNAME, EOL_FIELDNAME, BLANK_FIELDNAME);
        if (fieldname)
        {
            FREE(fieldname);
            fieldname = NULL;
        }
        return 0;
    }

    if (fieldname)
    {
        FREE(fieldname);
        fieldname = NULL;
    }

    createSingleString(pvApiCtx, Rhs + 1, fieldvalue);

    if (fieldvalue)
    {
        FREE(fieldvalue);
        fieldvalue = NULL;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
// =============================================================================
static int sci_csvDefault_two_rhs(char *fname)
{
    int iErr = 0;
    int resultSet = 0;

    char *fieldname = NULL;
    char *fieldvalue = NULL;
    int  ifieldvalue = 0;

    fieldname = csv_getArgumentAsString(pvApiCtx, 1, fname, &iErr);
    if (iErr) return 0;

    if (strcmp(fieldname, PRECISION_FIELDNAME) == 0)
    {
        if (csv_isEmpty(pvApiCtx, 2))
        {
            if (fieldname)
            {
                FREE(fieldname);
                fieldname = NULL;
            }
            Scierror(999, _("%s: Wrong type for input argument #%d: A double expected.\n"), fname, 2);
            return 0;
        }

        if (csv_isDoubleScalar(pvApiCtx, 2))
        {
#define FORMAT_FIELDVALUESTR "%%.%dlg"
            ifieldvalue = (int) csv_getArgumentAsScalarDouble(pvApiCtx, 2, fname, &iErr);
            if (iErr)
            {
                if (fieldname)
                {
                    FREE(fieldname);
                    fieldname = NULL;
                }
                return 0;
            }

            if ((ifieldvalue < 1) || (ifieldvalue > 17))
            {
                if (fieldname)
                {
                    FREE(fieldname);
                    fieldname = NULL;
                }
                Scierror(999, _("%s: Wrong value for input argument #%d: A double (value 1 to 17) expected.\n"), fname, 2);
                return 0;
            }

            fieldvalue = (char*)MALLOC(sizeof(char) * ((int)strlen(FORMAT_FIELDVALUESTR) + 1));
            if (fieldvalue == NULL)
            {
                if (fieldname)
                {
                    FREE(fieldname);
                    fieldname = NULL;
                }
                Scierror(999, _("%s: Memory allocation error.\n"), fname);
                return 0;
            }
            sprintf(fieldvalue, FORMAT_FIELDVALUESTR, ifieldvalue);
        }
        else
        {
            fieldvalue = csv_getArgumentAsString(pvApiCtx, 2, fname, &iErr);
            if (iErr)
            {
                if (fieldname)
                {
                    FREE(fieldname);
                    fieldname = NULL;
                }
                return 0;
            }
        }
    }
    else
    {
        fieldvalue = csv_getArgumentAsString(pvApiCtx, 2, fname, &iErr);
        if (iErr)
        {
            if (fieldname)
            {
                FREE(fieldname);
                fieldname = NULL;
            }
            return 0;
        }
    }

    if (strcmp(fieldname, SEPARATOR_FIELDNAME) == 0)
    {
        resultSet = setCsvDefaultSeparator(fieldvalue);
    }
    else if (strcmp(fieldname, DECIMAL_FIELDNAME) == 0)
    {
        resultSet = setCsvDefaultDecimal(fieldvalue);
    }
    else if (strcmp(fieldname, CONVERSION_FIELDNAME) == 0)
    {
        resultSet = setCsvDefaultConversion(fieldvalue);
    }
    else if (strcmp(fieldname, PRECISION_FIELDNAME) == 0)
    {
        resultSet = setCsvDefaultPrecision(fieldvalue);
    }
    else if (strcmp(fieldname, COMMENTSREGEXP_FIELDNAME) == 0)
    {
        resultSet = setCsvDefaultCommentsRegExp(fieldvalue);
    }
    else if (strcmp(fieldname, EOL_FIELDNAME) == 0)
    {
        if (strcmp(fieldvalue, MACOS9_EOL_STRING) == 0)
        {
            resultSet = setCsvDefaultEOL(MACOS9_EOL);
        }
        else if (strcmp(fieldvalue, WINDOWS_EOL_STRING) == 0)
        {
            resultSet = setCsvDefaultEOL(WINDOWS_EOL);
        }
        else if (strcmp(fieldvalue, LINUX_EOL_STRING) == 0)
        {
            resultSet = setCsvDefaultEOL(LINUX_EOL);
        }
        else
        {
            resultSet = 1;
        }
    }
    else if (strcmp(fieldname, ENCODING_FIELDNAME) == 0)
    {
        resultSet = setCsvDefaultEncoding(fieldvalue);
    }
    else if (strcmp(fieldname, BLANK_FIELDNAME) == 0)
    {
        resultSet = setCsvDefaultCsvIgnoreBlankLine(fieldvalue);
    }
    else
    {
        Scierror(999, _("%s: Wrong value for input argument #%d: '%s', '%s' ,'%s' , '%s', '%s', '%s', '%s' or '%s' expected.\n"), fname, 1, SEPARATOR_FIELDNAME, DECIMAL_FIELDNAME, CONVERSION_FIELDNAME, PRECISION_FIELDNAME, COMMENTSREGEXP_FIELDNAME, EOL_FIELDNAME, ENCODING_FIELDNAME, BLANK_FIELDNAME);
        if (fieldname)
        {
            FREE(fieldname);
            fieldname = NULL;
        }
        if (fieldvalue)
        {
            FREE(fieldvalue);
            fieldvalue = NULL;
        }
        return 0;
    }

    createScalarBoolean(pvApiCtx, Rhs + 1, (resultSet == 0));

    if (fieldname)
    {
        FREE(fieldname);
        fieldname = NULL;
    }
    if (fieldvalue)
    {
        FREE(fieldvalue);
        fieldvalue = NULL;
    }

    LhsVar(1) = Rhs + 1;
    PutLhsVar();

    return 0;
}
// =============================================================================
