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

#ifndef __COMPLETION_WRAP_JAVA_H__
#define __COMPLETION_WRAP_JAVA_H__

/* wrap of completion.h (for java */
/* size of string array removed */
/* in C , I prefer to know dimensions */

/**
* completion function on all types for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
char **searchAllDictionaries(char *somechars);

/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
char **searchFunctionsDictionary(char *somechars);

/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
char **searchCommandsDictionary(char *somechars);

/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
char **searchMacrosDictionary(char *somechars);

/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
char **searchVariablesDictionary(char *somechars);

/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
char **searchFilesDictionary(char *somechars);

/**
* completion function on files for scilab
* @param[in] the line before the caret
* @param[in] the part
* @return a array of chars 
*/
char **searchFieldsDictionary(char *lineBeforeCaret, char *pattern);

/**
* completion handle graphics properties for scilab
* @param[in] somechars first characters of a word
* @return a array of chars 
*/
char **searchHandleGraphicsPropertiesDictionary(char *somechars);
/*--------------------------------------------------------------------------*/
#endif /* __COMPLETION_WRAP_JAVA_H__ */
