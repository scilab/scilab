/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA
 * Copyright (C) 2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <stdlib.h> 
#include <string.h> /* strlen */
#ifdef _MSC_VER
	#include <Windows.h> /* GetEnvironmentVariable */
	#include "strdup_windows.h"
#endif
#include "MALLOC.h"
#include "getenvc.h"
#include "localization.h"
#include "sciprint.h"
#include "FileExist.h"
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static void searchenv_others(const char *filename, const char *varname,
   			    char *pathname);
#endif
/*--------------------------------------------------------------------------*/
void C2F(getenvc)(int *ierr,char *var,char *buf,int *buflen,int *iflag)
{
	#ifdef _MSC_VER
	if (GetEnvironmentVariable(var,buf,(DWORD)*buflen) == 0)
	{
		if ( *iflag == 1 ) sciprint(_("Undefined environment variable %s.\n"),var);
		*ierr=1;
	}
	else
	{
		*buflen = (int)strlen(buf);
		*ierr=0;
	}
	#else
	char *local;
	if ( (local=getenv(var)) == 0)
	{
		if ( *iflag == 1 ) sciprint(_("Undefined environment variable %s.\n"),var);
		*ierr=1;
	}
	else
	{
		strncpy(buf,local,*buflen);
		*buflen = strlen(buf);
		*ierr=0;
	}
	#endif
}
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static void searchenv_others(const char *filename, 
			    const char *varname,
			    char *pathname)
{
	char *cp = NULL;

	*pathname = '\0';

	if( filename[0] == DIR_SEPARATOR[0])
	{
		strcpy(pathname, filename);
		return;
	}
	
	cp = getenv(varname);
	if(cp == NULL)
	{
		/* environment Variable not defined. */
		return;
	}

	while(*cp)
	{
		char *concat = NULL;
		*pathname = '\0';
        concat = pathname;
		/* skip PATH_SEPARATOR[0] and empty entries */
		while( (*cp) && (*cp == PATH_SEPARATOR[0]) )
		{
			cp++;
		}

		/* copy path */
		while( (*cp) && (*cp != PATH_SEPARATOR[0]) )
		{
			*concat = *cp;
			cp++;
			concat++;
		}
		
		if ( concat == pathname )
		{
			/* filename not found */
			*pathname = '\0';
			return;
		}
		
		if( *(concat-1) != DIR_SEPARATOR[0] )
		{
			/* add directory separator */
			*concat = DIR_SEPARATOR[0];
			concat++;
		}

		/* concatate path & filename */
		strcpy(concat, filename);

		/* file exists ? */
		if(FileExist(pathname))
		{
			// file found
			return;
		}
	}

	/* file not found */
	*pathname = '\0';
}
#endif
/*--------------------------------------------------------------------------*/
char *searchEnv(const char *name,const char *env_var)
{
	char *buffer = NULL;
	char fullpath[PATH_MAX];

	strcpy(fullpath,"");

	#if _MSC_VER 
		_searchenv(name,env_var,fullpath);
	#else
		searchenv_others(name,env_var,fullpath);
	#endif

	if (strlen(fullpath) > 0) buffer = strdup(fullpath);
	return buffer;
}
/*--------------------------------------------------------------------------*/

