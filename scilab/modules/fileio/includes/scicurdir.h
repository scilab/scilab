/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
