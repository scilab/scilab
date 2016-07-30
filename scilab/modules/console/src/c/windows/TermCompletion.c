/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2010 - DIGITEO - Allan CORNET
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
#include <string.h>
#include <stdlib.h>
#include "TermCompletion.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "TermLine.h"
#include "TermConsole.h"
#include "getPartLine.h"
#include "getCommonPart.h"
#include "completion.h"
#include "scilines.h"
#include "os_string.h"
#include "completeLine.h"
/*--------------------------------------------------------------------------*/
static void displayCompletionDictionary(char **dictionary, int sizedictionary, char *namedictionary);
static char **concatenateStrings(int *sizearrayofstring, char *string1,
                                 char *string2, char *string3,
                                 char *string4, char *string5);
static void TermCompletionOnFiles(char **dictionaryFiles, int sizedictionaryFiles,
                                  char *lineBeforeCaret, char *lineAfterCaret, char *filePattern, char *defaultPattern);
static void separateFilesDirectories(char** dictionary, int size, char*** files, int* sizeFiles, char*** directories, int* sizeDirectories);
static void TermCompletionOnAll(char *lineBeforeCaret, char *lineAfterCaret, char *defaultPattern);
/*--------------------------------------------------------------------------*/
static void TermCompletionOnFiles(char **dictionaryFiles, int sizedictionaryFiles,
                                  char *lineBeforeCaret, char *lineAfterCaret, char *filePattern, char *defaultPattern)
{
    if (dictionaryFiles)
    {
        if (sizedictionaryFiles == 1)
        {
            char *newline = completeLine(lineBeforeCaret, dictionaryFiles[0], filePattern, defaultPattern, TRUE, lineAfterCaret);
            if (newline)
            {
                clearCurrentLine();
                copyLine(newline);
                FREE(newline);
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

            displayPrompt();
            newLine();

            if (defaultPattern[0] == 0)
            {
                int lennewline = (int)strlen(lineBeforeCaret) + (int)strlen(lineAfterCaret);
                char *newline = (char*)MALLOC(sizeof(char) * (lennewline + 1));

                clearCurrentLine();
                if (newline)
                {
                    strcpy(newline, lineBeforeCaret);
                    strcat(newline, lineAfterCaret);
                    copyLine(newline);
                    FREE(newline);
                    newline = NULL;
                }
            }
            else if (common)
            {
                char *newline = completeLine(lineBeforeCaret, common, filePattern, defaultPattern, TRUE, lineAfterCaret);
                if (newline)
                {
                    clearCurrentLine();
                    copyLine(newline);
                    FREE(common);
                    FREE(newline);
                    return;
                }
                else
                {
                    int lennewline = (int)strlen(lineBeforeCaret) + (int)strlen(lineAfterCaret);
                    newline = (char*)MALLOC(sizeof(char) * (lennewline + 1));

                    clearCurrentLine();
                    if (newline)
                    {
                        strcpy(newline, lineBeforeCaret);
                        strcat(newline, lineAfterCaret);
                        copyLine(newline);
                        FREE(newline);
                        newline = NULL;
                    }
                }
                FREE(common);
                common = NULL;
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
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
        if (len && word[len - 1] == '\\')
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
/*--------------------------------------------------------------------------*/
static void TermCompletionOnAll(char *lineBeforeCaret, char *lineAfterCaret, char *defaultPattern)
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

        if ((completionDictionaryFields == NULL) && strcmp(defaultPattern, ""))
        {
            completionDictionaryFunctions = completionOnFunctions(defaultPattern, &sizecompletionDictionaryFunctions);
            completionDictionaryCommandWords = completionOnCommandWords(defaultPattern, &sizecompletionDictionaryCommandWords);
            completionDictionaryMacros = completionOnMacros(defaultPattern, &sizecompletionDictionaryMacros);
            completionDictionaryVariables = completionOnVariablesWithoutMacros(defaultPattern, &sizecompletionDictionaryVariables);
            completionDictionaryHandleGraphicsProperties = completionOnHandleGraphicsProperties(defaultPattern, &sizecompletionDictionaryHandleGraphicsProperties);
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
                    clearCurrentLine();
                    copyLine(new_line);
                    FREE(new_line);
                }
            }
            else
            {
                char *commonAll = NULL;
                if (completionDictionaryFields)
                {
                    commonAll = getCommonPart(completionDictionaryFields, sizecompletionDictionaryFields);
                    displayCompletionDictionary(completionDictionaryFields, sizecompletionDictionaryFields, (char *)_("Scilab Fields"));
                    freeArrayOfString(completionDictionaryFields, sizecompletionDictionaryFields);
                }
                else
                {
                    char *commonFunctions = getCommonPart(completionDictionaryFunctions, sizecompletionDictionaryFunctions);
                    char *commonCommandWords = getCommonPart(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords);
                    char *commonMacros = getCommonPart(completionDictionaryMacros, sizecompletionDictionaryMacros);
                    char *commonVariables = getCommonPart(completionDictionaryVariables, sizecompletionDictionaryVariables);
                    char *commonHandleGraphicsProperties = getCommonPart(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties);

                    int sizecommonsDictionary = 0;
                    char **commonsDictionary = concatenateStrings(&sizecommonsDictionary, commonFunctions,
                                               commonMacros, commonCommandWords, commonVariables, commonHandleGraphicsProperties);

                    if (sizecommonsDictionary > 0)
                    {
                        if (sizecommonsDictionary == 1)
                        {
                            commonAll = os_strdup(commonsDictionary[0]);
                        }
                        else
                        {
                            commonAll = getCommonPart(commonsDictionary, sizecommonsDictionary);
                        }
                        freeArrayOfString(commonsDictionary, sizecommonsDictionary);
                    }

                    displayCompletionDictionary(completionDictionaryFunctions, sizecompletionDictionaryFunctions, (char *)_("Scilab Function"));
                    displayCompletionDictionary(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords, (char *)_("Scilab Command"));
                    displayCompletionDictionary(completionDictionaryMacros, sizecompletionDictionaryMacros, (char *)_("Scilab Macro"));
                    displayCompletionDictionary(completionDictionaryVariables, sizecompletionDictionaryVariables, (char *)_("Scilab Variable"));
                    displayCompletionDictionary(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties, (char *)_("Graphics handle field"));
                    freeArrayOfString(completionDictionaryFunctions, sizecompletionDictionaryFunctions);
                    freeArrayOfString(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords);
                    freeArrayOfString(completionDictionaryMacros, sizecompletionDictionaryMacros);
                    freeArrayOfString(completionDictionaryVariables, sizecompletionDictionaryVariables);
                    freeArrayOfString(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties);
                }

                displayPrompt();
                newLine();

                if (commonAll)
                {
                    char *newline = NULL;

                    newline = completeLine(lineBeforeCaret, commonAll, NULL, defaultPattern, FALSE, lineAfterCaret);

                    if (newline)
                    {
                        clearCurrentLine();
                        copyLine(newline);
                        FREE(newline);
                    }
                    FREE(commonAll);
                    commonAll = NULL;
                }
            }
        }
    }
}
/*--------------------------------------------------------------------------*/
void TermCompletion(void)
{
    char *LineBeforeCaret = getLineBeforeCaret();
    char *LineAfterCaret = getLineAfterCaret();
    char *fileSearchedPattern = getFilePartLevel(LineBeforeCaret);
    char *SearchedPattern = getPartLevel(LineBeforeCaret);


    char **completionDictionaryFiles = NULL;
    int sizecompletionDictionaryFiles = 0;

    completionDictionaryFiles = completionOnFiles(fileSearchedPattern, &sizecompletionDictionaryFiles);
    if (completionDictionaryFiles)
    {
        TermCompletionOnFiles(completionDictionaryFiles, sizecompletionDictionaryFiles,
                              LineBeforeCaret, LineAfterCaret, fileSearchedPattern, SearchedPattern);

        freeArrayOfString(completionDictionaryFiles, sizecompletionDictionaryFiles);
    }
    else
    {
        TermCompletionOnAll(LineBeforeCaret, LineAfterCaret, SearchedPattern);
    }

    if (LineBeforeCaret)
    {
        FREE(LineBeforeCaret);
        LineBeforeCaret = NULL;
    }
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
/*--------------------------------------------------------------------------*/
static void displayCompletionDictionary(char **dictionary, int sizedictionary, char *namedictionary)
{
    if (dictionary)
    {
        int i = 0;
        int lenCurrentLine = 0;

        TerminalPrintf("\n");
        TerminalPrintf(namedictionary);
        TerminalPrintf(":");
        TerminalPrintf("\n");

        for (i = 0; i < sizedictionary; i++)
        {
            int newlenLine = lenCurrentLine + (int)strlen(dictionary[i]) + (int)strlen(" ");
            if (newlenLine >= (getConsoleWidth() - 10))
            {
                TerminalPrintf("\n");
                lenCurrentLine = 0;
            }
            else
            {
                lenCurrentLine = newlenLine;
            }

            TerminalPrintf(dictionary[i]);
            TerminalPrintf(" ");
        }
        TerminalPrintf("\n");
    }
}
/*--------------------------------------------------------------------------*/
static char **concatenateStrings(int *sizearrayofstring, char *string1,
                                 char *string2, char *string3,
                                 char *string4, char *string5)
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
        arrayOfString = (char**)MALLOC(sizeof(char*) * (newsize));
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
/*--------------------------------------------------------------------------*/
