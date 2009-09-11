/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#ifdef _MSC_VER
	#include <Windows.h>
#else
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <dirent.h>
#endif
#include <string.h>
#include "isDrive.h"
#include "isdir.h"
#include "MALLOC.h"
#include "charEncoding.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
BOOL isdir(const char * path)
{
	BOOL bOK = FALSE;
#ifndef _MSC_VER
	struct stat buf;
	if (path == NULL) return FALSE;
	if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode)) bOK=TRUE;
#else
	wchar_t *wcpath = to_wide_string((char*)path);
	if (wcpath == NULL) return FALSE;
	bOK = isdirW(wcpath);
	FREE(wcpath);
#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL isdirW(const wchar_t * wcpath)
{
	BOOL bOK = FALSE;
#ifndef _MSC_VER
	struct stat buf;
	char *path = wide_string_to_UTF8(wcpath);
	if (path == NULL) return FALSE;
	bOK = isdir(path);
	FREE(path);
#else
	if (isDriveW(wcpath)) return TRUE;
	else
	{
		DWORD attr = 0;
		wchar_t *tmpPath = wstrdup(wcpath);

		if ( (tmpPath[wcslen(tmpPath) - 1] == L'\\') || (tmpPath[wcslen(tmpPath) - 1] == L'/') )
		{
			tmpPath[wcslen(tmpPath) - 1] = L'\0';
		}
		attr = GetFileAttributesW(tmpPath);
		FREE(tmpPath);
		if (attr == INVALID_FILE_ATTRIBUTES) return FALSE;
		return ((attr & FILE_ATTRIBUTE_DIRECTORY) != 0) ? TRUE : FALSE;
	}
#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/
