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
