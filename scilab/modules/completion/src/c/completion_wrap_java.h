/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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

#ifndef __COMPLETION_WRAP_JAVA_H__
#define __COMPLETION_WRAP_JAVA_H__

/* wrap of completion.h (for java */
/* size of string array removed */
/* in C , I prefer to know dimensions */

/**
* completion function on all types for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
char **searchAllDictionaries(char *somechars);

/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
char **searchFunctionsDictionary(char *somechars);

/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
char **searchCommandsDictionary(char *somechars);

/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
char **searchMacrosDictionary(char *somechars);

/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
char **searchVariablesDictionary(char *somechars);

/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
char **searchFilesDictionary(char *somechars);

/**
* completion function on files for scilab
* @param[in] the line before the caret
* @param[in] the part
* @return an array of chars
*/
char **searchFieldsDictionary(char *lineBeforeCaret, char *pattern);

/**
* completion handle graphics properties for scilab
* @param[in] somechars first characters of a word
* @return an array of chars
*/
char **searchHandleGraphicsPropertiesDictionary(char *somechars);
/*--------------------------------------------------------------------------*/
#endif /* __COMPLETION_WRAP_JAVA_H__ */
