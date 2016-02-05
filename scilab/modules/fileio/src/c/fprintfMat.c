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
/*--------------------------------------------------------------------------*/
#if defined(__linux__)
#define _GNU_SOURCE /* Bug 5673 fix: avoid dependency on GLIBC_2.7 */
#endif
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <ctype.h> /* isdigit */
#include <string.h>
#include "fprintfMat.h"
#include "charEncoding.h"
#include "sci_malloc.h"
#include "core_math.h"
#include "strsubst.h"
#include "BOOL.h"
#include "os_string.h"

/*--------------------------------------------------------------------------*/
#define EOL "\n"
#define NanString "Nan"
#define InfString "Inf"
#define NegInfString "-Inf"
/*--------------------------------------------------------------------------*/
#define NB_FORMAT_SUPPORTED 7
static char *supportedFormat[NB_FORMAT_SUPPORTED] =
{"lf", "lg", "d", "i", "e", "f", "g"};
/*--------------------------------------------------------------------------*/
static BOOL checkFprintfMatFormat(char *format);
static char *getCleanedFormat(char *format);
static char *replaceInFormat(char *format);
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
fprintfMatError fprintfMat(char *filename, char *format, char *separator,
                           double *MatrixValues, int m, int n,
                           char **textAdded, int sizeTextAdded)
{
#if _MSC_VER
#define MODEFD "wt"
#else
#define MODEFD "w"
#endif
    int i = 0;
    FILE  *fd = NULL;

    if (filename == NULL)
    {
        return FPRINTMAT_NULL_POINTER_ERROR;
    }
    if (format == NULL)
    {
        return FPRINTMAT_NULL_POINTER_ERROR;
    }
    if (separator == NULL)
    {
        return FPRINTMAT_NULL_POINTER_ERROR;
    }
    if ((sizeTextAdded > 0) && (textAdded == NULL))
    {
        return FPRINTFMAT_ERROR;
    }
    if ((m > 0) && (n > 0) && (MatrixValues == NULL))
    {
        return FPRINTFMAT_ERROR;
    }

    if (!checkFprintfMatFormat(format))
    {
        return FPRINTMAT_FORMAT_ERROR;
    }

    wcfopen(fd , filename, MODEFD);

    if ( fd == (FILE *)NULL )
    {
        return FPRINTFMAT_FOPEN_ERROR;
    }

    if (sizeTextAdded > 0)
    {
        if (textAdded)
        {
            for (i = 0; i < sizeTextAdded; i++)
            {
                if (textAdded[i])
                {
                    fprintf(fd, "%s\n", textAdded[i]);
                }
            }
        }
    }

    for (i = 0 ; i < m ; i++ )
    {
        int j = 0;
        for ( j = 0 ; j < n ; j++)
        {
            if (ISNAN(MatrixValues[i + m * j]))
            {
                char *localFormat = replaceInFormat(format);
                if (localFormat)
                {
                    fprintf(fd, localFormat, NanString);
                    FREE(localFormat);
                    localFormat = NULL;
                }
                else
                {
                    fprintf(fd, format, MatrixValues[i + m * j]);
                }
            }
            else if (finite(MatrixValues[i + m * j]))
            {
                fprintf(fd, format, MatrixValues[i + m * j]);
            }
            else
            {
                if ( signbit(MatrixValues[i + m * j]) )
                {
                    char *localFormat = replaceInFormat(format);
                    if (localFormat)
                    {
                        fprintf(fd, localFormat, NegInfString);
                        FREE(localFormat);
                        localFormat = NULL;
                    }
                    else
                    {
                        fprintf(fd, format, MatrixValues[i + m * j]);
                    }
                }
                else
                {
                    char *localFormat = replaceInFormat(format);
                    if (localFormat)
                    {
                        fprintf(fd, localFormat, InfString);
                        FREE(localFormat);
                        localFormat = NULL;
                    }
                    else
                    {
                        fprintf(fd, format, MatrixValues[i + m * j]);
                    }
                }
            }

            fprintf(fd, "%s", separator);
        }
        fprintf(fd, EOL);
    }

    fclose(fd);

    return FPRINTFMAT_NO_ERROR;
}
/*--------------------------------------------------------------------------*/
static BOOL checkFprintfMatFormat(char *format)
{
    if (format)
    {
        char *tokenPercent1 = strchr(format, '%');
        char *tokenPercent2 = strrchr(format, '%');
        if ((tokenPercent2 && tokenPercent1) && (tokenPercent1 == tokenPercent2))
        {
            char *cleanedFormat = getCleanedFormat(format);
            if (cleanedFormat)
            {
                FREE(cleanedFormat);
                cleanedFormat = NULL;
                return TRUE;
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static char *replaceInFormat(char *format)
{
    char *newFormat = NULL;
    if (format)
    {
        char *cleanedFormat = getCleanedFormat(format);
        if (cleanedFormat)
        {
            newFormat = strsub(format, cleanedFormat, "%s");
            FREE(cleanedFormat);
            cleanedFormat = NULL;
        }
    }
    return newFormat;
}
/*--------------------------------------------------------------------------*/
static char *getCleanedFormat(char *format)
{
    char *cleanedFormat = NULL;
    if (format)
    {
        char *percent = strchr(format, '%');
        if (percent)
        {
            int i = 0;
            for (i = 0; i < NB_FORMAT_SUPPORTED; i++)
            {
                char *token = strstr(percent, supportedFormat[i]);
                if (token)
                {
                    int nbcharacters = (int)(strlen(percent) - strlen(token));
                    cleanedFormat = os_strdup(percent);
                    cleanedFormat[nbcharacters] = 0;
                    if ( ((nbcharacters - 1 > 0) && (isdigit(cleanedFormat[nbcharacters - 1])) ||
                            (cleanedFormat[nbcharacters - 1]) == '.') ||
                            (cleanedFormat[nbcharacters - 1]) == '%')
                    {
                        strcat(cleanedFormat, supportedFormat[i]);
                        return cleanedFormat;
                    }
                    else
                    {
                        FREE(cleanedFormat);
                        cleanedFormat = NULL;
                    }
                }
            }
        }
    }
    return cleanedFormat;
}
/*--------------------------------------------------------------------------*/
