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
#include "completion_wrap_java.h"
#include "completion.h"
/*--------------------------------------------------------------------------*/
char **searchAllDictionaries(char *somechars)
{
	int nbrElements = 0;
	return completion(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchFunctionsDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnFunctions(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchCommandsDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnCommandWords(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchMacrosDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnMacros(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchVariablesDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnVariablesWithoutMacros(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchFilesDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnFiles(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
char **searchHandleGraphicsPropertiesDictionary(char *somechars)
{
	int nbrElements = 0;
	return completionOnHandleGraphicsProperties(somechars,&nbrElements);
}
/*--------------------------------------------------------------------------*/
