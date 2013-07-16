/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include "csvDefault.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "checkCsvWriteFormat.h"
// =============================================================================
#define DEFAULT_CSV_SEPARATOR ","
#define DEFAULT_CSV_DECIMAL "."
#define DEFAULT_CSV_PRECISION "%.17lg"
#define DEFAULT_CSV_COMMENTS_REGEXP ""
#ifdef _MSC_VER
#define DEFAULT_CSV_EOL "\r\n"
#else
#define DEFAULT_CSV_EOL "\n"
#endif
#define CSV_DECIMAL_MODE_1 DEFAULT_CSV_DECIMAL
#define CSV_DECIMAL_MODE_2 ","
#define DEFAULT_CSV_CONVERSION "double"
#define CSV_CONVERSION_MODE_1 DEFAULT_CSV_CONVERSION
#define CSV_CONVERSION_MODE_2 "string"
#define DEFAULT_CSV_ENCODING "utf-8"
#define DEFAULT_CSV_ENCODING_MODE1 "utf-8"
#define DEFAULT_CSV_ENCODING_MODE2 "iso-latin"
#define CSV_IGNORE_BLANK_LINE_MODE_1 "off"
#define CSV_IGNORE_BLANK_LINE_MODE_2 "on"
#define DEFAULT_IGNORE_BLANK_LINE CSV_IGNORE_BLANK_LINE_MODE_2
// =============================================================================
static char *defaultCsvSeparator = NULL;
static char *defaultCsvDecimal = NULL;
static char *defaultCsvConversion = NULL;
static char *defaultCsvPrecision = NULL;
static char *defaultCsvCommentsRegExp = NULL;
static char *defaultCsvEOL = NULL;
static char *defaultCsvEncoding = NULL;
static char *defaultCsvIgnoreBlankLine = NULL;
// =============================================================================
static int initializeCsvDefaultValues(void);
// =============================================================================
const char *getCsvDefaultSeparator(void)
{
    initializeCsvDefaultValues();
    return defaultCsvSeparator;
}
// =============================================================================
const char *getCsvDefaultDecimal(void)
{
    initializeCsvDefaultValues();
    return defaultCsvDecimal;
}
// =============================================================================
const char *getCsvDefaultConversion(void)
{
    initializeCsvDefaultValues();
    return defaultCsvConversion;
}
// =============================================================================
const char *getCsvDefaultPrecision(void)
{
    initializeCsvDefaultValues();
    return defaultCsvPrecision;
}
// =============================================================================
const char *getCsvDefaultCommentsRegExp(void)
{
    initializeCsvDefaultValues();
    return defaultCsvCommentsRegExp;
}
// =============================================================================
const char *getCsvDefaultEOL(void)
{
    initializeCsvDefaultValues();
    return defaultCsvEOL;
}
// =============================================================================
const char *getCsvDefaultEncoding(void)
{
    initializeCsvDefaultValues();
    return defaultCsvEncoding;
}
// =============================================================================
const char *getCsvDefaultCsvIgnoreBlankLine(void)
{
    initializeCsvDefaultValues();
    return defaultCsvIgnoreBlankLine;
}
// =============================================================================
int setCsvDefaultSeparator(const char *separator)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (separator == NULL)
    {
        return 1;
    }

    if (strcmp(separator, getCsvDefaultDecimal()) == 0)
    {
        return 1;
    }

    if (defaultCsvSeparator)
    {
        FREE(defaultCsvSeparator);
    }
    defaultCsvSeparator = strdup(separator);
    if (defaultCsvSeparator == NULL)
    {
        return 1;
    }

    return 0;
}
// =============================================================================
int setCsvDefaultDecimal(const char *decimal)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (decimal == NULL)
    {
        return 1;
    }

    /* decimal separator supported . and , */
    if ((strcmp(decimal, CSV_DECIMAL_MODE_1) == 0) ||
            (strcmp(decimal, CSV_DECIMAL_MODE_2) == 0))
    {
        if (strcmp(decimal, getCsvDefaultSeparator()) == 0)
        {
            return 1;
        }
        if (defaultCsvDecimal)
        {
            FREE(defaultCsvDecimal);
        }
        defaultCsvDecimal = strdup(decimal);
        if (defaultCsvDecimal == NULL)
        {
            return 1;
        }
        return 0;
    }
    return 1;
}
// =============================================================================
int setCsvDefaultConversion(const char *conversion)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (conversion == NULL)
    {
        return 1;
    }

    if ((strcmp(conversion, CSV_CONVERSION_MODE_1) == 0) ||
            (strcmp(conversion, CSV_CONVERSION_MODE_2) == 0))
    {
        if (defaultCsvConversion)
        {
            FREE(defaultCsvConversion);
        }
        defaultCsvConversion = strdup(conversion);
        if (defaultCsvConversion)
        {
            return 0;
        }
    }

    return 1;
}
// =============================================================================
int setCsvDefaultPrecision(const char *precision)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (precision == NULL)
    {
        return 1;
    }
    if (checkCsvWriteFormat(precision) == 0)
    {
        if (defaultCsvPrecision)
        {
            FREE(defaultCsvPrecision);
        }
        defaultCsvPrecision = strdup(precision);
        if (defaultCsvPrecision)
        {
            return 0;
        }
    }
    return 1;
}
// =============================================================================
int setCsvDefaultCsvIgnoreBlankLine(const char *blankMode)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (blankMode == NULL)
    {
        return 1;
    }
    if ((strcmp(blankMode, CSV_IGNORE_BLANK_LINE_MODE_1) == 0) ||
            (strcmp(blankMode, CSV_IGNORE_BLANK_LINE_MODE_2) == 0))
    {
        if (defaultCsvIgnoreBlankLine)
        {
            FREE(defaultCsvIgnoreBlankLine);
        }
        defaultCsvIgnoreBlankLine = strdup(blankMode);
        if (defaultCsvIgnoreBlankLine)
        {
            return 0;
        }
    }
    return 1;
}
// =============================================================================
static int initializeCsvDefaultValues(void)
{
    if (defaultCsvSeparator == NULL)
    {
        defaultCsvSeparator = strdup(DEFAULT_CSV_SEPARATOR);
    }

    if (defaultCsvDecimal == NULL)
    {
        defaultCsvDecimal = strdup(DEFAULT_CSV_DECIMAL);
    }

    if (defaultCsvConversion == NULL)
    {
        defaultCsvConversion = strdup(DEFAULT_CSV_CONVERSION);
    }

    if (defaultCsvPrecision == NULL)
    {
        defaultCsvPrecision = strdup(DEFAULT_CSV_PRECISION);
    }

    if (defaultCsvCommentsRegExp == NULL)
    {
        defaultCsvCommentsRegExp = strdup(DEFAULT_CSV_COMMENTS_REGEXP);
    }

    if (defaultCsvEOL == NULL)
    {
        defaultCsvEOL = strdup(DEFAULT_CSV_EOL);
    }

    if (defaultCsvEncoding == NULL)
    {
        defaultCsvEncoding = strdup(DEFAULT_CSV_ENCODING);
    }

    if (defaultCsvIgnoreBlankLine == NULL)
    {
        defaultCsvIgnoreBlankLine =  strdup(DEFAULT_IGNORE_BLANK_LINE);
    }

    if ((defaultCsvSeparator == NULL) ||
            (defaultCsvDecimal == NULL) ||
            (defaultCsvConversion == NULL) ||
            (defaultCsvPrecision == NULL) ||
            (defaultCsvCommentsRegExp == NULL) ||
            (defaultCsvEOL == NULL) ||
            (defaultCsvEncoding == NULL) ||
            (defaultCsvIgnoreBlankLine == NULL))
    {
        return 1;
    }

    return 0;
}
// =============================================================================
int setCsvDefaultReset(void)
{
    if (defaultCsvSeparator)
    {
        FREE(defaultCsvSeparator);
        defaultCsvSeparator = NULL;
    }
    if (defaultCsvDecimal)
    {
        FREE(defaultCsvDecimal);
        defaultCsvDecimal = NULL;
    }
    if (defaultCsvConversion)
    {
        FREE(defaultCsvConversion);
        defaultCsvConversion = NULL;
    }
    if (defaultCsvPrecision)
    {
        FREE(defaultCsvPrecision);
        defaultCsvPrecision = NULL;
    }
    if (defaultCsvCommentsRegExp)
    {
        FREE(defaultCsvCommentsRegExp);
        defaultCsvCommentsRegExp = NULL;
    }
    if (defaultCsvEOL)
    {
        FREE(defaultCsvEOL);
        defaultCsvEOL = NULL;
    }
    if (defaultCsvEncoding)
    {
        FREE(defaultCsvEncoding);
        defaultCsvEncoding = NULL;
    }
    if (defaultCsvIgnoreBlankLine)
    {
        FREE(defaultCsvIgnoreBlankLine);
        defaultCsvIgnoreBlankLine = NULL;
    }
    return initializeCsvDefaultValues();
}
// =============================================================================
int setCsvDefaultCommentsRegExp(const char *commentsRegExp)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (commentsRegExp == NULL)
    {
        return 1;
    }

    if (strcmp(commentsRegExp, getCsvDefaultCommentsRegExp()) == 0)
    {
        return 1;
    }

    if (defaultCsvCommentsRegExp)
    {
        FREE(defaultCsvCommentsRegExp);
        defaultCsvCommentsRegExp = NULL;
    }

    defaultCsvCommentsRegExp = strdup(commentsRegExp);

    if (defaultCsvDecimal == NULL)
    {
        return 1;
    }

    return 0;
}
// =============================================================================
int setCsvDefaultEOL(const char *eol)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (eol == NULL)
    {
        return 1;
    }

    if (strcmp(eol, getCsvDefaultEOL()) == 0)
    {
        return 0;
    }

    if (defaultCsvEOL)
    {
        FREE(defaultCsvEOL);
        defaultCsvEOL = NULL;
    }

    defaultCsvEOL = strdup(eol);

    if (defaultCsvEOL == NULL)
    {
        return 1;
    }

    return 0;
}
// =============================================================================
int setCsvDefaultEncoding(const char *encoding)
{
    if (initializeCsvDefaultValues())
    {
        return 1;
    }
    if (encoding == NULL)
    {
        return 1;
    }

    if (strcmp(encoding, getCsvDefaultEncoding()) == 0)
    {
        return 1;
    }

    if ((strcmp(encoding, DEFAULT_CSV_ENCODING_MODE1) != 0) &&
            (strcmp(encoding, DEFAULT_CSV_ENCODING_MODE2) != 0))
    {
        return 1;
    }

    if (defaultCsvEncoding)
    {
        FREE(defaultCsvEncoding);
        defaultCsvEncoding = NULL;
    }

    defaultCsvEncoding = strdup(encoding);

    if (defaultCsvEncoding == NULL)
    {
        return 1;
    }

    return 0;
}
// =============================================================================
