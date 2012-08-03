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
#ifndef __LIBRARYINFO_H__
#define __LIBRARYINFO_H__

#include "dynlib_functions.h"

/**
* get path of scilab library
* @param[in] name of scilab library
* @return a string : path of library
*/
FUNCTIONS_SCILAB_IMPEXP char *getlibrarypath(char *libraryname);

/**
* get macros list in a scilab's library
* @param[in] name of scilab library
* @param[out] size of array returned
* @return an array of string (macros list)
*/
FUNCTIONS_SCILAB_IMPEXP char **getlistmacrosfromlibrary(char *libraryname,int *sizearray);

#endif /* __LIBRARYINFO_H__ */
/*--------------------------------------------------------------------------*/
