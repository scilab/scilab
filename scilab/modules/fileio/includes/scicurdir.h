/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#ifndef __SCICURDIR_H__
#define __SCICURDIR_H__

#include <wchar.h>
#include "dynlib_fileio.h"

/**
 * Changes scilab current directory 
 * @param path : the path where Scilab should go
 * @return err : the error code
 */
FILEIO_IMPEXP int scichdir(char *path);

/**
* Changes scilab current directory (wide string)
* @param path : the path where Scilab should go
* @return err : the error code
*/
FILEIO_IMPEXP int scichdirW(wchar_t *wcpath);

/**
 * Get scilab current directory 
 * @param[out] err : the error code
 * @return path : the current path
 */
FILEIO_IMPEXP char * scigetcwd(int *err);

/**
* Get scilab current directory (wide string)
* @param[out] err : the error code
* @return path : the current path
*/
FILEIO_IMPEXP wchar_t * scigetcwdW(int *err);

#endif /* __SCICURDIR_H__ */
/*--------------------------------------------------------------------------*/
