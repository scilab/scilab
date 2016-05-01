/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * Copyright (C) INRIA - 2008 - Allan CORNET
 * Copyright (C) Digiteo - 2011 - Cedric DELAMARRE
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

/*------------------------------------------------------------------------*/
#ifndef __TOKENS_H__
#define __TOKENS_H__

/**
* stringTokens function
* @param[in] *str string where we search delimiters
* @param[in] **delim delimiters to search
* @param[in] sizedelim size of matrix delim
* @param[out] sizeOutputs size of matrix of string returned
* @return matrix of string tokens
*/
#include <wchar.h>
#include "dynlib_string.h"

STRING_IMPEXP wchar_t** stringTokens(wchar_t* str, wchar_t* delim, int* sizeOutputs);

#endif /* __TOKENS_H__ */
/*------------------------------------------------------------------------*/
