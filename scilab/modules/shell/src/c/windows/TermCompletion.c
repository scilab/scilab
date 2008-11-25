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
#include "TermCompletion.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "TermLine.h"
#include "TermConsole.h"
#include "getPartLine.h"
#include "completion.h"
#include "scilines.h"
/*--------------------------------------------------------------------------*/
static void displayCompletionDictionary(char **dictionary,int sizedictionary, char *namedictionary);
/*--------------------------------------------------------------------------*/
void TermCompletion(void)
{
	char *CurrentLine = getCurrentLine();
	char *fileSearchedPattern = getFilePartLevel(CurrentLine);
	char *SearchedPattern = getPartLevel(CurrentLine);
	char **completionDictionaryFiles = NULL;
	int sizecompletionDictionaryFiles = 0;

	completionDictionaryFiles = completionOnFiles(fileSearchedPattern, &sizecompletionDictionaryFiles);

	if (completionDictionaryFiles)
	{
		if (sizecompletionDictionaryFiles == 1)
		{
			char *result = completionDictionaryFiles[0];
			char *partResult = &result[strlen(SearchedPattern)];
			char *newline = (char*)MALLOC(sizeof(char)*(strlen(CurrentLine)+ strlen(partResult)));

			if (newline)
			{
				strcpy(newline,CurrentLine);
				strcat(newline,partResult);
				clearCurrentLine();
				copyLine(newline);
				FREE(newline);
			}
		}
		else
		{
			displayCompletionDictionary(completionDictionaryFiles,sizecompletionDictionaryFiles,gettext("File or Directory"));
			TerminalPrintf("\n");
			displayPrompt();
			newLine();
			copyLine(CurrentLine);
		}
		FREE(CurrentLine);
		FREE(fileSearchedPattern);
		FREE(SearchedPattern);
		freeArrayOfString(completionDictionaryFiles,sizecompletionDictionaryFiles);
		return;
	}
	else
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

		char **completionDictionaryFiles = NULL;
		int sizecompletionDictionaryFiles = 0;

		char **completionDictionaryHandleGraphicsProperties = NULL;
		int sizecompletionDictionaryHandleGraphicsProperties = 0;

		if ( SearchedPattern && strcmp(SearchedPattern,"") )
		{

		completionDictionaryFunctions = completionOnFunctions(SearchedPattern, 
			&sizecompletionDictionaryFunctions);

		completionDictionaryCommandWords = completionOnCommandWords(SearchedPattern, 
			&sizecompletionDictionaryCommandWords);

		completionDictionaryMacros = completionOnMacros(SearchedPattern, 
			&sizecompletionDictionaryMacros);

		completionDictionaryVariables = completionOnVariablesWithoutMacros(SearchedPattern, 
			&sizecompletionDictionaryVariables);

		completionDictionaryFiles = completionOnFiles(SearchedPattern, 
			&sizecompletionDictionaryFiles);

		completionDictionaryHandleGraphicsProperties = completionOnHandleGraphicsProperties(SearchedPattern, 
			&sizecompletionDictionaryHandleGraphicsProperties);
		}

		numberWordFound = sizecompletionDictionaryFunctions + sizecompletionDictionaryCommandWords +
			sizecompletionDictionaryMacros + sizecompletionDictionaryVariables +
			sizecompletionDictionaryHandleGraphicsProperties;

		if (numberWordFound)
		{
			if (numberWordFound == 1)
			{
				char **completionDictionary = NULL;
				char *result = NULL;
				char *partResult = NULL;
				char *newline = NULL;

				if (completionDictionaryFunctions) completionDictionary = completionDictionaryFunctions;
				if (completionDictionaryCommandWords) completionDictionary = completionDictionaryCommandWords;
				if (completionDictionaryMacros) completionDictionary = completionDictionaryMacros;
				if (completionDictionaryVariables) completionDictionary = completionDictionaryVariables;
				if (completionDictionaryHandleGraphicsProperties) completionDictionary = completionDictionaryHandleGraphicsProperties;

				result = completionDictionary[0];
				partResult = &result[strlen(SearchedPattern)];
				newline = (char*)MALLOC(sizeof(char)*(strlen(CurrentLine)+ strlen(partResult)));

				if (newline)
				{
					strcpy(newline,CurrentLine);
					strcat(newline,partResult);
					clearCurrentLine();
					copyLine(newline);
					FREE(newline);
				}
			}
			else
			{
				displayCompletionDictionary(completionDictionaryFunctions,sizecompletionDictionaryFunctions,gettext("Scilab Function"));
				displayCompletionDictionary(completionDictionaryCommandWords, sizecompletionDictionaryCommandWords,gettext("Scilab Command"));
				displayCompletionDictionary(completionDictionaryMacros,sizecompletionDictionaryMacros,gettext("Scilab Macro"));
				displayCompletionDictionary(completionDictionaryVariables,sizecompletionDictionaryVariables,gettext("Scilab Variable"));
				displayCompletionDictionary(completionDictionaryHandleGraphicsProperties,sizecompletionDictionaryHandleGraphicsProperties,gettext("Graphics handle field"));

				TerminalPrintf("\n");
				displayPrompt();
				newLine();
				copyLine(CurrentLine);
			}
			freeArrayOfString(completionDictionaryFunctions,sizecompletionDictionaryFunctions);
			freeArrayOfString(completionDictionaryCommandWords,sizecompletionDictionaryCommandWords);
			freeArrayOfString(completionDictionaryMacros,sizecompletionDictionaryMacros);
			freeArrayOfString(completionDictionaryVariables,sizecompletionDictionaryVariables);
			freeArrayOfString(completionDictionaryFiles,sizecompletionDictionaryFiles);
			freeArrayOfString(completionDictionaryHandleGraphicsProperties,sizecompletionDictionaryHandleGraphicsProperties);
			FREE(CurrentLine);
			FREE(fileSearchedPattern);
			FREE(SearchedPattern);
		}
		else
		{
			FREE(CurrentLine);
			FREE(fileSearchedPattern);
			FREE(SearchedPattern);
		}
	}
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
