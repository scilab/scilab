
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __ISLETTER_H__
#define __ISLETTER_H__

#include <wchar.h>
#include "BOOL.h"

/**
* array elements that are alphabetic letters
* @param[in] a string
* @return BOOL array
*/
BOOL *isletter(char *input_string, int *sizeArray);

/**
* array elements that are alphabetic letters
* @param[in] a string
* @return BOOL array
*/
BOOL *isletterW(wchar_t *wcInput_string, int *sizeArray);

#endif /* __ISLETTER_H__ */
/*--------------------------------------------------------------------------*/
