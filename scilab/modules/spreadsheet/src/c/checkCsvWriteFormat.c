/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include <string.h>
#include <ctype.h>
#include "csvDefault.h"
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "checkCsvWriteFormat.h"
// =============================================================================
#define NB_FORMAT_SUPPORTED 7
static const char *supportedFormat[NB_FORMAT_SUPPORTED] =
{"lf", "lg", "d", "i", "e", "f", "g"};
// =============================================================================
static char *replaceInFormat(const char *format);
static char *getCleanedFormat(const char *format);
// =============================================================================
int checkCsvWriteFormat(const char *format)
{
    if (format)
    {
        char *tokenPercent1 = strchr((char*)format, '%');
        char *tokenPercent2 = strrchr((char*)format, '%');
        if ((tokenPercent2 && tokenPercent1) && (tokenPercent1 == tokenPercent2))
        {
            char *cleanedFormat = getCleanedFormat(format);
            if (cleanedFormat)
            {
                FREE(cleanedFormat);
                cleanedFormat = NULL;
                return 0;
            }
        }
    }
    return 1;
}
// =============================================================================
static char *replaceInFormat(const char *format)
{
    if (format)
    {
        char *cleanedFormat = getCleanedFormat(format);
        if (cleanedFormat)
        {
            FREE(cleanedFormat);
            cleanedFormat = NULL;
            return strdup("%s");
        }
    }

    return NULL;
}
// =============================================================================
static char *getCleanedFormat(const char *format)
{
    char *cleanedFormat = NULL;
    if (format)
    {
        char *percent = strchr((char*)format, '%');
        if (percent)
        {
            int i = 0;
            for (i = 0; i < NB_FORMAT_SUPPORTED; i++)
            {
                char *token = strstr(percent, supportedFormat[i]);
                if (token)
                {
                    size_t nbcharacters = strlen(percent) - strlen(token);
                    cleanedFormat = strdup(percent);
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
// =============================================================================
