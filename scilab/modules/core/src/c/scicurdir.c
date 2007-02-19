/*-----------------------------------------------------------------------------------*/
/* Copyright INRIA/ENPC */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "machine.h"
#ifdef _MSC_VER 
	#include <windows.h>
#else 
	#include <unistd.h>
	#define GETCWD(x,y) getcwd(x,y)
#endif 
#include "sciprint.h"
#include "scicurdir.h"
#define FSIZE 1024
static char cur_dir[FSIZE];
/*-----------------------------------------------------------------------------------*/
int C2F(scichdir)(char *path,int *err)
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
		sciprint("Can't go to directory %s \r\n", path); 
	    *err=1;
	} 
#else
	if (SetCurrentDirectory(path) == 0)
	{
		sciprint("Can't go to directory %s \r\n", path); 
		*err=1;
	}
	{

		char cdir[FSIZE];
		GetCurrentDirectory(FSIZE,cdir);
		MessageBox(NULL,cdir," ",MB_OK);
	}

#endif
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(scigetcwd)(char **path,int *lpath,int *err)
{
#ifndef _MSC_VER
	if (GETCWD(cur_dir, FSIZE) == (char*) 0)
#else
	if ( GetCurrentDirectory(FSIZE,cur_dir) == 0 )
#endif
	{
		/* get current working dir */
		sciprint("Can't get current directory\r\n");
		*cur_dir = '\0';
		*lpath=0;
		*err=1;
	}
    else 
	{
		*path= cur_dir;
		*lpath=strlen(cur_dir);
		*err=0;
	}
    return 0;
}
/*-----------------------------------------------------------------------------------*/
