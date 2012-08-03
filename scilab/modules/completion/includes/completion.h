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

#ifndef __COMPLETION_H__
#define __COMPLETION_H__

#include "dynlib_completion.h"

/**
 * completion function on all types for scilab
 * @param[in] somechars first characters of a word
 * @param[out] sizeArrayReturned
 * @return an array of chars 
 */
COMPLETION_IMPEXP char **completion(char *somechars, int *sizeArrayReturned);

/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnFunctions(char *somechars, int *sizeArrayReturned);

/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnCommandWords(char *somechars, int *sizeArrayReturned);

/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnMacros(char *somechars, int *sizeArrayReturned);

/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnVariables(char *somechars, int *sizeArrayReturned);

/**
* completion function on Variables for scilab
* Macros already defined as variables are removed
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnVariablesWithoutMacros(char *somechars, int *sizeArrayReturned);

/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnFiles(char *somechars, int *sizeArrayReturned);

/**
* completion function on files for scilab
* @param[in] the line before the caret
* @param[in] the part
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnFields(char *lineBeforeCaret, char *pattern, int *sizeArrayReturned);

/**
* completion handle graphics properties for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars 
*/
COMPLETION_IMPEXP char **completionOnHandleGraphicsProperties(char *somechars, int *sizeArrayReturned);
/*--------------------------------------------------------------------------*/
#endif /* __COMPLETION_H__ */
