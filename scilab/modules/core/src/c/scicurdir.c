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
#include "machine.h"
#ifdef _MSC_VER 
	#include <windows.h>
	#include <direct.h>
#else 
	#include <unistd.h>
	#define GETCWD(x,y) getcwd(x,y)
#endif 
#include "sciprint.h"
#include "scicurdir.h"
#include "MALLOC.h"
#include "localization.h"
#include "warningmode.h"
static char cur_dir[PATH_MAX];
/*--------------------------------------------------------------------------*/
/* checks if it is a UNC path */
static BOOL isUNCpath(char *path);
/*--------------------------------------------------------------------------*/
int C2F(scichdir)(char *path,int *err)
{
	*err=0;
	if (path == (char*) NULL) 
	{
		*cur_dir = '\0';
		return (0);
	}

	 if (isUNCpath(path))
	 {
	    if ( getWarningMode() ) sciprint(_("Can't go to directory %s.\n"), path); 
		*err = 1;
		return (0);
	 }

#ifndef _MSC_VER
	if (chdir(path) == -1) 
#else
	if (SetCurrentDirectory(path) == 0)
#endif
	{
		if ( getWarningMode() ) sciprint(_("Can't go to directory %s.\n"), path); 
	    *err=1;
	} 
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(scigetcwd)(char **path,int *lpath,int *err)
{
#ifndef _MSC_VER
	if (GETCWD(cur_dir, PATH_MAX) == (char*) 0)
#else
	if ( _getcwd(cur_dir, PATH_MAX) == (char*) 0 )
#endif
	{
		/* get current working dir */
		if ( getWarningMode() ) sciprint(_("Can't get current directory.\n"));
		*cur_dir = '\0';
		*lpath=0;
		*err=1;
	}
    else 
	{
		*path= cur_dir;
		*lpath=(int)strlen(cur_dir);
		*err=0;
	}
    return 0;
}
/*--------------------------------------------------------------------------*/
static BOOL isUNCpath(char *path)
{
	if ( (path) && (strncmp(path,"\\\\",2) == 0) )
	{
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
