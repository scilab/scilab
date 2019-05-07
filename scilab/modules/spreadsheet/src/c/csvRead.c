/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010-2011 - DIGITEO - Allan CORNET
 * Copyright (C) 2019-2019 - ESI Group - Clement DAVID
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
#include "csvRead.h"
#include "FileExist.h"
#include "Sciwarning.h"
#include "configvariable_interface.h"
#include "csvDefault.h"
#include "expandPathVariable.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "stringToComplex.h"
#include "mclose.h"
#include "mgetl.h"
#include "mopen.h"
#include "os_string.h"
#include "pcre_private.h"
#include "sci_malloc.h"
#include "sciprint.h"
#include "splitLine.h"
#include "strsubst.h"
#include <stdio.h>
#include <string.h>
// =============================================================================
#if _MSC_VER
#define READ_ONLY_TEXT_MODE "rt"
#else
#define READ_ONLY_TEXT_MODE "r"
#endif
// =============================================================================
wchar_t* EMPTY_STR = L"";
// =============================================================================
static char* getNumbersOfColumnsInLines(wchar_t** lines, int sizelines,
                                        const wchar_t* separator, int* cols);
static int getNumbersOfColumnsInLine(wchar_t* line, const wchar_t* separator);
static void removeEmptyLinesAtEnd(wchar_t** lines, int* nonEmptyLines);
static void moveEmptyLinesToEnd(wchar_t** lines, int* nonEmptyLines);
static void replaceDoubleQuotes(wchar_t** lines, int* nbLines);
static int hasOnlyBlankCharacters(wchar_t** line);
static void replaceStrings(wchar_t** lines, int* nbLines, wchar_t** toreplace, int sizetoreplace);
static wchar_t** extractComments(wchar_t** lines, int* nbLines, const wchar_t* regexpcomments, int* nbcomments, int* iErr);
// =============================================================================
csvResult* csvRead(const wchar_t* filename, const wchar_t* separator, const wchar_t* decimal, wchar_t** toreplace, int sizetoreplace, const wchar_t* regexpcomments, int header)
{
    wchar_t* expandedFilename = NULL;
    csvResult* result = NULL;
    int fd = 0;
    int f_swap = 0;
    double res = 0.0;
    int errMOPEN = MOPEN_INVALID_STATUS;
    wchar_t** pwstLines = NULL;
    int nbLines = 0;
    wchar_t** replacedInLines = NULL;
    wchar_t** pComments = NULL;
    int nbComments = 0;
    wchar_t** pHeader = NULL;
    int nbHeader = 0;

    if ((filename == NULL) || (separator == NULL) || (decimal == NULL))
    {
        return NULL;
    }

    expandedFilename = expandPathVariableW(filename);
    if (!FileExistW(expandedFilename))
    {
        result = (csvResult*)(CALLOC(1, sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_FILE_NOT_EXIST;
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
        result = (csvResult*)(CALLOC(1, sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_MOPEN_ERROR;
        }
        return result;
    }

    if (header != 0)
    {
        nbHeader = mgetl(fd, header, &pHeader);
    }

    nbLines = mgetl(fd, -1, &pwstLines);
    mclose(fd);

    if (nbLines < 0)
    {
        result = (csvResult*)(CALLOC(1, sizeof(csvResult)));
        if (result)
        {
            result->err = CSV_READ_READLINES_ERROR;
        }
        freeArrayOfWideString(pHeader, nbHeader);
        return result;
    }

    if (regexpcomments)
    {
        int iErr = 0;

        pComments = extractComments(pwstLines, &nbLines, regexpcomments, &nbComments, &iErr);

        if ((iErr == CAN_NOT_COMPILE_PATTERN) || (iErr == DELIMITER_NOT_ALPHANUMERIC))
        {
            result = (csvResult*)(CALLOC(1, sizeof(csvResult)));
            if (result)
            {
                if ((iErr == CAN_NOT_COMPILE_PATTERN) || (iErr == DELIMITER_NOT_ALPHANUMERIC))
                {
                    iErr = CSV_READ_REGEXP_ERROR;
                }
                result->err = (csvReadError)iErr;
            }
            freeArrayOfWideString(pHeader, nbHeader);
            freeArrayOfWideString(pwstLines, nbLines);
            return result;
        }
    }

    if (toreplace && (sizetoreplace > 0))
    {
        replaceStrings(pwstLines, &nbLines, toreplace, sizetoreplace);
    }

    result = (csvResult*)(CALLOC(1, sizeof(csvResult)));
    if (result)
    {
        result->err = CSV_READ_NO_ERROR;
        result->nbLines = nbLines;
        result->pwstrValues = pwstLines;
        result->nbComments = nbComments;
        result->pwstrComments = pComments;
        result->nbHeader = nbHeader;
        result->pwstrHeader = pHeader;
    }
    else
    {
        freeArrayOfWideString(pComments, nbComments);
        freeArrayOfWideString(pHeader, nbHeader);
        freeArrayOfWideString(pwstLines, nbLines);
    }
    return result;
}
// =============================================================================
/*
 * Decode each cell into the previously allocated memory, this code should not allocate per-cell
 * but rather manipulate the input lines to speed the decoding.
 *
 * Note: some global allocation are also performed depending in some parsing condition. For example,
 * the image buffer of complex numbers is allocated only when a complex number is detected.
 */

int csvTextScanInPlace(wchar_t** text, int nbLines, const wchar_t* separator,
                       const wchar_t* decimal, int haveRange, const int* iRange, int m1, int n1,
                       wchar_t** pstrValues, double* pDblRealValues, double** pDblImgValues)
{
    stringToComplexError ierr = STRINGTOCOMPLEX_ERROR;


    // per line
    for (int i = 0; i < nbLines; i++)
    {
        wchar_t* start = NULL;
        wchar_t* end = NULL;
        wchar_t* previousEnd = NULL;
        wchar_t* iter = text[i];

        // skip some values to be within the range
        if (haveRange)
        {
            if (i < iRange[0] - 1)
            {
                continue;
            }
            if (i >= iRange[2])
            {
                break;
            }
        }

        // per cell (column)
        for (int j = 0; iter != NULL; j++)
        {
            int zeroIndex = i + nbLines * j;

            iter = splitLineCSVNext(iter, separator, &start, &end);

            // skip some values to be within the range
            if (haveRange)
            {
                if (j < iRange[1] - 1)
                {
                    continue;
                }
                if (j >= iRange[3])
                {
                    break;
                }
                // adapt zeroIndex to be within the range
                zeroIndex = i - iRange[0] + 1
                            + (iRange[2] - iRange[0] + 1) * (j - iRange[1] + 1);
            }

            // decode in double or string
            if (pDblRealValues != NULL)
            {
                doublecomplex v = stringToComplexWInPlace(start, end, decimal,
                                  TRUE, &ierr);
                if (ierr == STRINGTOCOMPLEX_NO_ERROR)
                {
                    // the imag part of a complex number is allocated on demand
                    if (v.i != 0. && *pDblImgValues == NULL)
                    {
                        size_t n;
                        if (haveRange)
                        {
                            n = (iRange[2] - iRange[0] + 1)
                                * (iRange[3] - iRange[1] + 1)
                                * sizeof(double);
                        }
                        else
                        {
                            n = m1 * n1 * sizeof(double);
                        }
                        *pDblImgValues = CALLOC(1, n);
                        if (*pDblImgValues == NULL)
                        {
                            return -1;
                        }
                    }
                    if (v.i != 0.)
                    {
                        (*pDblImgValues)[zeroIndex] = v.i;
                    }
                    pDblRealValues[zeroIndex] = v.r;
                }
            }
            else
            {
                // finish a previously stored "end" by a trailing '\0', needed for further iteration *BUT* easy strdup() at the end
                if (previousEnd)
                {
                    *previousEnd = '\0';
                }
                previousEnd = end;

                // convert the passed decimal to a dot
                convertDecimalToDotInPlace(start, decimal);

                // escape double quotes
                escapeDoubleQuotesInPlace(start, end);

                pstrValues[zeroIndex] = start;
            }
        }
    }
    return 0;
}
// =============================================================================
static void replaceDoubleQuotes(wchar_t** lines, int* nbLines)
{
    int dest = 0;
    wchar_t* it = NULL;
    for (int next = dest; next < *nbLines; dest++, next++)
    {
        int quoteCount = 0;
        lines[dest] = lines[next];

        it = lines[dest];
        for (; *it != L'\0'; it++)
        {
            if (*it == L'"')
            {
                quoteCount++;
            }
        }

        // if at the end of this line, we still have an open double quote.
        // merge this line and the followings.
        for (; quoteCount % 2 == 1 && next + 1 < *nbLines; next++)
        {
            wchar_t* currentLine = lines[dest];
            wchar_t* nextLine = lines[next + 1];

            size_t currentLen = it - lines[dest];
            size_t nextLen = wcslen(nextLine);
            size_t newLen = currentLen + nextLen + 2; // includes 'LF' and '\0'

            lines[dest] = MALLOC(newLen * sizeof(wchar_t));
            memcpy(lines[dest], currentLine, currentLen * sizeof(wchar_t));
            *(lines[dest] + currentLen) = 0x0A; // LF
            memcpy(lines[dest] + currentLen + 1, nextLine, nextLen * sizeof(wchar_t));
            *(lines[dest] + currentLen + 1 + nextLen) = L'\0';

            FREE(currentLine);
            FREE(nextLine);

            it = lines[dest] + currentLen;
            for (; *it != L'\0'; it++)
            {
                if (*it == L'"')
                {
                    quoteCount++;
                }
            }
        }
    }

    *nbLines = dest;
}
// =============================================================================
char* csvTextScanSize(wchar_t** lines, int* nbLines, const wchar_t* separator, int* rows, int* cols, int haveRange, int* iRange)
{
    char* errorMsg = NULL;

    // ticket 472
    {
        const char* blankMode = getCsvDefaultCsvIgnoreBlankLine();
        if (strcmp(blankMode, "on") == 0)
        {
            moveEmptyLinesToEnd(lines, nbLines);
        }
        else
        {
            /* remove last lines empty (bug 7003 in scilab)*/
            removeEmptyLinesAtEnd(lines, nbLines);
        }
    }

    // escape line breaks (CRLF) inside double quotes
    // Note: this reallocate and change the number of lines to restore valid
    //       lines and number of lines.
    replaceDoubleQuotes(lines, nbLines);

    errorMsg = getNumbersOfColumnsInLines(lines, *nbLines, separator, cols);
    *rows = *nbLines;
    if (errorMsg != NULL)
    {
        return errorMsg;
    }

    // update the expected range if needed
    if (haveRange)
    {
        // error
        if (iRange[0] > *rows)
        {
            return gettext("%s: Range row or/and column left indice(s) out of bounds.\n");
        }
        if (iRange[1] > *cols)
        {
            return gettext("%s: Range row or/and column left indice(s) out of bounds.\n");
        }

        // truncate
        if (iRange[2] > *rows)
        {
            iRange[2] = *rows;
        }
        if (iRange[3] > *cols)
        {
            iRange[3] = *cols;
        }
    }

    return NULL;
}
// =============================================================================
void freeCsvResult(csvResult* result)
{
    if (result)
    {
        if (result->pwstrValues)
        {
            freeArrayOfWideString(result->pwstrValues, result->nbLines);
            result->pwstrValues = NULL;
        }
        result->nbLines = 0;

        if (result->pwstrComments)
        {
            freeArrayOfWideString(result->pwstrComments, result->nbComments);
            result->pwstrComments = NULL;
        }
        if (result->pwstrHeader)
        {
            freeArrayOfWideString(result->pwstrHeader, result->nbHeader);
            result->pwstrHeader = NULL;
        }
        result->err = CSV_READ_ERROR;
        FREE(result);
        result = NULL;
    }
}
// =============================================================================
static char* getNumbersOfColumnsInLines(wchar_t** lines, int sizelines,
                                        const wchar_t* separator, int* cols)
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
                    return gettext("%s: can not read file, error in the column structure\n");
                }
            }
        }
    }
    *cols = NbColumns;
    return NULL;
}
// =============================================================================
static int getNumbersOfColumnsInLine(wchar_t* line, const wchar_t* separator)
{
    int nbTokens = 0;

    if (line && separator)
    {
        wchar_t* start = NULL;
        wchar_t* end = NULL;

        wchar_t* iter = line;
        while (iter != NULL)
        {
            nbTokens++;

            iter = splitLineCSVNext(iter, separator, &start, &end);
        }
    }
    return nbTokens;
}
// =============================================================================
static void removeEmptyLinesAtEnd(wchar_t** lines, int* nonEmptyLines)
{
    const int len = *nonEmptyLines;

    if (lines)
    {
        int i = len - 1;
        for (; i >= 0; i--)
        {
            if (!hasOnlyBlankCharacters(&lines[i]))
            {
                break;
            }
        }

        *nonEmptyLines = i + 1;
    }
}
// =============================================================================
static void moveEmptyLinesToEnd(wchar_t** lines, int* nonEmptyLines)
{
    const int len = *nonEmptyLines;
    int nbLinesToRemove = 0;

    if (lines)
    {
        for (int i = 0; i < len; i++)
        {
            if (hasOnlyBlankCharacters(&lines[i]))
            {
                nbLinesToRemove++;

                // move the lines at the end
                for (int j = i + 1; j < len; i++, j++)
                {
                    // hopefully, empty lines have been flagged by hasOnlyBlankCharacters
                    if (*(lines[j]) != L'\0')
                    {
                        // swap
                        wchar_t* str = lines[i];
                        lines[i] = lines[j];
                        lines[j] = str;
                    }
                }

            }
        }
    }

    *nonEmptyLines = len - nbLinesToRemove;
}
// =============================================================================
static int hasOnlyBlankCharacters(wchar_t** line)
{
    const wchar_t* iter = *line;
    while (*iter == L'\t' || *iter == L'\r' || *iter == L'\n' || *iter == L' ')
    {
        iter++;
    }

    if (*iter == L'\0')
    {
        // flag the empty line as really empty, further comparisons will be cheap
        FREE(*line);
        *line = EMPTY_STR;
        return 1;
    }
    else
    {
        return 0;
    }
}
// =============================================================================
static void replaceStrings(wchar_t** lines, int* nbLines, wchar_t** toreplace, int sizetoreplace)
{
    // number of string to compare and replace
    int nr = sizetoreplace / 2;

    if (nbLines == NULL || *nbLines == 0 || sizetoreplace == 0)
    {
        return;
    }
    if (lines == NULL || toreplace == NULL)
    {
        return;
    }

    // for each line
    for (int i = 0; i < *nbLines; i++)
    {
        // for each string to compare with
        for (int j = 0; j < nr; j++)
        {
            wchar_t* found = wcsstr(lines[i], toreplace[j]);
            if (found)
            {
                // we found a matching string, either reallocate and copy OR
                // copy in place if there is enough room for the replacement
                size_t lineLen = wcslen(lines[i]);
                size_t foundLen = wcslen(found);
                size_t matchLen = wcslen(toreplace[j]);
                size_t replaceLen = wcslen(toreplace[j + nr]);
                if (replaceLen > matchLen)
                {
                    wchar_t* previousLine = lines[i];
                    size_t allocatedLen = lineLen - matchLen + replaceLen + 1;
                    size_t foundIndex = found - previousLine;

                    lines[i] = MALLOC(allocatedLen * sizeof(wchar_t));
                    // copy up to the found string
                    memcpy(lines[i], previousLine, foundIndex * sizeof(wchar_t));
                    // copy the replacement string
                    memcpy(&lines[i][foundIndex], toreplace[j + nr], replaceLen * sizeof(wchar_t));
                    // copy from the end of the match up to the end of line
                    memcpy(&lines[i][foundIndex + replaceLen], &previousLine[foundIndex + matchLen], (allocatedLen - foundIndex - replaceLen) * sizeof(wchar_t));

                    FREE(previousLine);
                }
                else
                {
                    // copy the replacement string
                    memcpy(found, toreplace[j + nr], replaceLen * sizeof(wchar_t));
                    // move the remaining string next to the replaced string (including the trailing 0)
                    memmove(&found[replaceLen], &found[matchLen], (foundLen - matchLen + 1) * sizeof(wchar_t));
                }

                // continue to replace
                j--;
            }
        }
    }
}
// =============================================================================
static wchar_t** extractComments(wchar_t** lines, int* nbLines,
                                 const wchar_t* regexpcomments, int* nbcomments, int* iErr)
{
    wchar_t** pComments = NULL;
    int i = 0;

    for (i = 0; i < *nbLines; i++)
    {
        int Output_Start = 0;
        int Output_End = 0;
        pcre_error_code answer = wide_pcre_private(lines[i], regexpcomments, &Output_Start, &Output_End, NULL, NULL);

        if ((answer == CAN_NOT_COMPILE_PATTERN) || (answer == DELIMITER_NOT_ALPHANUMERIC))
        {
            if (pComments)
            {
                freeArrayOfWideString(pComments, *nbcomments);
            }

            *nbcomments = 0;

            *iErr = answer;
            return NULL;
        }

        if (answer == PCRE_FINISHED_OK)
        {
            (*nbcomments)++;
            if (pComments == NULL)
            {
                pComments = (wchar_t**)MALLOC(sizeof(wchar_t*) * (*nbcomments));
            }
            else
            {
                pComments = (wchar_t**)REALLOC(pComments, sizeof(wchar_t*) * (*nbcomments));
            }

            if (pComments == NULL)
            {
                *nbcomments = 0;
                *iErr = 1;
                return NULL;
            }

            // move lines[i] to comments and continue
            pComments[(*nbcomments) - 1] = lines[i];
            memmove(lines + i, lines + i + 1, sizeof(wchar_t*) * (*nbLines - i - 1));
            (*nbLines)--;
            i--;
        }
    }

    return pComments;
}
// =============================================================================
