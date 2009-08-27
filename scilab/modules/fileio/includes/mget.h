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
#ifndef __MGET_H__
#define __MGET_H__

#include <stdio.h>
#include "dynlib_fileio.h"
#include "machine.h"

/**
* mget functions  read data in a machine 
* independant way (i.e write in little-endian) 
* or can be used to read in little or big endian 
* if read fails *ierr contains the number of properly read items 
*/
FILEIO_IMPEXP void C2F(mget) (int *fd, double *res, int *n, char *type, int *ierr);

FILEIO_IMPEXP void C2F(mgetnc)(int *fd, void * res, int *n1, char *type, int *ierr);

FILEIO_IMPEXP void mget2(FILE *fa, int swap, double *res, int n, char *type, int *ierr);

#endif /* __MGET_H__ */
/*--------------------------------------------------------------------------*/
