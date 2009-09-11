/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
