
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __STRSUBST_H__
#define __STRSUBST_H__

#include "dynlib_string.h"

/**
* substitute a character string by another in a character string in string matrix
* @param[in] strings_input
* @param[in] dimension of strings_input
* @param[in] string_to_search
* @param[in] replacement_string
* @return substituted string matrix
*/
STRING_IMPEXP char **strsubst(char **strings_input,int strings_dim,char *string_to_search,char *replacement_string);
STRING_IMPEXP char **strsubst_reg(char **strings_input,int strings_dim,char *string_to_search,char *replacement_string, int *ierr);

/**
* substitute a character string by another in a character string
* @param[in] string 
* @param[in] string to search in input_string
* @param[in] replacement_string
* @param[out] error returned by pcre_private
* @return substituted string
*/
STRING_IMPEXP char *strsub(char* input_string, const char* string_to_search, const char* replacement_string);
STRING_IMPEXP char *strsub_reg(char* input_string, const char* string_to_search, const char* replacement_string, int *ierr);

#endif /* __STRSUBST_H__ */
