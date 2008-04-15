/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "machine.h"
#include "stack-c.h"
#include "libraryinfo.h"
#include "stackinfo.h"
#include "MALLOC.h"
#include "cvstr.h"
#include "cluni0.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
/*--------------------------------------------------------------------------*/
char *getlibrarypath(char *libraryname)
{
	char *path = NULL;
	int lw = 0; int fin = 0;

	if (C2F(objptr)(libraryname,&lw,&fin,(unsigned long)strlen(libraryname))) 
	{
		int *header = istk(iadr(*Lstk(fin)));  
		if ( (header) && (header[0] == sci_lib ) ) 
		{
			int lengthpath = 0, job = 0;

			lengthpath = header[1];
			path = (char *) MALLOC((lengthpath+1)*sizeof(char));

			job=1; /* convert scilab to ascii */
			C2F(cvstr)(&lengthpath, &header[2], path,&job,lengthpath);
			path[lengthpath]='\0';
		}
	}
	return path;
}
/*--------------------------------------------------------------------------*/
char **getlistmacrosfromlibrary(char *libraryname,int *sizearray)
{
	char **macroslist = NULL;
	char *pathlibrary = getlibrarypath(libraryname);
	
	if (pathlibrary)
	{
		/* in each library directory , we have a "names" file with list of macros */
		#define filenameNAMES "names"
		char expandedpath[PATH_MAX+1];
		char *fullfilename = NULL;
		int out_n = 0;

		C2F(cluni0)(pathlibrary,expandedpath, &out_n,(long)strlen(pathlibrary),PATH_MAX);

		fullfilename = (char*)MALLOC(sizeof(char)*(strlen(expandedpath)+strlen(filenameNAMES)+1));
		if (fullfilename)
		{
			char  line[PATH_MAX+1];
			FILE * pFile = NULL;
			int nbElements = 0;

			sprintf(fullfilename,"%s%s",expandedpath,filenameNAMES);

			pFile = fopen (fullfilename,"rt");
			if (pFile)
			{
				while(fgets (line,sizeof(line),pFile) != NULL)
				{
					line[strlen(line)-1]='\0'; /* remove carriage return */
					
					if (macroslist) macroslist = (char**)REALLOC(macroslist,sizeof(char*)*(nbElements+1));
					else macroslist =(char**)MALLOC(sizeof(char*)*(nbElements+1));

					macroslist[nbElements] = strdup(line);
					nbElements++;
				}
				fclose(pFile);
				*sizearray = nbElements;
			}
			FREE(fullfilename);
			fullfilename = NULL;
		}
		FREE(pathlibrary);
		pathlibrary = NULL;
	}
	else
	{
		*sizearray = 0;
	}
	return macroslist;
}
/*--------------------------------------------------------------------------*/
