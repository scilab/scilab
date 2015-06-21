/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2015 - Kartik Gupta
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#ifndef __ARCHIVE_LIST_H__
#define __ARCHIVE_LIST_H__

#include "dynlib_fileio.h"

/**
* Returns a list of files in the archive
* @param[filename] The absolute or relative path to the archive.
* @param[size] The size of the list
* @param[error] The error code for error detection
* @return returns a pointer to a matrix containing the file names.
*/

FILEIO_IMPEXP char** archive_list(char *filename, int *size, int *error);


#endif
/*--------------------------------------------------------------------------*/
