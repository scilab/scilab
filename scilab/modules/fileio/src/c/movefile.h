/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
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
/*--------------------------------------------------------------------------*/
#ifndef __MOVEFILE_H__
#define __MOVEFILE_H__

#include <wchar.h>
#include "dynlib_fileio.h"

/**
* move a file
* @param[in] destination filename
* @param[in] source filename
* @return error number
*/
FILEIO_IMPEXP int MoveFileFunction(wchar_t *DestinationFilename, wchar_t *SourceFilename);

/**
* move a directory
* @param[in] destination directory
* @param[in] source directory
* @return error number
*/
FILEIO_IMPEXP int MoveDirectoryFunction(wchar_t *DestinationDirectory, wchar_t *SourceDirectory);

#endif /* __MOVEFILE_H__ */
/*--------------------------------------------------------------------------*/
