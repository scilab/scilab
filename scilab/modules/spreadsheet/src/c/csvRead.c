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
#include <string.h>
#include <stdio.h>
#include "csvRead.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "mopen.h"
#include "mgetl.h"
#include "localization.h"
#include "expandPathVariable.h"
#include "FileExist.h"
#include "mclose.h"
#include "configvariable_interface.h"
#include "pcre_private.h"
#include "sciprint.h"
#include "splitLine.h"
#include "os_string.h"
#include "csvDefault.h"
#include "strsubst.h"
#include "Sciwarning.h"
// =============================================================================
#if _MSC_VER
#define READ_ONLY_TEXT_MODE "rt"
#else
#define READ_ONLY_TEXT_MODE "r"
#endif
// =============================================================================
static int getNumbersOfColumnsInLines(const char **lines, int sizelines,
                                      const char *separator);
static int getNumbersOfColumnsInLine(const char *line, const char *separator);
static char **getStringsFromLines(const char **lines, int sizelines,
                                  const char *separator, const char *decimal,
                                  int m, int n);
static char **removeEmptyLinesAtTheEnd(const char **lines, int *sizelines);
static char *stripCharacters(const char *line);
static char **replaceStrings(const char **lines, int nbLines, const char **toreplace, int sizetoreplace);
static char **extractComments(const char **lines, int nbLines, const char *regexpcomments, int *nbcomments, int *iErr);
static char **removeComments(const char **lines, int nbLines, const char *regexpcomments, int *nbNewLine, int *iErr);
static char **removeAllBlankLines(const char **lines, int *sizelines);
// =============================================================================
csvResult* csvRead(const char *filename, const char *separator, const char *decimal, const char **toreplace, int sizetoreplace, const char *regexpcomments, int header)
{
    wchar_t *expandedFilename = NULL;
    wchar_t *wideFilename = NULL;
    csvResult *result = NULL;
    int fd = 0;
    int f_swap = 0;
    double res = 0.0;
    int errMOPEN = MOPEN_INVALID_STATUS;
    int errMGETL = MGETL_ERROR;
    wchar_t **pwstLines = NULL;
    char **pstLines = NULL;
    int nblines = 0;
    char **replacedInLines = NULL;
    char **pComments = NULL;
    int nbComments = 0;

    if ((filename == NULL) || (separator == NULL) || (decimal == NULL))
    {
        return NULL;
    }

    wideFilename = to_wide_string((char*)filename);
    expandedFilename = expandPathVariableW(wideFilename);
    FREE(wideFilename);

    if (!FileExistW(expandedFilename))
    {
        result = (csvResult*)(MALLOC(sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_FILE_NOT_EXIST;
            result->m = 0;
            result->n = 0;
            result->pstrValues = NULL;
            result->pstrComments = NULL;
            result->nbComments = 0;
        }

        FREE(expandedFilename);
        return result;
    }

    errMOPEN = mopen(expandedFilename, L"rt", f_swap, &fd); // rt = read only
    if (expandedFilename)
    {
        FREE(expandedFilename);
        expandedFilename = NULL;
    }

    if (errMOPEN != MOPEN_NO_ERROR)
    {
        result = (csvResult*)(MALLOC(sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_MOPEN_ERROR;
            result->m = 0;
            result->n = 0;
            result->pstrValues = NULL;
            result->pstrComments = NULL;
            result->nbComments = 0;

        }
        return result;
    }

    if (header != 0)
    {
        // Ignoring the header
        pstLines = mgetl(fd, header, &nblines, &errMGETL);
        freeArrayOfWideString(pstLines, nblines);
        pstLines = NULL;
    }

    pwstLines = mgetl(fd, -1, &nblines, &errMGETL);

    mclose(fd);

    if (errMGETL != MGETL_NO_ERROR)
    {
        result = (csvResult*)(MALLOC(sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_READLINES_ERROR;
            result->m = 0;
            result->n = 0;
            result->pstrValues = NULL;
            result->pstrComments = NULL;
            result->nbComments = 0;
        }
        return result;
    }

    pstLines = (char**)MALLOC(sizeof(char*) * nblines);

    {
        int i = 0;
        for (i = 0 ; i < nblines ; i++)
        {
            pstLines[i] = wide_string_to_UTF8(pwstLines[i]);
        }

    }
    freeArrayOfWideString(pwstLines, nblines);
    pwstLines = NULL;

    if (regexpcomments)
    {
        int iErr = 0;

        pComments = extractComments((const char**)pstLines, nblines, regexpcomments, &nbComments, &iErr);

        if ((iErr == CAN_NOT_COMPILE_PATTERN) || (iErr == DELIMITER_NOT_ALPHANUMERIC))
        {
            result = (csvResult*)(MALLOC(sizeof(csvResult)));
            if (result)
            {
                if ((iErr == CAN_NOT_COMPILE_PATTERN) || (iErr == DELIMITER_NOT_ALPHANUMERIC))
                {
                    iErr = CSV_READ_REGEXP_ERROR;
                }
                result->err = (csvReadError)iErr;
                result->m = 0;
                result->n = 0;
                result->pstrValues = NULL;
                result->pstrComments = NULL;
                result->nbComments = 0;
            }
            freeArrayOfString(pstLines, nblines);
            return result;
        }

        if (pComments)
        {
            char **pCleanedLines = NULL;
            int nbCleanedLines = 0;
            int i = 0;

            pCleanedLines = removeComments((const char**)pstLines, nblines, (const char*)regexpcomments, &nbCleanedLines, &iErr);
            if (pCleanedLines)
            {
                FREE(pstLines);
                pstLines = pCleanedLines;
                nblines = nbCleanedLines;
            }

        }
    }

    if (toreplace && (sizetoreplace > 0))
    {
        replacedInLines = replaceStrings((const char**)pstLines, nblines, toreplace, sizetoreplace);
        if (replacedInLines)
        {
            freeArrayOfString(pstLines, nblines);
            pstLines = replacedInLines;
        }
    }

    result = csvTextScan((const char**)pstLines, nblines, (const char*)separator, (const char*)decimal);
    freeArrayOfString(pstLines, nblines);

    if (result)
    {
        result->pstrComments = pComments;
        result->nbComments = nbComments;
    }
    else
    {
        freeArrayOfString(pComments, nbComments);
    }


    return result;
}
// =============================================================================
csvResult* csvTextScan(const char **lines, int numberOfLines, const char *separator, const char *decimal)
{
    csvResult *result = NULL;
    int nbRows = 0;
    int nbColumns = 0;
    char **cellsStrings = NULL;
    char **cleanedLines = NULL;
    int nbLines = numberOfLines;

    if (strcmp(separator, decimal) == 0)
    {
        result = (csvResult*)(MALLOC(sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_SEPARATOR_DECIMAL_EQUAL;
            result->m = 0;
            result->n = 0;
            result->pstrValues = NULL;
            result->pstrComments = NULL;
            result->nbComments = 0;
        }
        return result;
    }

    // ticket 472
    {
        const char *blankMode = getCsvDefaultCsvIgnoreBlankLine();
        if (strcmp(blankMode, "on") == 0)
        {
            char **tmpLines = removeAllBlankLines(lines, &nbLines);
            if (tmpLines)
            {
                freeArrayOfString(cleanedLines, nbLines);
                cleanedLines = tmpLines;
            }
        }
        else
        {
            /* remove last lines empty (bug 7003 in scilab)*/
            cleanedLines = removeEmptyLinesAtTheEnd(lines, &nbLines);
        }
    }

    nbColumns = getNumbersOfColumnsInLines((const char **)cleanedLines, nbLines, separator);
    if (nbColumns == 0)
    {
        result = (csvResult*)(MALLOC(sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_COLUMNS_ERROR;
            result->m = 0;
            result->n = 0;
            result->pstrValues = NULL;
            result->pstrComments = NULL;
            result->nbComments = 0;
        }
        FREE(cleanedLines);
        return result;
    }
    else
    {
        nbRows = nbLines;
    }

    cellsStrings = getStringsFromLines((const char **)cleanedLines, nbLines, separator, decimal, nbColumns, nbRows);
    if (cleanedLines)
    {
        freeArrayOfString(cleanedLines, nbLines);
        cleanedLines = NULL;
    }

    if (cellsStrings)
    {
        result = (csvResult*)(MALLOC(sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_NO_ERROR;
            result->m = nbRows;
            result->n = nbColumns;
            result->pstrValues = cellsStrings;
            result->pstrComments = NULL;
            result->nbComments = 0;
        }
        else
        {
            FREE(cellsStrings);
        }
    }
    else
    {
        result = (csvResult*)(MALLOC(sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_COLUMNS_ERROR;
            result->m = 0;
            result->n = 0;
            result->pstrValues = NULL;
            result->pstrComments = NULL;
            result->nbComments = 0;
        }
    }
    return result;
}
// =============================================================================
void freeCsvResult(csvResult *result)
{
    if (result)
    {
        if (result->pstrValues)
        {
            freeArrayOfString(result->pstrValues, result->m * result->n);
            result->pstrValues = NULL;
        }
        result->m = 0;
        result->n = 0;

        if (result->pstrComments)
        {
            freeArrayOfString(result->pstrComments, result->nbComments);
            result->pstrComments = NULL;
        }
        result->err = CSV_READ_ERROR;
        FREE(result);
        result = NULL;
    }
}
// =============================================================================
static int getNumbersOfColumnsInLines(const char **lines, int sizelines,
                                      const char *separator)
{
    int previousNbColumns = 0;
    int NbColumns = 0;
    BOOL firstLine = TRUE;
    if (lines)
    {
        int i = 0;
        for (i = 0; i < sizelines; i++)
        {
            NbColumns = getNumbersOfColumnsInLine(lines[i], separator);
            if (firstLine)
            {
                previousNbColumns = NbColumns;
                firstLine = FALSE;
            }
            else
            {
                if (previousNbColumns != NbColumns)
                {
                    Sciwarning(_("%s: Inconsistency found in the columns. At line %d, found %d columns while the previous had %d.\n"), _("Warning"), i + 1, NbColumns, previousNbColumns);
                    return 0;
                }
            }
        }
    }
    return NbColumns;
}
// =============================================================================
static int getNumbersOfColumnsInLine(const char *line, const char *separator)
{
    if (line && separator)
    {
        int i = 0;
        int nbTokens = 0;
        char **splittedStr = splitLineCSV(line, separator, &nbTokens);
        if (splittedStr)
        {
            freeArrayOfString(splittedStr, nbTokens);
            return nbTokens;
        }
        else
        {
            int len = (int)strlen(line);
            if (len > 0)
            {
                nbTokens = 1;
                return nbTokens;
            }
        }
    }
    return 0;
}
// =============================================================================
static char **getStringsFromLines(const char **lines, int sizelines,
                                  const char *separator,
                                  const char *decimal,
                                  int m, int n)
{
    char **results = NULL;

    if (lines == NULL)
    {
        return NULL;
    }
    if (separator == NULL)
    {
        return NULL;
    }
    if (m == 0 || n == 0)
    {
        return NULL;
    }

    results = (char**) MALLOC(sizeof(char*) * (m * n));
    if (results)
    {
        int i = 0;
        for (i = 0; i < sizelines; i++)
        {
            int nbTokens = 0;
            char **lineStrings = splitLineCSV(lines[i], separator, &nbTokens);
            int j = 0;

            if (lineStrings == NULL)
            {
                lineStrings = (char**)MALLOC(sizeof(char*) * 1);
                lineStrings[0] = os_strdup(lines[i]);
                nbTokens = 1;
            }

            if (m != nbTokens)
            {
                freeArrayOfString(results, nbTokens * n);
                FREE(lineStrings);
                return NULL;
            }

            for (j = 0; j < m; j++)
            {

                if (!decimal)
                {
                    results[i + n * j] = os_strdup(lineStrings[j]);
                }
                else
                {
                    /* Proceed to the remplacement of the provided decimal to the default on
                     * usually, it converts "," => "." */
                    results[i + n * j] = strsub(lineStrings[j], decimal, getCsvDefaultDecimal());
                }

                if (lineStrings[j])
                {
                    FREE(lineStrings[j]);
                    lineStrings[j] = NULL;
                }
            }
            FREE(lineStrings);
        }
    }
    return results;
}
// =============================================================================
static char **removeEmptyLinesAtTheEnd(const char **lines, int *sizelines)
{
    char **returnedLines = NULL;
    int nbLinesToRemove = 0;

    if (lines)
    {
        int i = 0;
        if (*sizelines >= 1)
        {
            for (i = *sizelines - 1; i >= 0; i--)
            {
                char *cleanedLine = stripCharacters(lines[i]);
                if (cleanedLine)
                {
                    int len = (int) strlen(cleanedLine);
                    FREE(cleanedLine);
                    cleanedLine = NULL;
                    if (len == 0)
                    {
                        nbLinesToRemove++;
                        FREE((char*)lines[i]);
                        lines[i] = NULL;
                    }
                    else
                    {
                        break;
                    }
                }
            }

            if (nbLinesToRemove > 0)
            {
                *sizelines = *sizelines - nbLinesToRemove;
            }
            returnedLines = (char **)MALLOC(sizeof(char *) * (*sizelines));
            if (returnedLines)
            {
                for (i = 0; i < *sizelines; i++)
                {
                    returnedLines[i] = os_strdup(lines[i]);
                }
            }
        }
    }

    return returnedLines;
}
// =============================================================================
static char **removeAllBlankLines(const char **lines, int *sizelines)
{
    char **returnedLines = NULL;
    int nbLines = 0;
    if (lines)
    {
        int i = 0;
        for (i = 0; i < *sizelines; i++)
        {
            char *cleanedLine = stripCharacters(lines[i]);
            if (cleanedLine)
            {
                int len = (int) strlen(cleanedLine);
                FREE(cleanedLine);
                cleanedLine = NULL;
                if (len != 0)
                {
                    if (nbLines == 0)
                    {
                        nbLines++;
                        returnedLines = (char**)MALLOC(sizeof(char*) * nbLines);
                    }
                    else
                    {
                        nbLines++;
                        returnedLines = (char**)REALLOC(returnedLines, sizeof(char*) * nbLines);
                    }

                    if (returnedLines)
                    {
                        returnedLines[nbLines - 1] = os_strdup(lines[i]);
                    }
                    else
                    {
                        *sizelines = 0;
                        return NULL;
                    }
                }
            }
        }
        *sizelines = nbLines;
    }
    return returnedLines;
}
// =============================================================================
static char *stripCharacters(const char *line)
{
    char *returnedLine = NULL;
    if (line)
    {
        char *tmpLineWithoutTab = strsub((char*)line, "\t", "");
        if (tmpLineWithoutTab)
        {
            char *tmpLineWithoutLF = strsub(tmpLineWithoutTab, "\r", "");
            if (tmpLineWithoutLF)
            {
                char *tmpLineWithoutCR = strsub(tmpLineWithoutTab, "\n", "");
                if (tmpLineWithoutCR)
                {
                    returnedLine = strsub(tmpLineWithoutCR, " ", "");
                    FREE(tmpLineWithoutCR);
                }
                else
                {
                    returnedLine = os_strdup(line);
                }
                FREE(tmpLineWithoutLF);
                tmpLineWithoutLF = NULL;
            }
            else
            {
                returnedLine = os_strdup(line);
            }
            FREE(tmpLineWithoutTab);
            tmpLineWithoutTab = NULL;
        }
        else
        {
            returnedLine = os_strdup(line);
        }
    }

    return returnedLine;
}
// =============================================================================
static char **replaceStrings(const char **lines, int nbLines, const char **toreplace, int sizetoreplace)
{
    char **replacedStrings = NULL;
    int nr = 0;

    nr = sizetoreplace / 2;

    if (lines)
    {
        int i = 0;

        replacedStrings = (char**)MALLOC(sizeof(char*) * nbLines);
        if (replacedStrings)
        {
            // Copy the source lines to the target replacedStrings.
            int j = 0;
            for (j = 0; j < nbLines; j++)
            {
                replacedStrings[j] = os_strdup(lines[j]);
            }
            // Make replacements within the target replacedStrings.
            for (i = 0; i < nr; i++)
            {
                for (j = 0; j < nbLines; j++)
                {
                    replacedStrings[j] = strsub(replacedStrings[j], toreplace[i], toreplace[nr + i]);
                }
            }
        }
    }
    return replacedStrings;
}
// =============================================================================
static char **extractComments(const char **lines, int nbLines,
                              const char *regexpcomments, int *nbcomments, int *iErr)
{
    char **pComments = NULL;
    int i = 0;

    for (i = 0; i < nbLines; i++)
    {
        int Output_Start = 0;
        int Output_End = 0;
        pcre_error_code answer = pcre_private((char*)lines[i], (char*)regexpcomments, &Output_Start, &Output_End, NULL, NULL);

        if ( (answer == CAN_NOT_COMPILE_PATTERN) || (answer == DELIMITER_NOT_ALPHANUMERIC))
        {
            if (pComments)
            {
                freeArrayOfString(pComments, *nbcomments);
            }

            *nbcomments = 0;

            *iErr = answer;
            return NULL;
        }

        if ( answer == PCRE_FINISHED_OK )
        {
            (*nbcomments)++;
            if (pComments == NULL)
            {
                pComments = (char **)MALLOC(sizeof(char*) * (*nbcomments));
            }
            else
            {
                pComments = (char **)REALLOC(pComments, sizeof(char*) * (*nbcomments));
            }

            if (pComments == NULL)
            {
                *nbcomments = 0;
                *iErr = 1;
                return NULL;
            }
            pComments[(*nbcomments) - 1] = os_strdup(lines[i]);
        }
    }

    return pComments;
}
// =============================================================================
static char **removeComments(const char **lines, int nbLines,
                             const char *regexpcomments, int *newNbLines, int *iErr)
{
    char **pLinesCleaned = NULL;

    int i = 0;
    *newNbLines = 0;

    for (i = 0; i < nbLines; i++)
    {
        int Output_Start = 0;
        int Output_End = 0;
        pcre_error_code answer = pcre_private((char*)lines[i], (char*)regexpcomments, &Output_Start, &Output_End, NULL, NULL);
        if ( answer == PCRE_FINISHED_OK )
        {
            FREE((char*)lines[i]);
            lines[i] = NULL;
        }
        else
        {
            (*newNbLines)++;
            if (pLinesCleaned == NULL)
            {
                pLinesCleaned = (char **)MALLOC(sizeof(char*) * (*newNbLines));
            }
            else
            {
                pLinesCleaned = (char **)REALLOC(pLinesCleaned, sizeof(char*) * (*newNbLines));
            }

            if (pLinesCleaned == NULL)
            {
                *newNbLines = 0;
                *iErr = 1;
                return NULL;
            }

            pLinesCleaned[(*newNbLines) - 1] = (char*)lines[i];
        }
    }
    return pLinesCleaned;
}
// =============================================================================
