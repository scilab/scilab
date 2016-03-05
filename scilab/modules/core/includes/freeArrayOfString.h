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
BOOL freeArrayOfString(char **Str, int dim);

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
