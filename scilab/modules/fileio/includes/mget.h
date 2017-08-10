/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
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
FILEIO_IMPEXP void C2F(mget) (int *fd, double *res, int *n, const char *type, int *ierr);

FILEIO_IMPEXP void C2F(mgetnc)(int *fd, void * res, int *n1, const char *type, int *ierr);

FILEIO_IMPEXP void mget2(FILE *fa, int swap, double *res, int n, const char *type, int *ierr);

#endif /* __MGET_H__ */
/*--------------------------------------------------------------------------*/
