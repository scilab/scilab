/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __MOPEN_H__
#define __MOPEN_H__

#include "dynlib_fileio.h"
#include "machine.h"

/**
* Opens file given by file and return it's id 
* @param[out] fd (id)
* @param filename
* @param status "r","w","a" or "rb","wb","ab"
* @param swap
* @param[out] res
* @param[out] error 
*/

typedef enum { 
	MOPEN_NO_ERROR = 0,
	MOPEN_NO_MORE_LOGICAL_UNIT = 1,
	MOPEN_CAN_NOT_OPEN_FILE = 2,
	MOPEN_NO_MORE_MEMORY = 3,
	MOPEN_INVALID_FILENAME = 4,
	MOPEN_INVALID_STATUS = 5
} mopenError;

FILEIO_IMPEXP void C2F(mopen)(int *fd, char *file, char *status, int *f_swap, double *res, int *error);

#endif  /* __MOPEN_H__ */
/*--------------------------------------------------------------------------*/
