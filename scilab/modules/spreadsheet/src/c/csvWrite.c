/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
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
#include "sci_malloc.h"
#include "core_math.h"
#include "csvDefault.h"
#include "utftolatin.h"
#include "strsubst.h"
// =============================================================================
#define DEFAULT_CSV_WRITE_STRING_FORMAT "%s"
#define DEFAULT_CSV_WRITE_DOUBLE_FORMAT "%.lg"
#define PlusStr "+"
#define LessStr "-"
#define ComplexStr "i"
#define EMPTY_STRING ""
#if _MSC_VER
#define MODEWFD "wb"
#else
#define MODEWFD "w"
#endif
#if _MSC_VER
#define snprintf _snprintf
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

    if (filename == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (pdValues == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (m < 0 || n < 0)
    {
        return CSV_WRITE_ERROR;
    }
    if (separator == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (decimal == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (precisionFormat == NULL)
    {
        return CSV_WRITE_ERROR;
    }

    if (strcmp(separator, decimal) == 0)
    {
        return CSV_WRITE_SEPARATOR_DECIMAL_EQUAL;
    }

    expandedFilename = expandPathVariable((char*)filename);
    wcfopen(fd , filename, MODEWFD);
    if (expandedFilename)
    {
        FREE(expandedFilename);
        expandedFilename = NULL;
    }
    if ( fd == (FILE *)NULL )
    {
        return CSV_WRITE_FOPEN_ERROR;
    }

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
                result = strsub(buffer, getCsvDefaultDecimal(), decimal);
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
            if (j + 1 < n)
            {
                fprintf(fd, "%s", separator);
            }
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

    if (filename == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (pdValuesReal == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (pdValuesImag == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (m < 0 || n < 0)
    {
        return CSV_WRITE_ERROR;
    }
    if (separator == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (decimal == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (precisionFormat == NULL)
    {
        return CSV_WRITE_ERROR;
    }

    if (strcmp(separator, decimal) == 0)
    {
        return CSV_WRITE_SEPARATOR_DECIMAL_EQUAL;
    }

    expandedFilename = expandPathVariable((char*)filename);
    wcfopen(fd , filename, MODEWFD);
    if (expandedFilename)
    {
        FREE(expandedFilename);
        expandedFilename = NULL;
    }
    if ( fd == (FILE *)NULL )
    {
        return CSV_WRITE_FOPEN_ERROR;
    }

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

            double realValue = pdValuesReal[i + m * j];
            double imagValue = pdValuesImag[i + m * j];
            int hasReal = 0;

            if (ISNAN(realValue))
            {
                strcpy(StringValue, NanString);
                hasReal = 1;
            }
            else if (finite(realValue))
            {
                if ((realValue != 0) || (finite(imagValue) && (imagValue == 0)))
                {
                    char buffer[65535];
                    char *result = NULL;

                    sprintf(buffer, precisionFormat, pdValuesReal[i + m * j]);
                    result = strsub(buffer, getCsvDefaultDecimal(), decimal);
                    if (result)
                    {
                        strncpy(StringValue, result, sizeof(StringValue) - 1);
                        FREE(result);
                    }
                    else
                    {
                        sprintf(StringValue, DEFAULT_CSV_WRITE_DOUBLE_FORMAT, pdValuesReal[i + m * j]);
                    }
                    hasReal = 1;
                }
            }
            else
            {
                if (signbit(realValue))
                {
                    strcpy(StringValue, NegInfString);
                }
                else
                {
                    strcpy(StringValue, InfString);
                }
                hasReal = 1;
            }

            if (ISNAN(imagValue))
            {
                if (hasReal)
                {
                    strcat(StringValue, PlusStr);
                    strcat(StringValue, NanString);
                }
                else
                {
                    strcpy(StringValue, NanString);
                }
                strcat(StringValue, ComplexStr);
            }
            else if (finite(imagValue))
            {
                if (imagValue != 0)
                {
                    char buffer[65535];
                    char *result = NULL;

                    if (hasReal && (imagValue > 0))
                    {
                        strcat(StringValue, PlusStr);
                    }
                    else if (imagValue < 0)
                    {
                        if (hasReal)
                        {
                            strcat(StringValue, LessStr);
                        }
                        else
                        {
                            strcpy(StringValue, LessStr);
                        }
                    }

                    sprintf(buffer, precisionFormat, fabs(imagValue));
                    result = strsub(buffer, getCsvDefaultDecimal(), decimal);
                    if (result)
                    {
                        if ((hasReal) || (imagValue < 0))
                        {
                            strncat(StringValue, result, sizeof(StringValue) - strlen(StringValue) - 1);
                            StringValue[sizeof(StringValue) - 1] = '\0';
                        }
                        else
                        {
                            strncpy(StringValue, result, sizeof(StringValue) - 1);
                        }
                        FREE(result);
                    }
                    else
                    {
                        sprintf(StringValue, DEFAULT_CSV_WRITE_DOUBLE_FORMAT, imagValue);
                    }
                    strcat(StringValue, ComplexStr);
                }
            }
            else
            {
                if (hasReal && (signbit(imagValue) == 0))
                {
                    strcat(StringValue, PlusStr);
                }
                else if (signbit(realValue) > 0)
                {
                    if (hasReal)
                    {
                        strcat(StringValue, LessStr);
                    }
                    else
                    {
                        strcpy(StringValue, LessStr);
                    }
                }
                strcat(StringValue, InfString);
                strcat(StringValue, ComplexStr);
            }
            fprintf(fd, DEFAULT_CSV_WRITE_STRING_FORMAT, StringValue);
            if (j + 1 < n)
            {
                fprintf(fd, "%s", separator);
            }
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

    if (filename == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (pStrValues == NULL)
    {
        return CSV_WRITE_ERROR;
    }
    if (m < 0 || n < 0)
    {
        return CSV_WRITE_ERROR;
    }
    if (separator == NULL)
    {
        return CSV_WRITE_ERROR;
    }

    if (strcmp(separator, decimal) == 0)
    {
        return CSV_WRITE_SEPARATOR_DECIMAL_EQUAL;
    }

    expandedFilename = expandPathVariable((char*)filename);
    wcfopen(fd , filename, MODEWFD);
    if (expandedFilename)
    {
        FREE(expandedFilename);
        expandedFilename = NULL;
    }
    if ( fd == (FILE *)NULL )
    {
        return CSV_WRITE_FOPEN_ERROR;
    }

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
            char *result = NULL;
            result = strsub((char*)(pStrValues[i + m * j]), getCsvDefaultDecimal(), decimal);
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

            if (j + 1 < n)
            {
                fprintf(fd, "%s", separator);
            }
        }
        fprintf(fd, "%s", getCsvDefaultEOL());
    }

    fclose(fd);
    return CSV_WRITE_NO_ERROR;
}
// =============================================================================

