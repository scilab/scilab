/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2006 - INRIA - Pierre MARECHAL
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

char* getrelativefilename(char *currentDirectory, char *absoluteFilename);

#endif /* __GETRELATIVEFILENAME_H__ */
