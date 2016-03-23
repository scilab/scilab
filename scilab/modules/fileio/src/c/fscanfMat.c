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
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sci_malloc.h"
#include "fscanfMat.h"
#include "charEncoding.h"
#include "BOOL.h"
#include "localization.h"
#include "Scierror.h"
#include "os_string.h"
#include "freeArrayOfString.h"
#include "mgetl.h"
#include "mopen.h"
#include "mclose.h"
/*--------------------------------------------------------------------------*/
#define EOL "\n"
#define NanString "Nan"
#define InfString "Inf"
#define NegInfString "-Inf"
/*--------------------------------------------------------------------------*/
#if _MSC_VER
#define READ_ONLY_TEXT_MODE L"rt"
#else
#define READ_ONLY_TEXT_MODE L"r"
#endif
/*--------------------------------------------------------------------------*/
#define NB_FORMAT_SUPPORTED 7
static char *supportedFormat[NB_FORMAT_SUPPORTED] =
{"lf", "lg", "d", "i", "e", "f", "g"};
/*--------------------------------------------------------------------------*/
static BOOL itCanBeMatrixLine(char *line, char *format, char *separator);
static int getNbColumnsInLine(char *line, char *format, char *separator);
static int getNumbersColumnsInLines(char **lines, int sizelines,
                                    int nbLinesText,
                                    char *format, char *separator);
static int getNumbersLinesOfText(char **lines, int sizelines,
                                 char *format, char *separator);
static char **splitLine(char *str, char *sep, int *toks, char meta);
static double *getDoubleValuesFromLines(char **lines, int sizelines,
                                        int nbLinesText,
                                        char *format, char *separator,
                                        int m, int n);
static double *getDoubleValuesInLine(char *line,
                                     char *format, char *separator,
                                     int nbColumnsMax);
