/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Karim Mamode
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include <string.h>
#include <wchar.h>
#include <wctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include "charEncoding.h"
#include "completion.h"
#include "autoCompletionCli.h"
#include "completeLine.h"
#include "cliDisplayManagement.h"
#include "freeArrayOfString.h"
#include "cliPrompt.h"
#include "sci_malloc.h"
#include "completion.h"
#include "getPartLine.h"
#include "getCommonPart.h"
#include "localization.h"
#include "configvariable_interface.h"
#include "sciprint.h"
#include "HistoryManager.h"
#include "ConsoleRead.h"
#include "SetConsolePrompt.h"
#include "TermReadAndProcess.h"
#include "getCommonPart.h"

static void doCompletion(char **wk_buf, unsigned int *cursor, unsigned int *cursor_max);

static char *getLineBeforeCaret(char *wk_buf, unsigned int *cursor);

static char *getLineAfterCaret(char *wk_buf, unsigned int *cursor, unsigned int *cursor_max);

static void backspace(int n);

static void erase_nchar(int n);

static void TermCompletionOnFiles(char **dictionaryFiles, int sizedictionaryFiles,
                                  char *lineBeforeCaret, char *lineAfterCaret, char *filePattern, char *defaultPattern,
                                  char **wk_buf, unsigned int *cursor, unsigned int *cursor_max);
static int CopyLineAtPrompt(char **wk_buf, char *line, unsigned int *cursor, unsigned int *cursor_max);
static void separateFilesDirectories(char** dictionary, int size, char*** files, int* sizeFiles, char*** directories, int* sizeDirectories);

static void TermCompletionOnAll(char *lineBeforeCaret, char *lineAfterCaret, char *defaultPattern, char **wk_buf, unsigned int *cursor,
                                unsigned int *cursor_max);

static void displayCompletionDictionary(char **dictionary, int sizedictionary, char *namedictionary);

static char **concatenateStrings(int *sizearrayofstring, char *string1, char *string2, char *string3, char *string4, char *string5);

/* Autocompletion in NW/NWNI */
void autoCompletionInConsoleMode(wchar_t ** commandLine, unsigned int *cursorLocation)
{
    char *multiByteString = NULL;
    wchar_t *wideString = NULL;

    int sizeToAlloc = 0;

    unsigned int nbrCharInString;

    multiByteString = wide_string_to_UTF8(*commandLine);
    nbrCharInString = wcslen(*commandLine);
    doCompletion(&multiByteString, cursorLocation, &nbrCharInString);

    wideString = to_wide_string(multiByteString);
    /* Copy the new string in a buffer wich size is a multiple of 1024 */
    sizeToAlloc = 1024 * (wcslen(wideString) / 1024 + 1);
    FREE(*commandLine);
    *commandLine = MALLOC(sizeof(**commandLine) * sizeToAlloc);
    wcscpy(*commandLine, wideString);
    FREE(wideString);
    FREE(multiByteString);
}

static void doCompletion(char **wk_buf, unsigned int *cursor, unsigned int *cursor_max)
{
    char *LineBeforeCaret = getLineBeforeCaret(*wk_buf, cursor);
    char *LineAfterCaret = getLineAfterCaret(*wk_buf, cursor, cursor_max);
    char *fileSearchedPattern = getFilePartLevel(LineBeforeCaret);
    char *SearchedPattern = getPartLevel(LineBeforeCaret);
    char **completionDictionaryFiles = NULL;
    int sizecompletionDictionaryFiles = 0;

    completionDictionaryFiles = completionOnFiles(fileSearchedPattern, &sizecompletionDictionaryFiles);
    if (completionDictionaryFiles)
    {
        TermCompletionOnFiles(completionDictionaryFiles, sizecompletionDictionaryFiles,
                              LineBeforeCaret, LineAfterCaret, fileSearchedPattern, SearchedPattern, wk_buf, cursor, cursor_max);

        freeArrayOfString(completionDictionaryFiles, sizecompletionDictionaryFiles);
    }
    else
    {
        TermCompletionOnAll(LineBeforeCaret, LineAfterCaret, SearchedPattern, wk_buf, cursor, cursor_max);
    }

    FREE(LineBeforeCaret);
    LineBeforeCaret = NULL;
    if (LineAfterCaret)
    {
        FREE(LineAfterCaret);
        LineAfterCaret = NULL;
    }
    if (fileSearchedPattern)
    {
        FREE(fileSearchedPattern);
        fileSearchedPattern = NULL;
    }
    if (SearchedPattern)
    {
        FREE(SearchedPattern);
        SearchedPattern = NULL;
    }
}

