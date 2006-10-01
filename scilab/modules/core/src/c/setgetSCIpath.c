/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "setgetSCIpath.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#define PATH_MAX 1024
/*-----------------------------------------------------------------------------------*/ 
static char SCIPATH[PATH_MAX];
/*-----------------------------------------------------------------------------------*/ 
void setSCIpath(char *path)
{
	printf("Setscipath to %s\n", path);
	sprintf(SCIPATH,"%s",path);
}
/*-----------------------------------------------------------------------------------*/ 
char *getSCIpath(void)
{
	char *pathtoreturn=NULL;
	pathtoreturn=(char*)MALLOC(sizeof(char)*(strlen(SCIPATH)+1));
	printf("getscipath to %s\n", pathtoreturn);
	sprintf(pathtoreturn,"%s",SCIPATH);
	return pathtoreturn;
}
/*-----------------------------------------------------------------------------------*/ 
