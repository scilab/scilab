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
#ifndef __GETFULLFILENAME_H__
#define __GETFULLFILENAME_H__

#include <wchar.h>
#include "dynlib_fileio.h"

/**
* get full filename of a file (wide string)
* @param[in] FilenameInput can be a relative path
* @return a full filename (path expanded)
*/
FILEIO_IMPEXP wchar_t *getFullFilenameW(wchar_t* FilenameInput);

/**
* get full filename of a file
* @param[in] FilenameInput can be a relative path
* @return a full filename (path expanded)
*/
FILEIO_IMPEXP char *getFullFilename(char* Filename);

#endif /* __GETFULLFILENAME_H__ */
/*--------------------------------------------------------------------------*/
