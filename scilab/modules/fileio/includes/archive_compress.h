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
/*------------------------------------------------------------------*/
#ifndef __ARCHIVE_COMPRESS_H__
#define __ARCHIVE_COMPRESS_H__

#include "dynlib_fileio.h"

/**
* Returns a list of files in the archive
* @param[filename] The archive name
* @param[file_list] Space seperated file list
* @param[arg_list] The argument for the format of the archive
* @return returns a 0 on success and -1 on failure
*/

FILEIO_IMPEXP int archive_compress(char *archive_name, char **file_list, int size, char *arg_list, int *error);


#endif
/*--------------------------------------------------------------------------*/
