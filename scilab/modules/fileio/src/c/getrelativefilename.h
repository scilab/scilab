/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Pierre MARECHAL
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

#ifndef __GETRELATIVEFILENAME_H__
#define __GETRELATIVEFILENAME_H__

#ifdef _MSC_VER
#define ABSOLUTE_NAME_START 3
#else
#define ABSOLUTE_NAME_START 1
#endif

// ABSOLUTE_NAME_START
//
// The number of characters at the start of an absolute filename.  e.g. in DOS,
// absolute filenames start with "X:\" so this value should be 3, in UNIX they start
// with "\" so this value should be 1.


/* ================================================================================== */
// getrelativefilename
//
// Given the absolute current directory and an absolute file name, returns a relative file name.
// For example, if the current directory is C:\foo\bar and the filename C:\foo\whee\text.txt is given,
// GetRelativeFilename will return ..\whee\text.txt.
/* ================================================================================== */

#include "wchar.h"
#include "dynlib_fileio.h"

FILEIO_IMPEXP char* getrelativefilename(char *currentDirectory, char *absoluteFilename);
FILEIO_IMPEXP wchar_t* getrelativefilenameW(wchar_t *currentDirectory, wchar_t *absoluteFilename);
#endif /* __GETRELATIVEFILENAME_H__ */
