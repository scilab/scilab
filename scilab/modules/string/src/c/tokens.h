
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * Copyright (C) INRIA - 2008 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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

char** stringTokens(char *str, char **delim, int sizedelim, int *sizeOutputs);

#endif /* __TOKENS_H__ */
/*------------------------------------------------------------------------*/