static char *getLineBeforeCaret(char *wk_buf, unsigned int *cursor)
{
    char *line = NULL;
    wchar_t * wtmp = to_wide_string(wk_buf);

    wtmp[*cursor] = 0;
    line = wide_string_to_UTF8(wtmp);
    FREE(wtmp);

    return line;
}

static char *getLineAfterCaret(char *wk_buf, unsigned int *cursor, unsigned int *cursor_max)
{
    if (wk_buf)
    {
        if (*cursor != *cursor_max)
        {
            int len = *cursor_max - *cursor;
            wchar_t * wtmp = to_wide_string(wk_buf);
            wchar_t aftercaret[WK_BUF_SIZE];

            wcscpy(aftercaret, &wtmp[*cursor]);
            aftercaret[len + 1] = '\0';
            FREE(wtmp);

            return wide_string_to_UTF8(aftercaret);
        }
    }
    return strdup("");
}

static void backspace(int n)
{
    if (n < 1)
    {
        return;
    }
    while (n--)
#ifdef TERMCAP
        if (BC)
        {
            /* if control-H won-t work */
            fputs(BC, stdout);
        }
        else
        {
            /* otherwise just use a normal control-H */
            putchar('\010');
        }
#else
        putchar('\010');
#endif

}

static void erase_nchar(int n)
{
    int i;                      /* fill field with blanks */

    for (i = 0; i < n; i++)
    {
        putchar(' ');
    }
    backspace(n);               /* and back up over blanks just written */
}

static void TermCompletionOnFiles(char **dictionaryFiles, int sizedictionaryFiles,
                                  char *lineBeforeCaret, char *lineAfterCaret, char *filePattern, char *defaultPattern,
                                  char **wk_buf, unsigned int *cursor, unsigned int *cursor_max)
{
    if (dictionaryFiles)
    {
        if (sizedictionaryFiles == 1)
        {
            char *new_line = completeLine(lineBeforeCaret, dictionaryFiles[0], filePattern, defaultPattern, TRUE, lineAfterCaret);

            if (new_line)
            {
                char buflinetmp[WK_BUF_SIZE + 1];

                strcpy(buflinetmp, new_line);
                FREE(new_line);

                backspace(*cursor);
                erase_nchar(*cursor_max);
                *cursor = *cursor_max = 0;

                CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
                return;
            }
        }
        else
        {
            char *common = getCommonPart(dictionaryFiles, sizedictionaryFiles);

            char** files;
            int sizeFiles;
            char** directories;
            int sizeDirectories;
            separateFilesDirectories(dictionaryFiles, sizedictionaryFiles, &files, &sizeFiles, &directories, &sizeDirectories);

            //displayCompletionDictionary(dictionaryFiles, sizedictionaryFiles, gettext("File or Directory"));
            displayCompletionDictionary(files, sizeFiles, gettext("File"));
            displayCompletionDictionary(directories, sizeDirectories, gettext("Directory"));
            freeArrayOfString(files, sizeFiles);
            freeArrayOfString(directories, sizeDirectories);

            printf("\n");

            backspace(*cursor);
            erase_nchar(*cursor_max);
            *cursor = *cursor_max = 0;

            printPrompt(WRITE_PROMPT);

            if (defaultPattern[0] == 0)
            {
                int lennewline = (int)strlen(lineBeforeCaret) + (int)strlen(lineAfterCaret);

                char *new_line = (char *)MALLOC(sizeof(char) * (lennewline + 1));

                if (new_line)
                {
                    strcpy(new_line, lineBeforeCaret);
                    strcat(new_line, lineAfterCaret);

                    CopyLineAtPrompt(wk_buf, new_line, cursor, cursor_max);
                    FREE(new_line);
                    new_line = NULL;
                }
            }
            else if (common)
            {
                char *new_line = completeLine(lineBeforeCaret, common, filePattern, defaultPattern, TRUE, lineAfterCaret);

                if (new_line)
                {
                    char buflinetmp[WK_BUF_SIZE + 1];

                    strcpy(buflinetmp, new_line);
                    FREE(new_line);

                    CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
                }
                else
                {
                    int lennewline = (int)strlen(lineBeforeCaret) + (int)strlen(lineAfterCaret);

                    new_line = (char *)MALLOC(sizeof(char) * (lennewline + 1));

                    if (new_line)
                    {
                        strcpy(new_line, lineBeforeCaret);
                        strcat(new_line, lineAfterCaret);

                        CopyLineAtPrompt(wk_buf, new_line, cursor, cursor_max);
                        FREE(new_line);
                        new_line = NULL;
                    }
                }
            }
            if (common)
            {
                FREE(common);
                common = NULL;
            }
        }
    }
}

