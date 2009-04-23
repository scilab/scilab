/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
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
static char cur_dir[PATH_MAX];
/*--------------------------------------------------------------------------*/
int scichdir(char *path,int *err)
{
	*err=0;
	if (path == (char*) NULL)
	{
		*cur_dir = '\0';
		return (0);
	}

#ifndef _MSC_VER
	if (chdir(path) == -1)
	{
		if ( getWarningMode() ) sciprint(_("Can't go to directory %s.\n"), path);
		*err=1;
	}
#else
	/**/
	{
		wchar_t *pwTemp = to_wide_string(path);

		if ( _wchdir(pwTemp) )
		{
			switch (errno)
			{
			case ENOENT:
				if ( getWarningMode() ) sciprint(_("Can't go to directory %s.\n"), path);
				break;
			case EINVAL:
				if ( getWarningMode() ) sciprint(_("Invalid buffer.\n"));
				break;
			default:
				if ( getWarningMode() ) sciprint(_("Unknown error.\n"));
			}
			*err = 1;
		}
		FREE(pwTemp);
	}
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/
int scigetcwd(char **path,int *lpath,int *err)
{
#ifndef _MSC_VER
	if (GETCWD(cur_dir, PATH_MAX) == (char*) 0)
#else
	wchar_t wcdir[PATH_MAX];
	if ( _wgetcwd(wcdir, PATH_MAX) == (wchar_t*) 0 )
#endif
	{
		/* get current working dir */
		if ( getWarningMode() ) sciprint(_("Can't get current directory.\n"));
		*cur_dir = '\0';
		*lpath = 0;
		*err = 1;
	}
  else
	{
#ifdef _MSC_VER
		char *tmpcdir = wide_string_to_UTF8(wcdir);
		if (tmpcdir)
		{
			strcpy(cur_dir,tmpcdir);
			FREE(tmpcdir);
			tmpcdir = NULL;
			*path = cur_dir;
			*lpath =(int)strlen(cur_dir);
			*err = 0;
		}
		else
		{
			*cur_dir = '\0';
			*lpath=0;
			*err = 1;
		}
#else
	*path = cur_dir;
	*lpath =(int)strlen(cur_dir);
	*err = 0;
#endif
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
