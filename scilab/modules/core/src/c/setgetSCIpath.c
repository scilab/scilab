/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "machine.h"
#include "setgetSCIpath.h"
#include "MALLOC.h"
#include "string.h"
/*-----------------------------------------------------------------------------------*/ 
static char SCIPATH[PATH_MAX];
/*-----------------------------------------------------------------------------------*/ 
void setSCIpath(char *path)
{
	sprintf(SCIPATH,"%s",path);
}
/*-----------------------------------------------------------------------------------*/ 
char *getSCIpath(void)
{
	char *pathtoreturn=NULL;
	pathtoreturn=(char*)MALLOC(sizeof(char)*(strlen(SCIPATH)+1));
	sprintf(pathtoreturn,"%s",SCIPATH);
	return pathtoreturn;
}
/*-----------------------------------------------------------------------------------*/ 