static int CopyLineAtPrompt(char **wk_buf, char *line, unsigned int *cursor, unsigned int *cursor_max)
{
    FREE(*wk_buf);
    if (line)
    {
        wchar_t *wtmp = NULL;
        //** Copy line to current command buffer, usefull in completion case.
        *wk_buf = MALLOC(sizeof(char) * strlen(line) + 1);
        strcpy(*wk_buf, line);
        backspace(*cursor);     /* backspace to beginning of line */
        printf("%s", *wk_buf);   /* copy to screen */

        wtmp = to_wide_string(*wk_buf);
        *cursor = wcslen(wtmp); /* cursor set at end of line */
        FREE(wtmp);

        /* erase extra characters left over if any */
        erase_nchar(GET_MAX(0, (int)(*cursor_max - *cursor)));
        *cursor_max = *cursor;
        return 1;
    }
    else
    {
        *wk_buf = (char*) MALLOC(sizeof(char));
        (*wk_buf)[0] = '\0';
    }
    return 0;
}

static void separateFilesDirectories(char** dictionary, int size, char*** files, int* sizeFiles, char*** directories, int* sizeDirectories)
{
    int i;
    *files = NULL;
    *sizeFiles = 0;
    *directories = NULL;
    *sizeDirectories = 0;
    for (i = 0; i < size; ++i)
    {
        // Check that the item is a file or a directory
        char* word = dictionary[i];
        int len = (int) strlen(word);
        if (len && word[len - 1] == '/')
        {
            (*sizeDirectories)++;
            *directories = (char **) REALLOC(*directories, sizeof(char *) * (*sizeDirectories));
            (*directories)[*sizeDirectories - 1] = strdup(word);
        }
        else
        {
            (*sizeFiles)++;
            *files = (char **) REALLOC(*files, sizeof(char *) * (*sizeFiles));
            (*files)[*sizeFiles - 1] = strdup(word);
        }
    }
}

static char **concatenateStrings(int *sizearrayofstring, char *string1, char *string2, char *string3, char *string4, char *string5)
{
    int newsize = 0;

    char **arrayOfString = NULL;

    *sizearrayofstring = 0;

    if (string1)
    {
        newsize++;
    }
    if (string2)
    {
        newsize++;
    }
    if (string3)
    {
        newsize++;
    }
    if (string4)
    {
        newsize++;
    }
    if (string5)
    {
        newsize++;
    }

    if (newsize > 0)
    {
        arrayOfString = (char **)MALLOC(sizeof(char *) * (newsize));
        if (arrayOfString)
        {
            int i = 0;

            if (string1)
            {
                arrayOfString[i] = string1;
                i++;
            }
            if (string2)
            {
                arrayOfString[i] = string2;
                i++;
            }
            if (string3)
            {
                arrayOfString[i] = string3;
                i++;
            }
            if (string4)
            {
                arrayOfString[i] = string4;
                i++;
            }
            if (string5)
            {
                arrayOfString[i] = string5;
                i++;
            }
            *sizearrayofstring = i;
        }
        else
        {
            *sizearrayofstring = 0;
        }
    }
    return arrayOfString;
}

