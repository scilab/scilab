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

#ifndef __GETFILESDICTIONARY_H__
#define __GETFILESDICTIONARY_H__

#include "BOOL.h"

/**
* get files dictionary
* @param[in] some chars
* @param[out] size of returned array
* @param[in] returns with the full path name
* @return array of strings
*/
char **getfilesdictionary(char *somechars, int *sizearray, BOOL fullpath);

#endif /* __GETFILESDICTIONARY_H__ */
/*--------------------------------------------------------------------------*/
