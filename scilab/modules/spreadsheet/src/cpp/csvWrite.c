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
#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif
// =============================================================================
#include <stdio.h>
#include <string.h>
#include "stringToDouble.h"
#include "csvWrite.h"
#include "mopen.h"
#include "mgetl.h"
#include "expandPathVariable.h"
#include "FileExist.h"
#include "mclose.h"
#include "MALLOC.h"
#include "core_math.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "csv_strsubst.h"
#include "csvDefault.h"
#include "utftolatin.h"
// =============================================================================
#define DEFAULT_CSV_WRITE_STRING_FORMAT "%s"
#define DEFAULT_CSV_WRITE_DOUBLE_FORMAT "%.lg"
#define PlusStr "+"
#define LessStr "-"
#define ComplexStr "i"
#define EMPTY_STRING ""
#if _MSC_VER
#define MODEWFD "wt"
#else
#define MODEWFD "w"
#endif
// =============================================================================
#ifndef signbit
static int signbit(double x)
{
    union
    {
        double d;
        short s[4];
        int i[2];
    } u;

    u.d = x;
#if SIZEOF_INT == 4
    return u.i[1] < 0;
#else
    return u.s[3] < 0;
#endif
}
#endif /* signbit */
// =============================================================================
static int doConvertToLatin(void)
{
    const char *encoding = getCsvDefaultEncoding();
    if (encoding)
    {
        return (strcmp(encoding, "iso-latin") == 0) ? 1 : 0;
    }
    return 0;
}
// =============================================================================
csvWriteError csvWrite_double(const char *filename,
                              const double *pdValues, int m, int n,
                              const char *separator,
                              const char *decimal,
                              const char *precisionFormat,
                              const char **headersLines,
                              int nbHeadersLines)
{
    FILE  *fd = NULL;
    int i = 0, j = 0;
    char *expandedFilename = NULL;
    int isIsoLatin = 0;

    if (filename == NULL) return CSV_WRITE_ERROR;
    if (pdValues == NULL) return CSV_WRITE_ERROR;
    if (m < 0 || n < 0) return CSV_WRITE_ERROR;
    if (separator == NULL) return CSV_WRITE_ERROR;
    if (decimal == NULL) return CSV_WRITE_ERROR;
    if (precisionFormat == NULL) return CSV_WRITE_ERROR;

    if (strcmp(separator, decimal) == 0) return CSV_WRITE_SEPARATOR_DECIMAL_EQUAL;

    expandedFilename = expandPathVariable((char*)filename);
    wcfopen(fd , filename, MODEWFD);
    if (expandedFilename)
    {
        FREE(expandedFilename);
        expandedFilename = NULL;
    }
    if ( fd == (FILE *)NULL ) return CSV_WRITE_FOPEN_ERROR;

    isIsoLatin = doConvertToLatin();

    if ((headersLines) && (nbHeadersLines > 0))
    {
        for (i = 0; i < nbHeadersLines; i++)
        {
            if (isIsoLatin)
            {
                char *converted = utftolatin((char*)headersLines[i]);
                if (converted)
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, converted);
                    FREE(converted);
                }
                else
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, headersLines[i]);
                }
            }
            else
            {
                fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, headersLines[i]);
            }
            fprintf(fd, "%s", getCsvDefaultEOL());
        }
    }

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (ISNAN(pdValues[i + m * j]))
            {
                fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, NanString);
            }
            else if (finite(pdValues[i + m * j]))
            {
                char buffer[65535];
                char *result = NULL;
                sprintf(buffer, precisionFormat, pdValues[i + m * j]);
                result = csv_strsubst(buffer, getCsvDefaultDecimal(), decimal);
                if (result)
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, result);
                    FREE(result);
                    result = NULL;
                }
                else
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_DOUBLE_FORMAT, pdValues[i + m * j]);
                }
            }
            else
            {
                if ( signbit(pdValues[i + m * j]) )
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, NegInfString);
                }
                else
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, InfString);
                }
            }
            if (j + 1 < n) fprintf(fd, "%s", separator);
        }
        fprintf(fd, "%s", getCsvDefaultEOL());
    }

    fclose(fd);
    return CSV_WRITE_NO_ERROR;
}
// =============================================================================
csvWriteError csvWrite_complex(const char *filename,
                               const double *pdValuesReal,
                               const double *pdValuesImag,
                               int m, int n,
                               const char *separator,
                               const char *decimal,
                               const char *precisionFormat,
                               const char **headersLines,
                               int nbHeadersLines)
{
    FILE  *fd = NULL;
    int i = 0, j = 0;
    char *expandedFilename = NULL;
    int isIsoLatin = 0;

    if (filename == NULL) return CSV_WRITE_ERROR;
    if (pdValuesReal == NULL) return CSV_WRITE_ERROR;
    if (pdValuesImag == NULL) return CSV_WRITE_ERROR;
    if (m < 0 || n < 0) return CSV_WRITE_ERROR;
    if (separator == NULL) return CSV_WRITE_ERROR;
    if (decimal == NULL) return CSV_WRITE_ERROR;
    if (precisionFormat == NULL) return CSV_WRITE_ERROR;

    if (strcmp(separator, decimal) == 0) return CSV_WRITE_SEPARATOR_DECIMAL_EQUAL;

    expandedFilename = expandPathVariable((char*)filename);
    wcfopen(fd , filename, MODEWFD);
    if (expandedFilename)
    {
        FREE(expandedFilename);
        expandedFilename = NULL;
    }
    if ( fd == (FILE *)NULL ) return CSV_WRITE_FOPEN_ERROR;

    isIsoLatin = doConvertToLatin();

    if ((headersLines) && (nbHeadersLines > 0))
    {
        for (i = 0; i < nbHeadersLines; i++)
        {
            if (isIsoLatin)
            {
                const char *converted = utftolatin((char*)headersLines[i]);
                if (converted)
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, converted);
                    FREE((char*)converted);
                    converted = NULL;
                }
                else
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, headersLines[i]);
                }
            }
            else
            {
                fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, headersLines[i]);
            }
            fprintf(fd, "%s", getCsvDefaultEOL());
        }
    }

    for (i = 0; i < m; i++)
    {
        for (j = 0; j < n; j++)
        {
            char StringValue[65535];
            if (ISNAN(pdValuesReal[i + m * j]))
            {
                strcpy(StringValue, NanString);
            }
            else if (finite(pdValuesReal[i + m * j]))
            {
                char buffer[65535];
                char *result = NULL;
                sprintf(buffer, precisionFormat, pdValuesReal[i + m * j]);
                result = csv_strsubst(buffer, getCsvDefaultDecimal(), decimal);
                if (result)
                {
                    strcpy(StringValue, result);
                    FREE(result);
                    result = NULL;
                }
                else
                {
                    sprintf(StringValue, DEFAULT_CSV_WRITE_DOUBLE_FORMAT, pdValuesReal[i + m * j]);
                }
            }
            else
            {
                if ( signbit(pdValuesReal[i + m * j]) )
                {
                    // NegInfString
                    strcpy(StringValue, NegInfString);
                }
                else
                {
                    // InfString
                    strcpy(StringValue, InfString);
                }
            }

            if (ISNAN(pdValuesImag[i + m * j]))
            {
                strcat(StringValue, PlusStr);
                strcat(StringValue, NanString);
                strcat(StringValue, ComplexStr);
            }
            else if (finite(pdValuesImag[i + m * j]))
            {
                char buffer[65535];
                char *result = NULL;

                if (pdValuesImag[i + m * j] >= 0)
                {
                    strcat(StringValue, PlusStr);
                }
                else
                {
                    strcat(StringValue, LessStr);
                }

                sprintf(buffer, precisionFormat, fabs(pdValuesImag[i + m * j]));
                result = csv_strsubst(buffer, getCsvDefaultDecimal(), decimal);

                if (result)
                {
                    strcat(StringValue, result);
                    FREE(result);
                    result = NULL;
                }
                else
                {
                    sprintf(buffer, DEFAULT_CSV_WRITE_DOUBLE_FORMAT, fabs(pdValuesImag[i + m * j]));
                    strcat(StringValue, buffer);
                }
                strcat(StringValue, ComplexStr);
            }
            else
            {
                if ( signbit(pdValuesImag[i + m * j]) )
                {
                    // NegInfString
                    strcat(StringValue, LessStr);
                }
                else
                {
                    // InfString
                    strcat(StringValue, PlusStr);
                }
                strcat(StringValue, InfString);
                strcat(StringValue, ComplexStr);
            }
            fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, StringValue);
            if (j + 1 < n) fprintf(fd, "%s", separator);
        }
        fprintf(fd, "%s", getCsvDefaultEOL());
    }

    fclose(fd);
    return CSV_WRITE_NO_ERROR;
}
// =============================================================================
csvWriteError csvWrite_string(const char *filename,
                              const char **pStrValues, int m, int n,
                              const char *separator,
                              const char *decimal,
                              const char **headersLines,
                              int nbHeadersLines)
{
    FILE  *fd = NULL;
    int i = 0, j = 0;
    char *expandedFilename = NULL;
    int isIsoLatin = 0;

    if (filename == NULL) return CSV_WRITE_ERROR;
    if (pStrValues == NULL) return CSV_WRITE_ERROR;
    if (m < 0 || n < 0) return CSV_WRITE_ERROR;
    if (separator == NULL) return CSV_WRITE_ERROR;

    if (strcmp(separator, decimal) == 0) return CSV_WRITE_SEPARATOR_DECIMAL_EQUAL;

    expandedFilename = expandPathVariable((char*)filename);
    wcfopen(fd , filename, MODEWFD);
    if (expandedFilename)
    {
        FREE(expandedFilename);
        expandedFilename = NULL;
    }
    if ( fd == (FILE *)NULL ) return CSV_WRITE_FOPEN_ERROR;

    isIsoLatin = doConvertToLatin();

    if ((headersLines) && (nbHeadersLines > 0))
    {
        for (i = 0; i < nbHeadersLines; i++)
        {
            if (isIsoLatin)
            {
                const char *converted = utftolatin((char*)headersLines[i]);
                if (converted)
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, converted);
                    FREE((char*)converted);
                    converted = NULL;
                }
                else
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, headersLines[i]);
                }
            }
            else
            {
                fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, headersLines[i]);
            }
            fprintf(fd, "%s", getCsvDefaultEOL());
        }
    }

    for (i = 0 ; i < m ; i++ )
    {
        for ( j = 0 ; j < n ; j++)
        {
            if (decimal == NULL)
            {
                if (isIsoLatin)
                {
                    const char *converted = utftolatin((char*)pStrValues[i + m * j]);
                    if (converted)
                    {
                        fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, converted);
                        FREE((char*)converted);
                        converted = NULL;
                    }
                    else
                    {
                        fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, pStrValues[i + m * j]);
                    }
                }
                else
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, pStrValues[i + m * j]);
                }
            }
            else
            {
                char *result = NULL;
                result = csv_strsubst((char*)(pStrValues[i + m * j]), getCsvDefaultDecimal(), decimal);
                if (result)
                {
                    if (isIsoLatin)
                    {
                        char *converted = utftolatin(result);
                        if (converted)
                        {
                            fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, converted);
                            FREE(converted);
                            converted = NULL;
                        }
                        else
                        {
                            fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, result);
                        }
                    }
                    else
                    {
                        fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, result);
                    }
                    FREE(result);
                    result = NULL;
                }
                else
                {
                    fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, pStrValues[i + m * j]);
                }
            }
            if (j + 1 < n) fprintf(fd, "%s", separator);
        }
        fprintf(fd, "%s", getCsvDefaultEOL());
    }

    fclose(fd);
    return CSV_WRITE_NO_ERROR;
}
// =============================================================================

