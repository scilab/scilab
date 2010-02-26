
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * Copyright (C) DIGITEO - Allan CORNET - 2009
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __ISALPHANUM_H__
#define __ISALPHANUM_H__

#include <wchar.h>
#include "BOOL.h"

/**
* array elements that are alphabetic letters or digits
* @param[in] a string
* @return BOOL array 
*/
BOOL *isalphanumW(wchar_t *input_string, int *returnedSize);

#endif /* __ISALPHANUM_H__ */
/*--------------------------------------------------------------------------*/
