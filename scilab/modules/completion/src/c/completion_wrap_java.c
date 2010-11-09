/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include <string.h>
#include "completion_wrap_java.h"
#include "completion.h"
/*--------------------------------------------------------------------------*/
char **searchAllDictionaries(char *somechars)
{
	int nbrElements = 0;
	if ( (somechars) && (strcmp(somechars,"")) ) 
	{
		return completion(somechars,&nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchFunctionsDictionary(char *somechars)
{
	int nbrElements = 0;
	if ( (somechars) && (strcmp(somechars,"")) ) 
	{
		return completionOnFunctions(somechars,&nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchCommandsDictionary(char *somechars)
{
	int nbrElements = 0;
	if ( (somechars) && (strcmp(somechars,"")) ) 
	{
		return completionOnCommandWords(somechars,&nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchMacrosDictionary(char *somechars)
{
	int nbrElements = 0;
	if ( (somechars) && (strcmp(somechars,"")) ) 
	{
		return completionOnMacros(somechars,&nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchVariablesDictionary(char *somechars)
{
	int nbrElements = 0;
	if ( (somechars) && (strcmp(somechars,"")) ) 
	{
		return completionOnVariablesWithoutMacros(somechars,&nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchFilesDictionary(char *somechars)
{
	int nbrElements = 0;
	if ( (somechars) && (strcmp(somechars,"")) ) 
	{
		return completionOnFiles(somechars,&nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchFieldsDictionary(char *lineBeforeCaret, char *pattern)
{
	int nbrElements = 0;
	if ( (lineBeforeCaret) && (strcmp(lineBeforeCaret,"")) ) 
	{
	        return completionOnFields(lineBeforeCaret, pattern, &nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
char **searchHandleGraphicsPropertiesDictionary(char *somechars)
{
	int nbrElements = 0;
	if ( (somechars) && (strcmp(somechars,"")) ) 
	{
		return completionOnHandleGraphicsProperties(somechars,&nbrElements);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
