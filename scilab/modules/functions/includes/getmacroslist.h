/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __GETMACROSLIST_H__
#define __GETMACROSLIST_H__

#include "dynlib_functions.h"

/**
* get scilab macros list
* @param[out] size of returned array
* @return array of strings
*/
FUNCTIONS_SCILAB_IMPEXP char **getmacroslist(int *sizearray);

#endif /* __GETMACROSDICTIONARY_H__ */
/*--------------------------------------------------------------------------*/ 
