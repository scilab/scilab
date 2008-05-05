/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h> /* strcmp */
#include <stdio.h> /* sprintf */
#include "getfilesdictionary.h"
#include "../../../core/src/c/scicurdir.h" /* C2F(scigetcwd) */
#include "findfiles.h" /* findfiles */
#include "MALLOC.h"
#include "cluni0.h"
/*--------------------------------------------------------------------------*/ 
static void splitpath(char *composite,  char *path,  char *fname);
/*--------------------------------------------------------------------------*/ 
char **getfilesdictionary(char *somechars,int *sizearray)
{
	char **dictionary = NULL;

	if (somechars)
	{
		int sizeListReturned = 0;
		char path[PATH_MAX];
		
		char filespec[PATH_MAX];

		char pathname[PATH_MAX];
		char filename[PATH_MAX];

		char pathextended[PATH_MAX];
		int out_n = 0;

		splitpath(somechars,pathname,filename);

		if ( strcmp(pathname,"")==0 )
		{
			/* current path */
			int ierr = 0;
			int lpath = 0;
			char *currentpath = NULL;

			C2F(scigetcwd)(&currentpath,&lpath,&ierr);
			if (currentpath) {strcpy(path,currentpath);}
		}
		else
		{
			/* we have a path */
			strcpy(path,pathname);
		}

		if ( strcmp(filename,"")==0 )
		{
			/* no filename */
			strcpy(filespec,"*");
		}
		else
		{
			/* we have the beginning of a filename */
			sprintf(filespec,"%s*",filename);
		}

		C2F(cluni0)(path,pathextended,&out_n,(long)strlen(path),PATH_MAX);
		dictionary = findfiles(pathextended,filespec,&sizeListReturned);
		*sizearray = sizeListReturned;

        /* Add a NULL element at the end (to get number of items from JNI) */
        if (dictionary != NULL)
        {
			dictionary = (char**)REALLOC(dictionary,sizeof(char*)*(sizeListReturned+1));
			dictionary[sizeListReturned] = NULL;
		}
	}
	else
	{
		*sizearray = 0;
	}
	return dictionary;
}
/*--------------------------------------------------------------------------*/ 
static void splitpath(char *composite,  char *path,  char *fname)
{
	if (composite && path && fname)
	{
		char *lastslash = NULL;
		char *p2 = NULL;

		lastslash = NULL;
		p2 = composite;

		while (*p2)  
		{
		#ifdef _MSC_VER
			if ( (*p2 == '/') || (*p2 == '\\') ) lastslash = p2;
		#else
			if (*p2 == '/') lastslash = p2;
		#endif
			p2++;
		}

		if (lastslash != NULL)  
		{
			strncpy(path, composite, 1 + (int)(lastslash - composite));
			path[1 + (int)(lastslash - composite)]='\0';
			strcpy(fname, lastslash+1);
		}
		else 
		{
			strcpy(path, "");
			strcpy(fname, composite);
		}
	}
}
/*--------------------------------------------------------------------------*/
