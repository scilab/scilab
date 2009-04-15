/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA
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
#ifndef _MSC_VER
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#else
#include <Windows.h>
#endif
#include "charEncoding.h"
#include "MALLOC.h"
#include "createdirectory.h"
#include "isdir.h"
/*--------------------------------------------------------------------------*/
#define DIRMODE 0777
/*--------------------------------------------------------------------------*/
BOOL createdirectory(const char *path)
{
	BOOL bOK=FALSE;
#ifndef _MSC_VER
	if  (!isdir(path)) 
	{
		if (mkdir(path, DIRMODE) == 0) bOK=TRUE;
	}
#else
	wchar_t *widePath = to_wide_string((char*)path);
	if (widePath)
	{
		if (CreateDirectoryW(widePath,NULL)) bOK = TRUE;
		FREE(widePath);
	}
#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/
