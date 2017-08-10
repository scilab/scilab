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
#ifndef __MCLOSE_H__
#define __MCLOSE_H__
#include "dynlib_fileio.h"
#include "machine.h"

/*--------------------------------------------------------------------------*/
#define ALL_FILES_DESCRIPTOR -2
/*--------------------------------------------------------------------------*/

/**
* close the file with id *fd
* if *id != -1 and *id != -2
* the current file if *id = -1
* all opened file if *id = -2
*/
FILEIO_IMPEXP void C2F(mclose) (int *fd, double *res);
FILEIO_IMPEXP int mclose(int _iID);
FILEIO_IMPEXP int mcloseAll();
FILEIO_IMPEXP int mcloseCurrentFile();
#endif /* __MCLOSE_H__ */
/*--------------------------------------------------------------------------*/
