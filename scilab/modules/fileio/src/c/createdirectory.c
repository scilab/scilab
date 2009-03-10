/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
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
#include <wchar.h>
#endif
#include "createdirectory.h"
#include "isdir.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#define DIRMODE 0777
/*--------------------------------------------------------------------------*/
/* convert UTF-8 to Multi-byte character set (Default on Windows) */
/* Example to show how to do on Windows */
/* We need to use "Multi-byte" C function compatible */
#ifdef _MSC_VER
static WCHAR *to_wide_string(LPCSTR pStr)
{
	int nwide = 0;
	WCHAR *buf = NULL;

	if(pStr == NULL) return NULL;
	nwide = MultiByteToWideChar(CP_UTF8, 0, pStr, -1, NULL, 0);
	if(nwide == 0) return NULL;
	buf = MALLOC(nwide * sizeof(WCHAR));
	if(buf == NULL) return NULL;
	if(MultiByteToWideChar(CP_UTF8, 0, pStr, -1, buf, nwide) == 0) 
	{
		FREE(buf);
		return NULL;
	}
	return buf;
}
#endif
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
	WCHAR *wpath = NULL;
	wpath = to_wide_string(path);
	/* we replace mkdir by _wmkdir (Multi byte) */
	if ( _wmkdir(wpath) == 0) bOK = TRUE;
	FREE(wpath);
#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/
