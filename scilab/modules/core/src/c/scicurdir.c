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
		if ( getWarningMode() ) message_scilab("core_message_142", path); 
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
	if ( GetCurrentDirectory(FSIZE,cur_dir) == 0 )
#endif
	{
		/* get current working dir */
		if ( getWarningMode() ) message_scilab("core_message_143");
		*cur_dir = '\0';
		*lpath=0;
		*err=1;
	}
    else 
	{
		char *returnPath=NULL;
		returnPath=(char*)MALLOC(sizeof(char)*(strlen(cur_dir)+1));
		strcpy(returnPath,cur_dir);

		*path= returnPath;

		*lpath=strlen(cur_dir);
		*err=0;
	}
    return 0;
}
/*-----------------------------------------------------------------------------------*/