static double returnINF(BOOL bPositive);
static double returnNAN(void);
static BOOL checkFscanfMatFormat(char *format);
static char *getCleanedFormat(char *format);
static BOOL isOnlyBlankLine(const char *line);
static char **removeEmptyLinesAtTheEnd(char **lines, int *sizelines);
static BOOL isValidLineWithOnlyOneNumber(char *line);
static char ** removeTextLinesAtTheEnd(char **lines, int *sizelines, char *format, char *separator);
/*--------------------------------------------------------------------------*/
fscanfMatResult *fscanfMat(char *filename, char *format, char *separator)
{
    int fd = 0;
    int f_swap = 0;
    double res = 0.0;
    int errMOPEN = MOPEN_INVALID_STATUS;
    int errMGETL = MGETL_ERROR;
    int i = 0;
    int nbLinesTextDetected = 0;
    int nbColumns = 0;
    int nbRows = 0;


    fscanfMatResult *resultFscanfMat = NULL;
    wchar_t **pwsLines = NULL;
    char **lines = NULL;
    int nblines = 0;
    double *dValues = NULL;
    wchar_t* filenameW = NULL;

    if ((filename == NULL) || (format == NULL) || (separator == NULL))
    {
        return NULL;
    }

    if (!checkFscanfMatFormat(format))
    {
        resultFscanfMat = (fscanfMatResult*)(MALLOC(sizeof(fscanfMatResult)));
        if (resultFscanfMat)
        {
            resultFscanfMat->err = FSCANFMAT_FORMAT_ERROR;
            resultFscanfMat->m = 0;
            resultFscanfMat->n = 0;
            resultFscanfMat->sizeText = 0;
            resultFscanfMat->text = NULL;
            resultFscanfMat->values = NULL;
        }
        return resultFscanfMat;
    }

    filenameW = to_wide_string(filename);
    errMOPEN = mopen(filenameW, READ_ONLY_TEXT_MODE, f_swap, &fd);
    FREE(filenameW);
    if (errMOPEN != MOPEN_NO_ERROR)
    {
        resultFscanfMat = (fscanfMatResult*)(MALLOC(sizeof(fscanfMatResult)));
        if (resultFscanfMat)
        {
            resultFscanfMat->err = FSCANFMAT_MOPEN_ERROR;
            resultFscanfMat->m = 0;
            resultFscanfMat->n = 0;
            resultFscanfMat->sizeText = 0;
            resultFscanfMat->text = NULL;
            resultFscanfMat->values = NULL;
        }
        return resultFscanfMat;
    }

    pwsLines = mgetl(fd, -1, &nblines, &errMGETL);
    mclose(fd);

    if (errMGETL != MGETL_NO_ERROR)
    {
        resultFscanfMat = (fscanfMatResult*)(MALLOC(sizeof(fscanfMatResult)));
        if (resultFscanfMat)
        {
            resultFscanfMat->err = FSCANFMAT_READLINES_ERROR;
            resultFscanfMat->m = 0;
            resultFscanfMat->n = 0;
            resultFscanfMat->sizeText = 0;
            resultFscanfMat->text = NULL;
            resultFscanfMat->values = NULL;
        }
        freeArrayOfWideString(pwsLines, nblines);
        return resultFscanfMat;
    }

    lines = (char**)MALLOC(sizeof(char*) * nblines);
    for (i = 0; i < nblines; i++)
    {
        lines[i] = wide_string_to_UTF8(pwsLines[i]);
    }

    freeArrayOfWideString(pwsLines, nblines);

    lines = removeEmptyLinesAtTheEnd(lines, &nblines);
    lines = removeTextLinesAtTheEnd(lines, &nblines, format, separator);

    nbLinesTextDetected = getNumbersLinesOfText(lines, nblines, format, separator);
    nbRows = nblines - nbLinesTextDetected;
    nbColumns = getNumbersColumnsInLines(lines, nblines, nbLinesTextDetected, format, separator);

    dValues = getDoubleValuesFromLines(lines, nblines,
                                       nbLinesTextDetected,
                                       format, separator,
                                       nbColumns, nbRows);
    if (dValues)
    {
        resultFscanfMat = (fscanfMatResult*)(MALLOC(sizeof(fscanfMatResult)));
        if (resultFscanfMat)
        {
            if (nbLinesTextDetected > 0)
            {
                if (lines)
                {
                    for (i = nbLinesTextDetected; i < nblines; i++)
                    {
                        if (lines[i])
                        {
                            FREE(lines[i]);
                            lines[i] = NULL;
                        }
                    }
                }
                resultFscanfMat->text = lines;
            }
            else
            {
                freeArrayOfString(lines, nblines);
                resultFscanfMat->text = NULL;
            }
            resultFscanfMat->sizeText = nbLinesTextDetected;
            resultFscanfMat->m = nbRows;
            resultFscanfMat->n = nbColumns;
            resultFscanfMat->values = dValues;
            resultFscanfMat->err = FSCANFMAT_NO_ERROR;
        }
        else
        {
            FREE(dValues);
            freeArrayOfString(lines, nblines);
        }
    }
    else
    {
        freeArrayOfString(lines, nblines);
        if (nbColumns == 0 || nbRows == 0)
        {
            resultFscanfMat = (fscanfMatResult*)(MALLOC(sizeof(fscanfMatResult)));
            if (resultFscanfMat)
            {
                resultFscanfMat->err = FSCANFMAT_READLINES_ERROR;
                resultFscanfMat->m = 0;
                resultFscanfMat->n = 0;
                resultFscanfMat->sizeText = 0;
                resultFscanfMat->text = NULL;
                resultFscanfMat->values = NULL;
            }
        }
    }
    return resultFscanfMat;
}
/*--------------------------------------------------------------------------*/
void freeFscanfMatResult(fscanfMatResult *resultStruct)
{
    if (resultStruct)
    {
        if (resultStruct->text)
        {
            freeArrayOfString(resultStruct->text, resultStruct->sizeText);
            resultStruct->text = NULL;
        }

        if (resultStruct->values)
        {
            FREE(resultStruct->values);
            resultStruct->values = NULL;
        }

        resultStruct->err = FSCANFMAT_ERROR;
        resultStruct->m = 0;
        resultStruct->n = 0;
        resultStruct->sizeText = 0;

        FREE(resultStruct);
        resultStruct = NULL;
    }
}
/*--------------------------------------------------------------------------*/
static BOOL itCanBeMatrixLine(char *line, char *format, char *separator)
{
    if (line)
    {
        int ierr = 0;
        double dValue = 0.;

        ierr = sscanf(line, format, &dValue);

        if ((ierr == EOF) || (ierr == 0))
        {
            char *str = os_strdup(line);
            if (str)
            {
                ierr = sscanf(line, "%4s", str);

                if ((ierr != 0) && (ierr != EOF))
                {
                    if ((strncmp(str, NanString, (int)strlen(NanString)) == 0) ||
                            (strncmp(str, NegInfString, (int)strlen(NegInfString)) == 0) ||
                            (strncmp(str, InfString, (int)strlen(InfString)) == 0))
                    {
                        FREE(str);
                        str = NULL;
                        return TRUE;
                    }
                }
                FREE(str);
                str = NULL;
            }
        }
        else
        {
            return TRUE;
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static int getNumbersLinesOfText(char **lines, int sizelines,
                                 char *format, char *separator)
{
    int numberOfLines = 0;
    if (lines)
    {
        int i = 0;
        for (i = 0; i < sizelines; i++)
        {
            if (!itCanBeMatrixLine(lines[i], format, separator))
            {
                numberOfLines++;
            }
            else
            {
                return numberOfLines;
            }
        }
    }
    return numberOfLines;
}
/*--------------------------------------------------------------------------*/
static int getNumbersColumnsInLines(char **lines, int sizelines,
                                    int nbLinesText,
                                    char *format, char *separator)
{
    int previousNbColumns = 0;
    int NbColumns = 0;
    BOOL firstLine = TRUE;
    if (lines)
    {
        int i = 0;
        int firstLinesMatrix = nbLinesText;
        for (i = firstLinesMatrix; i < sizelines; i++)
        {
            NbColumns = getNbColumnsInLine(lines[i], format, separator);
            if (firstLine)
            {
                previousNbColumns = NbColumns;
                firstLine = FALSE;
            }
            else
            {
                if (previousNbColumns != NbColumns)
                {
                    return 0;
                }
            }
        }
    }
    return NbColumns;
}
/*--------------------------------------------------------------------------*/
static int getNbColumnsInLine(char *line, char *format, char *separator)
{
    int nbColums = 0;

    if (line && format && separator)
    {
        int i = 0;
        int nbTokens = 0;
        char **splittedStr = splitLine(line, separator, &nbTokens, 0);
        if (nbTokens == 0)
        {
            freeArrayOfString(splittedStr, nbTokens);
            return nbColums;
        }
        if (splittedStr)
        {
            for (i = 0; i < nbTokens; i++)
            {
                double dValue = 0.;
                int ierr = sscanf(splittedStr[i], format, &dValue);
                if ((ierr != 0) && (ierr != EOF))
                {
                    nbColums++;
                }
                else
                {
                    char *str = os_strdup(splittedStr[i]);
                    strcpy(str, "");

                    ierr = sscanf(splittedStr[i], "%4s", str);

                    if ((ierr != 0) && (ierr != EOF))
                    {
                        if ( (strcmp(str, NanString) == 0) ||
                                (strcmp(str, NegInfString) == 0) ||
                                (strcmp(str, InfString) == 0) )
                        {
                            nbColums++;
                        }
                        else
                        {
                            freeArrayOfString(splittedStr, nbTokens);
                            /* bug 6889 */
                            if (nbColums)
                            {
                                nbColums--;
                            }
                            FREE(str);
                            str = NULL;
                            return nbColums;
                        }
                    }
                    else
                    {
                        FREE(str);
                        str = NULL;
                        freeArrayOfString(splittedStr, nbTokens);
                        return nbColums;
                    }

                    FREE(str);
                    str = NULL;
                }
            }
            freeArrayOfString(splittedStr, nbTokens);
        }
    }
    return nbColums;
}
/*--------------------------------------------------------------------------*/
static char **splitLine(char *str, char *sep, int *toks, char meta)
{
    char **retstr = NULL;
    char *idx = NULL;
    char *end = NULL;
    char *sep_end = NULL;
    char *sep_idx = NULL;
    int len = 0;
    int curr_str = 0;
    char last_char = 0xFF;

    *toks = 0;

    if ((sep == NULL) || (str == NULL))
    {
        return NULL;
    }

    sep_end = sep + strlen(sep);
    end = str + strlen(str);

    sep_idx = sep;
    idx = str;

    if (strstr(str, sep) == NULL)
    {
        if ((int)strlen(str) > 0)
        {
            if (isValidLineWithOnlyOneNumber(str))
            {
                retstr = (char **) MALLOC(sizeof(char *));
                if (retstr)
                {
                    retstr[0] = os_strdup(str);
                    *toks = 1;
                }
            }
        }
        return retstr;
    }

    retstr = (char **) MALLOC((sizeof(char *) * (int)strlen(str)));
    if (retstr == NULL)
    {
        *toks = 0;
        return NULL;
    }

    while (idx < end)
    {
        while (sep_idx < sep_end)
        {
            if ((*idx == *sep_idx) && (last_char != meta))
            {
                if (len > 0)
                {
                    if (curr_str < (int)strlen(str))
                    {
                        retstr[curr_str] = (char *) MALLOC((sizeof(char) * len) + 1);

                        if (retstr[curr_str] == NULL)
                        {
                            freeArrayOfString(retstr, curr_str);
                            *toks = 0;
                            return NULL;
                        }
                        memcpy(retstr[curr_str], (idx - len), len);
                        retstr[curr_str][len] = 0;
                        len = 0;
                        curr_str++;
                        last_char = *idx;
                        idx++;
                    }

                    if (curr_str >= (int)strlen(str))
                    {
                        *toks = curr_str + 1;
                        return retstr;
                    }
                }
                else
                {
                    last_char = *idx;
                    idx++;
                    sep_idx = sep;
                    len = 0;
                }
            }
            else
            {
                sep_idx++;
            }
        }

        sep_idx = sep;
        len++;
        last_char = *idx;
        idx++;
    }

    if (len > 0)
    {
        retstr[curr_str] = (char *) MALLOC((sizeof(char) * len) + 1);

        if (retstr[curr_str] == NULL)
        {
            *toks = 0;
            FREE(retstr);
            return NULL;
        }

        memcpy(retstr[curr_str], (idx - len), len);
        retstr[curr_str][len] = 0;

        *toks = curr_str + 1;
    }
    return retstr;
}
/*--------------------------------------------------------------------------*/
static double *getDoubleValuesFromLines(char **lines, int sizelines,
                                        int nbLinesText,
                                        char *format, char *separator,
                                        int m, int n)
{
    double *dValues = NULL;

    if (m == 0 || n == 0)
    {
        return NULL;
    }

    dValues = (double*) MALLOC(sizeof(double) * (m * n));
    if (dValues)
    {
        int i = 0;
        int firstLinesMatrix = nbLinesText;
        for (i = firstLinesMatrix; i < sizelines; i++)
        {
            double *dValsTmp = getDoubleValuesInLine(lines[i], format, separator, m);
            if (dValsTmp)
            {
                int j = 0;
                for (j = 0; j < m; j++)
                {
                    dValues[(i - firstLinesMatrix) + n * j] = dValsTmp[j];
                }
                FREE(dValsTmp);
                dValsTmp = NULL;
            }
        }
    }
    return dValues;
}
/*--------------------------------------------------------------------------*/
static double *getDoubleValuesInLine(char *line,
                                     char *format, char *separator,
                                     int nbColumnsMax)
{
    double *dValues = NULL;

    if (line && format && separator)
    {
        int nbTokens = 0;
        char **splittedStr = splitLine(line, separator, &nbTokens, 0);
        if (splittedStr)
        {
            int i = 0;
            dValues = (double*)MALLOC(sizeof(double) * nbColumnsMax);
            for (i = 0; i < nbColumnsMax; i++)
            {
                int ierr = 0;
                double dValue = 0.;
                char *cleanedFormat = getCleanedFormat(format);
                int iLen = strlen(cleanedFormat);
                switch (cleanedFormat[iLen - 1])
                {
                    case 'e' :
                    case 'g' :
                    case 'f' :
                    {
                        if (cleanedFormat[iLen - 2] == 'l')
                        {
                            double tmp = 0.;
                            ierr = sscanf(splittedStr[i], cleanedFormat, &tmp);
                            dValue = tmp;
                        }
                        else
                        {
                            float tmp = 0.;
                            ierr = sscanf(splittedStr[i], cleanedFormat, &tmp);
                            dValue = tmp;
                        }
                        break;
                    }
                    case 'd' :
                    case 'i' :
                    {
                        int tmp = 0;
                        ierr = sscanf(splittedStr[i], cleanedFormat, &tmp);
                        dValue = tmp;
                        break;
                    }
                }

                FREE(cleanedFormat);

                if ((ierr != 0) && (ierr != EOF))
                {
                    dValues[i] = dValue;
                }
                else
                {
                    char *str = os_strdup(line);
                    strcpy(str, "");
                    ierr = sscanf(splittedStr[i], "%4s", str);
                    if ((ierr != 0) && (ierr != EOF))
                    {
                        if ( (strcmp(str, NanString) == 0) ||
                                (strcmp(str, NegInfString) == 0) ||
                                (strcmp(str, InfString) == 0) )
                        {
                            if (strcmp(str, NanString) == 0)
                            {
                                dValues[i] = returnNAN();
                            }

                            if (strcmp(str, NegInfString) == 0)
                            {
                                dValues[i] = returnINF(FALSE);
                            }

                            if (strcmp(str, InfString) == 0)
                            {
                                dValues[i] = returnINF(TRUE);
                            }
                        }
                        else
                        {
                            freeArrayOfString(splittedStr, nbTokens);
                            FREE(dValues);
                            dValues = NULL;
                            FREE(str);
                            str = NULL;
                            return NULL;
                        }
                    }
                    else
                    {
                        freeArrayOfString(splittedStr, nbTokens);
                        FREE(dValues);
                        dValues = NULL;
                        FREE(str);
                        str = NULL;
                        return NULL;
                    }
                    FREE(str);
                    str = NULL;
                }
            }
            freeArrayOfString(splittedStr, nbTokens);
        }
    }

    return dValues;
}
/*--------------------------------------------------------------------------*/
static double returnINF(BOOL bPositive)
{
    double v = 0;
    double p = 10;
    if (!bPositive)
    {
        p = -10;
    }
    return (double) p / (double)v;
}
/*--------------------------------------------------------------------------*/
static double returnNAN(void)
{
    static int first = 1;
    static double nan = 1.0;

    if ( first )
    {
        nan = (nan - (double) first) / (nan - (double) first);
        first = 0;
    }
    return (nan);
}
/*--------------------------------------------------------------------------*/
static BOOL checkFscanfMatFormat(char *format)
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
                    if ( (nbcharacters - 1 > 0) && (isdigit(cleanedFormat[nbcharacters - 1]) ||
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
static char **removeEmptyLinesAtTheEnd(char **lines, int *sizelines)
{
    int i = 0;
    int nbLinesToRemove = 0;
    if (lines)
    {
        for (i = *sizelines - 1; i >= 0; i--)
        {
            if (lines[i])
            {
                if ( (strcmp(lines[i], "") == 0) || (isOnlyBlankLine(lines[i])) )
                {
                    FREE(lines[i]);
                    lines[i] = NULL;
                    nbLinesToRemove++;
                }
                else
                {
                    break;
                }
            }
        }

        if (nbLinesToRemove > 0)
        {
            lines = (char**)REALLOC(lines, sizeof(char*) * (*sizelines - nbLinesToRemove));
            *sizelines = *sizelines - nbLinesToRemove;
        }
    }
    else
    {
        *sizelines = 0;
    }
    return lines;
}
/*--------------------------------------------------------------------------*/
static BOOL isValidLineWithOnlyOneNumber(char *line)
{
    if (line)
    {
        char *pEnd = NULL;
        strtod(line, &pEnd);
        if ((pEnd) && ((int)strlen(pEnd) == 0))
        {
            return TRUE;
        }
        else
        {
            if ((strncmp(line, NanString, (int)strlen(NanString)) == 0) ||
                    (strncmp(line, NegInfString, (int)strlen(NegInfString)) == 0) ||
                    (strncmp(line, InfString, (int)strlen(InfString)) == 0))
            {
                return TRUE;
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
static BOOL isOnlyBlankLine(const char *line)
{
    if (line)
    {
        int i = 0;
        for (i = 0; i < (int) strlen(line); i++)
        {
            if (line[i] != ' ')
            {
                return FALSE;
            }
        }
    }
    return TRUE;
}
/*--------------------------------------------------------------------------*/
static char ** removeTextLinesAtTheEnd(char **lines, int *sizelines, char *format, char *separator)
{
    char **linesReturned = NULL;
    int i = 0;
    int nbLinesToRemove = 0;

    for (i = *sizelines - 1; i > 0; i--)
    {
        if (itCanBeMatrixLine(lines[i], format, separator) == FALSE)
        {
            nbLinesToRemove++;
            FREE(lines[i]);
            lines[i] = NULL;
        }
        else
        {
            break;
        }
    }

    if (nbLinesToRemove > 0)
    {
        //must free last lines.
        *sizelines = *sizelines - nbLinesToRemove;
        linesReturned = lines;
    }
    else
    {
        linesReturned = lines;
    }

    return linesReturned;

}
/*--------------------------------------------------------------------------*/
