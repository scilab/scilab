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
#ifndef __freeArrayOfString_H__
#define __freeArrayOfString_H__

#include <wchar.h>

#include "BOOL.h"

/**
* Free Array of String (char **)
* @param[in] ptr on char **
* @param[in] dimension of ptr on char **
* @return TRUE if free
*/
BOOL freeArrayOfString(char **Str,int dim);

/**
* Free Array of String (wchar_t **)
* @param[in] ptr on wchar_t **
* @param[in] dimension of ptr on wchar_t **
* @return TRUE if free
*/
BOOL freeArrayOfWideString(wchar_t **wcStr, int dim);

/**
* Free Array of void **
* @param[in] ptr on void **
* @param[in] dimension of ptr on void **
* @return TRUE if free
*/
BOOL freeArray(void **pArray, int dim);

#endif
/*---------------------------------------------------------------------------*/ 