static void TermCompletionOnAll(char *lineBeforeCaret, char *lineAfterCaret, char *defaultPattern, char **wk_buf, unsigned int *cursor,
                                unsigned int *cursor_max)
{
    if (defaultPattern)
    {
        int numberWordFound = 0;

        char **completionDictionaryFunctions = NULL;

        int sizecompletionDictionaryFunctions = 0;

        char **completionDictionaryCommandWords = NULL;

        int sizecompletionDictionaryCommandWords = 0;

        char **completionDictionaryMacros = NULL;

        int sizecompletionDictionaryMacros = 0;

        char **completionDictionaryVariables = NULL;

        int sizecompletionDictionaryVariables = 0;

        char **completionDictionaryHandleGraphicsProperties = NULL;

        int sizecompletionDictionaryHandleGraphicsProperties = 0;

        char **completionDictionaryFields = NULL;

        int sizecompletionDictionaryFields = 0;

        completionDictionaryFields = completionOnFields(lineBeforeCaret, defaultPattern, &sizecompletionDictionaryFields);

        if (!completionDictionaryFields && strcmp(defaultPattern, ""))
        {
            completionDictionaryFunctions = completionOnFunctions(defaultPattern, &sizecompletionDictionaryFunctions);
            completionDictionaryCommandWords = completionOnCommandWords(defaultPattern, &sizecompletionDictionaryCommandWords);
            completionDictionaryMacros = completionOnMacros(defaultPattern, &sizecompletionDictionaryMacros);
            completionDictionaryVariables = completionOnVariablesWithoutMacros(defaultPattern, &sizecompletionDictionaryVariables);
            completionDictionaryHandleGraphicsProperties =
                completionOnHandleGraphicsProperties(defaultPattern, &sizecompletionDictionaryHandleGraphicsProperties);
        }

        numberWordFound = sizecompletionDictionaryFunctions + sizecompletionDictionaryCommandWords +
                          sizecompletionDictionaryMacros + sizecompletionDictionaryVariables +
                          sizecompletionDictionaryHandleGraphicsProperties + sizecompletionDictionaryFields;

        if (numberWordFound > 0)
        {
            if (numberWordFound == 1)
            {
                char **completionDictionary = NULL;

                char *new_line = NULL;

                if (completionDictionaryFields)
                {
                    completionDictionary = completionDictionaryFields;
                }
                if (completionDictionaryFunctions)
                {
                    completionDictionary = completionDictionaryFunctions;
                }
                if (completionDictionaryCommandWords)
                {
                    completionDictionary = completionDictionaryCommandWords;
                }
                if (completionDictionaryMacros)
                {
                    completionDictionary = completionDictionaryMacros;
                }
                if (completionDictionaryVariables)
                {
                    completionDictionary = completionDictionaryVariables;
                }
                if (completionDictionaryHandleGraphicsProperties)
                {
                    completionDictionary = completionDictionaryHandleGraphicsProperties;
                }

                new_line = completeLine(lineBeforeCaret, completionDictionary[0], NULL, defaultPattern, FALSE, lineAfterCaret);
                if (new_line)
                {
                    char buflinetmp[WK_BUF_SIZE + 1];

                    strcpy(buflinetmp, new_line);
                    FREE(new_line);

                    backspace(*cursor);
                    erase_nchar(*cursor_max);
                    *cursor = *cursor_max = 0;

                    CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
                }
            }
            else
            {
                char *commonAll = NULL;

                if (completionDictionaryFields)
                {
                    commonAll = getCommonPart(completionDictionaryFields, sizecompletionDictionaryFields);
                    displayCompletionDictionary(completionDictionaryFields, sizecompletionDictionaryFields, (char *)_("Scilab Fields"));
                }
                else
                {
                    char *commonFunctions = getCommonPart(completionDictionaryFunctions, sizecompletionDictionaryFunctions);

                    char *commonCommandWords = getCommonPart(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords);

                    char *commonMacros = getCommonPart(completionDictionaryMacros, sizecompletionDictionaryMacros);

                    char *commonVariables = getCommonPart(completionDictionaryVariables, sizecompletionDictionaryVariables);

                    char *commonHandleGraphicsProperties =
                        getCommonPart(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties);

                    int sizecommonsDictionary = 0;

                    char **commonsDictionary = concatenateStrings(&sizecommonsDictionary, commonFunctions,
                                               commonMacros, commonCommandWords, commonVariables, commonHandleGraphicsProperties);

                    if (sizecommonsDictionary > 0)
                    {
                        if (sizecommonsDictionary == 1)
                        {
                            commonAll = strdup(commonsDictionary[0]);
                        }
                        else
                        {
                            commonAll = getCommonPart(commonsDictionary, sizecommonsDictionary);
                        }
                    }
                    freeArrayOfString(commonsDictionary, sizecommonsDictionary);

                    displayCompletionDictionary(completionDictionaryFunctions, sizecompletionDictionaryFunctions, (char *)_("Scilab Function"));
                    displayCompletionDictionary(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords, (char *)_("Scilab Command"));
                    displayCompletionDictionary(completionDictionaryMacros, sizecompletionDictionaryMacros, (char *)_("Scilab Macro"));
                    displayCompletionDictionary(completionDictionaryVariables, sizecompletionDictionaryVariables, (char *)_("Scilab Variable"));
                    displayCompletionDictionary(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties,
                                                (char *)_("Graphics handle field"));
                }

                printf("\n");

                backspace(*cursor);
                erase_nchar(*cursor_max);
                *cursor = *cursor_max = 0;

                printPrompt(WRITE_PROMPT);

                if (commonAll)
                {
                    char *new_line = NULL;

                    new_line = completeLine(lineBeforeCaret, commonAll, NULL, defaultPattern, FALSE, lineAfterCaret);
                    if (new_line)
                    {
                        char buflinetmp[WK_BUF_SIZE + 1];

                        strcpy(buflinetmp, new_line);
                        FREE(new_line);

                        CopyLineAtPrompt(wk_buf, buflinetmp, cursor, cursor_max);
                    }

                    FREE(commonAll);
                    commonAll = NULL;
                }
            }
        }
        freeArrayOfString(completionDictionaryFields, sizecompletionDictionaryFields);
        freeArrayOfString(completionDictionaryFunctions, sizecompletionDictionaryFunctions);
        freeArrayOfString(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords);
        freeArrayOfString(completionDictionaryMacros, sizecompletionDictionaryMacros);
        freeArrayOfString(completionDictionaryVariables, sizecompletionDictionaryVariables);
        freeArrayOfString(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties);
    }
}

static void displayCompletionDictionary(char **dictionary, int sizedictionary, char *namedictionary)
{
#define MAX_LINE_SIZE 79        /* 80 - 1 the leading space */
    if (dictionary)
    {
        int i = 0;

        int lenCurrentLine = 0;

        /* Set the category */
        setCharDisplay(DISP_ITALIC);
        printf("\n");
        printf("%s", namedictionary);
        printf("%s", ":");
        printf("\n");
        /* Reset to set back the default display. */
        setCharDisplay(DISP_LAST_SET);

        /* Set the list of completion */
        setCharDisplay(DISP_RESET);
        for (i = 0; i < sizedictionary; i++)
        {
            int newlenLine = lenCurrentLine + (int)strlen(dictionary[i]) + (int)strlen(" ");

            if ((lenCurrentLine + newlenLine) > MAX_LINE_SIZE)
            {
                printf("\n");
                lenCurrentLine = 0;
            }
            else
            {
                lenCurrentLine = newlenLine;
            }
            printf("%s", dictionary[i]);
            printf("%s", " ");
        }
        printf("\n");
        /* Reset to set back the default display. */
        setCharDisplay(DISP_LAST_SET);
    }
}
