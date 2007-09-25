/*-----------------------------------------------------------------------------------*/
/* Copyright INRIA/ENPC */
/*-----------------------------------------------------------------------------------*/
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
#include "message_scilab.h"
#include "MALLOC.h"
#include "warningmode.h"
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
#else
	if (SetCurrentDirectory(path) == 0)
#endif
	{
		if ( getWarningMode() ) message_scilab("Can't go to directory %s .", path); 
	    *err=1;
	} 
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int C2F(scigetcwd)(char **path,int *lpath,int *err)
{
#ifndef _MSC_VER
	if (GETCWD(cur_dir, FSIZE) == (char*) 0)
#else
	if ( _getcwd(cur_dir, FSIZE) == (char*) 0 )
#endif
	{
		/* get current working dir */
		if ( getWarningMode() ) message_scilab("Can't get current directory.");
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
/*-----------------------------------------------------------------------------------*/
