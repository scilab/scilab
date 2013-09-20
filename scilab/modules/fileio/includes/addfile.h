/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __ADDFILE_H__
#define __ADDFILE_H__

#include <stdio.h>
#include "dynlib_fileio.h"
#include "machine.h"

/**
 *add a file in the files table
 *
 * @param fd
 * @param fa
 * @param swap2
 * @param type
 * @param mode
 * @param filename
 * @param ierr
 */
FILEIO_IMPEXP void C2F(addfile)(int *fd, FILE *fa, int *swap2, int *type, int *mode, char *filename, int *ierr);

#endif /*  __ADDFILE_H__ */
/*--------------------------------------------------------------------------*/
