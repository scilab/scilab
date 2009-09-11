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
/*--------------------------------------------------------------------------*/ 
#ifndef __REMOVEDIR_H__
#define __REMOVEDIR_H__

#include <wchar.h>
#include "dynlib_fileio.h"
#include "BOOL.h" /* BOOL */

/**
* Remove a directory
* @param path the directory
* @return the result of the operation
*/
FILEIO_IMPEXP BOOL removedir(char *path);

/**
* Remove a directory
* @param path the directory (wide string)
* @return the result of the operation
*/
FILEIO_IMPEXP BOOL removedirW(wchar_t *pathW);

#endif /* __REMOVEDIR_H__ */
/*--------------------------------------------------------------------------*/ 
