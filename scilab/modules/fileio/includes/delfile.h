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
#ifndef __DELFILE_H__
#define __DELFILE_H__
#include "dynlib_fileio.h"
#include "machine.h"

/**
* delete a file in the files table
*/
FILEIO_IMPEXP void C2F(delfile)(int *fd);

#endif /* __DELFILE_H__ */
/*--------------------------------------------------------------------------*/
