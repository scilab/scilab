/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h> /* strcmp */
#include <stdio.h> /* sprintf */
#include "getfilesdictionary.h"
#include "../../../core/src/c/scicurdir.h" /* C2F(scigetcwd) */
#include "findfiles.h" /* findfiles */
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
static void splitpath(char *composite,  char *path,  char *fname);
/*-----------------------------------------------------------------------------------*/ 
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
			/* we have the begining of a filename */
			sprintf(filespec,"%s*",filename);
		}

		dictionary = findfiles(path,filespec,&sizeListReturned);
		*sizearray = sizeListReturned;
	}
	else
	{
		*sizearray = 0;
	}
	return dictionary;
}
/*-----------------------------------------------------------------------------------*/ 
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
/*-----------------------------------------------------------------------------------*/
