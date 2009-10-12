/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2008 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdlib.h>
#include "TermCompletion.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "TermLine.h"
#include "TermConsole.h"
#include "getPartLine.h"
#include "getCommonPart.h"
#include "completion.h"
#include "scilines.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "completeLine.h"
/*--------------------------------------------------------------------------*/
static void displayCompletionDictionary(char **dictionary,int sizedictionary, char *namedictionary);
static char **concatenateStrings(int *sizearrayofstring, char *string1,
								 char *string2, char *string3,
								 char *string4, char *string5);
static void TermCompletionOnFiles(char **dictionaryFiles, int sizedictionaryFiles,
								  char *lineBeforeCaret, char *lineAfterCaret, char *filePattern, char *defaultPattern);
static void TermCompletionOnAll(char *lineBeforeCaret, char *lineAfterCaret, char *defaultPattern);
/*--------------------------------------------------------------------------*/
static void TermCompletionOnFiles(char **dictionaryFiles, int sizedictionaryFiles,
								  char *lineBeforeCaret, char *lineAfterCaret, char *filePattern, char *defaultPattern)
{
	if (dictionaryFiles)
	{
		if (sizedictionaryFiles == 1)
		{
			char *newline = completeLine(lineBeforeCaret,dictionaryFiles[0],filePattern,defaultPattern,TRUE,lineAfterCaret);
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

			displayCompletionDictionary(dictionaryFiles, 
				sizedictionaryFiles, gettext("File or Directory"));

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
					FREE(newline); newline = NULL;
				}
			}
			else if (common)
			{
				char *newline = completeLine(lineBeforeCaret,common,filePattern,defaultPattern,TRUE,lineAfterCaret);
				if (newline)
				{
					clearCurrentLine();
					copyLine(newline);
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
						FREE(newline); newline = NULL;
					}
				}
				FREE(common);
				common = NULL;
			}
		}
	}
}
/*--------------------------------------------------------------------------*/
static void TermCompletionOnAll(char *lineBeforeCaret, char *lineAfterCaret, char *defaultPattern)
{
	if ( defaultPattern && strcmp(defaultPattern, "") )
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

		completionDictionaryFunctions = completionOnFunctions(defaultPattern, 
			&sizecompletionDictionaryFunctions);

		completionDictionaryCommandWords = completionOnCommandWords(defaultPattern, 
			&sizecompletionDictionaryCommandWords);

		completionDictionaryMacros = completionOnMacros(defaultPattern, 
			&sizecompletionDictionaryMacros);

		completionDictionaryVariables = completionOnVariablesWithoutMacros(defaultPattern, 
			&sizecompletionDictionaryVariables);

		completionDictionaryHandleGraphicsProperties = completionOnHandleGraphicsProperties(defaultPattern, 
			&sizecompletionDictionaryHandleGraphicsProperties);

		numberWordFound = sizecompletionDictionaryFunctions + sizecompletionDictionaryCommandWords +
			sizecompletionDictionaryMacros + sizecompletionDictionaryVariables +
			sizecompletionDictionaryHandleGraphicsProperties;

		if (numberWordFound > 0)
		{
			if (numberWordFound == 1)
			{
				char **completionDictionary = NULL;
				char *newline = NULL;

				if (completionDictionaryFunctions) completionDictionary = completionDictionaryFunctions;
				if (completionDictionaryCommandWords) completionDictionary = completionDictionaryCommandWords;
				if (completionDictionaryMacros) completionDictionary = completionDictionaryMacros;
				if (completionDictionaryVariables) completionDictionary = completionDictionaryVariables;
				if (completionDictionaryHandleGraphicsProperties) completionDictionary = completionDictionaryHandleGraphicsProperties;

				newline = completeLine(lineBeforeCaret, completionDictionary[0],NULL,defaultPattern,FALSE, lineAfterCaret);
				if (newline)
				{
					clearCurrentLine();
					copyLine(newline);
					FREE(newline);
				}
			}
			else
			{
				char *commonFunctions = getCommonPart(completionDictionaryFunctions,sizecompletionDictionaryFunctions);
				char *commonCommandWords = getCommonPart(completionDictionaryCommandWords,sizecompletionDictionaryCommandWords);
				char *commonMacros = getCommonPart(completionDictionaryMacros,sizecompletionDictionaryMacros);
				char *commonVariables = getCommonPart(completionDictionaryVariables,sizecompletionDictionaryVariables);
				char *commonHandleGraphicsProperties = getCommonPart(completionDictionaryHandleGraphicsProperties,sizecompletionDictionaryHandleGraphicsProperties);

				char *commonAll = NULL;

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
					freeArrayOfString(commonsDictionary, sizecommonsDictionary);
				}

				displayCompletionDictionary(completionDictionaryFunctions, sizecompletionDictionaryFunctions,gettext("Scilab Function"));
				displayCompletionDictionary(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords,gettext("Scilab Command"));
				displayCompletionDictionary(completionDictionaryMacros, sizecompletionDictionaryMacros,gettext("Scilab Macro"));
				displayCompletionDictionary(completionDictionaryVariables, sizecompletionDictionaryVariables,gettext("Scilab Variable"));
				displayCompletionDictionary(completionDictionaryHandleGraphicsProperties, sizecompletionDictionaryHandleGraphicsProperties,gettext("Graphics handle field"));

				displayPrompt();
				newLine();

				if (commonAll)
				{
					char *newline = NULL;

					newline = completeLine(lineBeforeCaret, commonAll,NULL,defaultPattern,FALSE,lineAfterCaret);

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

			freeArrayOfString(completionDictionaryFunctions,sizecompletionDictionaryFunctions);
			freeArrayOfString(completionDictionaryCommandWords,sizecompletionDictionaryCommandWords);
			freeArrayOfString(completionDictionaryMacros,sizecompletionDictionaryMacros);
			freeArrayOfString(completionDictionaryVariables,sizecompletionDictionaryVariables);
			freeArrayOfString(completionDictionaryHandleGraphicsProperties,sizecompletionDictionaryHandleGraphicsProperties);
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

	if (LineBeforeCaret) {FREE(LineBeforeCaret); LineBeforeCaret = NULL;}
	if (LineAfterCaret) {FREE(LineAfterCaret); LineAfterCaret = NULL;}
	if (fileSearchedPattern) {FREE(fileSearchedPattern); fileSearchedPattern = NULL;}
	if (SearchedPattern) {FREE(SearchedPattern); SearchedPattern = NULL;}
}
/*--------------------------------------------------------------------------*/
static void displayCompletionDictionary(char **dictionary,int sizedictionary, char *namedictionary)
{
	if (dictionary)
	{
		int i = 0;
		int lenCurrentLine = 0;

		TerminalPrintf("\n");
		TerminalPrintf(namedictionary);
		TerminalPrintf(":");
		TerminalPrintf("\n");

		for(i = 0;i < sizedictionary;i++)
		{
			int newlenLine = lenCurrentLine + (int)strlen(dictionary[i]) + (int)strlen(" ");
			if ( newlenLine >= (getColumnsSize() - 10) )
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

	if (string1) newsize++;
	if (string2) newsize++;
	if (string3) newsize++;
	if (string4) newsize++;
	if (string5) newsize++;

	if (newsize > 0)
	{
		arrayOfString = (char**)MALLOC(sizeof(char*) *(newsize));
		if (arrayOfString)
		{
			int i = 0;
			if (string1) {arrayOfString[i] = string1; i++;}
			if (string2) {arrayOfString[i] = string2; i++;}
			if (string3) {arrayOfString[i] = string3; i++;}
			if (string4) {arrayOfString[i] = string4; i++;}
			if (string5) {arrayOfString[i] = string5; i++;}
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
