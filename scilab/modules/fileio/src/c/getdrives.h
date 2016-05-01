/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2011 - Digiteo - Cedric DELAMARRE
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
#ifndef __GETDRIVES_H__
#define __GETDRIVES_H__

/**
* Get the drive letters of all mounted filesystems on the computer.
* @param[out] nbDrives
* @return List of Drives
*/
#include <wchar.h>
#include "dynlib_fileio.h"

FILEIO_IMPEXP char **getdrives(int *nbDrives);
FILEIO_IMPEXP wchar_t **getdrivesW(int *nbDrives);
#endif /* __GETDRIVES_H__ */
