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

#ifndef __COMPLETION_H__
#define __COMPLETION_H__

#include "dynlib_completion.h"

/**
 * completion function on all types for scilab
 * @param[in] somechars first characters of a word
 * @param[out] sizeArrayReturned
 * @return an array of chars
 */
COMPLETION_IMPEXP char **completion(const char *somechars, int *sizeArrayReturned);

/**
* completion function on Functions for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars
*/
COMPLETION_IMPEXP char **completionOnFunctions(const char *somechars, int *sizeArrayReturned);

/**
* completion function on command words for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars
*/
COMPLETION_IMPEXP char **completionOnCommandWords(const char *somechars, int *sizeArrayReturned);

/**
* completion function on Macros in libraries for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars
*/
COMPLETION_IMPEXP char **completionOnMacros(const char *somechars, int *sizeArrayReturned);

/**
* completion function on Variables for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars
*/
COMPLETION_IMPEXP char **completionOnVariables(const char *somechars, int *sizeArrayReturned);

/**
* completion function on Variables for scilab
* Macros already defined as variables are removed
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars
*/
COMPLETION_IMPEXP char **completionOnVariablesWithoutMacros(const char *somechars, int *sizeArrayReturned);

/**
* completion function on files for scilab
* @param[in] somechars first characters of a word
* @param[out] sizeArrayReturned
* @return an array of chars
*/
COMPLETION_IMPEXP char **completionOnFiles(const char *somechars, int *sizeArrayReturned);

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
COMPLETION_IMPEXP char **completionOnHandleGraphicsProperties(const char *somechars, int *sizeArrayReturned);
/*--------------------------------------------------------------------------*/
#endif /* __COMPLETION_H__ */
