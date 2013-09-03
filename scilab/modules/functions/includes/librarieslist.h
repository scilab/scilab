/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __LIBRARIESLIST_H__
#define __LIBRARIESLIST_H__

#include "dynlib_functions.h"

/**
* get list of libraries in scilab's stack
* @param[out] int size of array returned
* @return list of libraries
*/
FUNCTIONS_SCILAB_IMPEXP char **getlibrarieslist(int *sizearray);

#endif /* __LIBRARIESLIST_H__ */
/*--------------------------------------------------------------------------*/
