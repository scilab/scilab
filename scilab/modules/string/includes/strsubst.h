
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __STRSUBST_H__
#define __STRSUBST_H__

#include <wchar.h>
#include "dynlib_string.h"

/**
* substitute a character string by another in a character string in string matrix
* @param[in] strings_input
* @param[in] dimension of strings_input
* @param[in] string_to_search
* @param[in] replacement_string
* @return substituted string matrix
*/
STRING_IMPEXP char **strsubst(const char **strings_input, int strings_dim, const char *string_to_search, const char *replacement_string);
STRING_IMPEXP char **strsubst_reg(const char **strings_input, int strings_dim, const char *string_to_search, const char *replacement_string, int *ierr);

/**
* substitute a character string by another in a character string
* @param[in] string
* @param[in] string to search in input_string
* @param[in] replacement_string
* @param[out] error returned by pcre_private
* @return substituted string
*/
STRING_IMPEXP char *strsub(const char* input_string, const char* string_to_search, const char* replacement_string);
STRING_IMPEXP char *strsub_reg(const char* input_string, const char* string_to_search, const char* replacement_string, int *ierr);


STRING_IMPEXP wchar_t **wcssubst(const wchar_t** _pwstInput, int _iInputSize, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace);
STRING_IMPEXP wchar_t **wcssubst_reg(const wchar_t** _pwstInput, int _iInputSize, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace, int* _piErr);

STRING_IMPEXP wchar_t *wcssub(const wchar_t* _pwstInput, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace);
STRING_IMPEXP wchar_t *wcssub_reg(const wchar_t* _pwstInput, const wchar_t* _pwstSearch, const wchar_t* _pwstReplace, int* _piErr);

#endif /* __STRSUBST_H__ */
