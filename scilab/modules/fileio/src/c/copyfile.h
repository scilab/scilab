/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __COPYFILE_H__
#define __COPYFILE_H__

#include <wchar.h>
#include "dynlib_fileio.h"

/**
* copy a file
* @param[in] destination filename
* @param[in] source filename
* @return error number
*/
FILEIO_IMPEXP int CopyFileFunction(char* DestinationFilename, char* SourceFilename);

/**
* copy a directory
* @param[in] destination directory
* @param[in] source directory
* @return error number
*/
FILEIO_IMPEXP int CopyDirectoryFunction(char* DestinationDirectory, char* SourceDirectory);

#endif /* __COPYFILE_H__ */
/*--------------------------------------------------------------------------*/
