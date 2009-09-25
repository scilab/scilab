/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 * Copyright (C) DIGITEO - 2009 - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#ifdef _MSC_VER
	#include <windows.h>
	#include <direct.h>
	#include <errno.h>
#else
	#include <unistd.h>
	#define GETCWD(x,y) getcwd(x,y)
#endif
#include "sciprint.h"
#include "scicurdir.h"
#include "MALLOC.h"
#include "localization.h"
#include "charEncoding.h"
#include "warningmode.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
int scichdirW(wchar_t *wcpath)
{
#ifndef _MSC_VER
	char *path = NULL;
	if (wcpath == NULL)
	{
		return 1;
	}

	path = wide_string_to_UTF8(wcpath);
	if (path == NULL)
	{
		return 1;
	}

	if (chdir(path) == -1)
	{
		if ( getWarningMode() ) sciprint(_("Can't go to directory %s.\n"), path);
		if (path) {FREE(path); path = NULL;}
		return 1;
	}

	if (path) {FREE(path); path = NULL;}

#else
	if (wcpath == NULL)
	{
		return 1;
	}

	if ( _wchdir(wcpath) )
	{
		switch (errno)
		{
		case ENOENT:
			{
				if ( getWarningMode() ) 
				{
					char *path = wide_string_to_UTF8(wcpath);
					if (path)
					{
						sciprint(_("Can't go to directory %s.\n"), path);
						FREE(path);
						path = NULL;
					}
				}
			}
			break;
		case EINVAL:
			{
				if ( getWarningMode() ) sciprint(_("Invalid buffer.\n"));
			}
			break;
		default:
			{
				if ( getWarningMode() ) sciprint(_("Unknown error.\n"));
			}
		}
		return 1;
	}
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
int scichdir(char *path)
{
	int ierr = 1;
	wchar_t *wcpath = NULL;
	if (path == NULL) return ierr;
	wcpath = to_wide_string(path);
	if (wcpath == NULL) return ierr;
	ierr = scichdirW(wcpath);
	FREE(wcpath);
	wcpath = NULL;
	return ierr;
}
/*--------------------------------------------------------------------------*/
wchar_t * scigetcwdW(int *err)
{
	wchar_t *wcCurrentDir = NULL;
	
#ifndef _MSC_VER
	char currentDir[PATH_MAX + 1];
	if (GETCWD(currentDir, PATH_MAX) == NULL)
	{
		if ( getWarningMode() ) sciprint(_("Can't get current directory.\n"));
		*err = 1;
	}
	else
	{
		wcCurrentDir = to_wide_string(currentDir);
		*err = 0;
	}
#else
	wchar_t wcdir[PATH_MAX + 1];
	if ( _wgetcwd(wcdir, PATH_MAX) == NULL )
	{
		if ( getWarningMode() ) sciprint(_("Can't get current directory.\n"));
		*err = 1;
	}
	else
	{
		wcCurrentDir = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(wcdir) + 1));
		if (wcCurrentDir)
		{
			wcscpy(wcCurrentDir, wcdir);
		}
	}
#endif
	return wcCurrentDir;
}
/*--------------------------------------------------------------------------*/
char * scigetcwd(int *err)
{
	char *currentDir = NULL;
	wchar_t *wcCurrentDir = scigetcwdW(err);
	if (wcCurrentDir)
	{
		currentDir = wide_string_to_UTF8(wcCurrentDir);
		FREE(wcCurrentDir);
		wcCurrentDir = NULL;
	}
	return currentDir;
}
/*--------------------------------------------------------------------------*/
