/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#include <Windows.h>
#else
#endif
#include "findfiles.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
char **findfiles(char *path,char *filespec,int *sizeListReturned)
{
	char **ListFiles=NULL;
	*sizeListReturned=0;

	return ListFiles;
}
#else
/*-----------------------------------------------------------------------------------*/ 
char **findfiles(char *path,char *filespec,int *sizeListReturned)
{
	char **ListFiles=NULL;
	*sizeListReturned=0;

	return ListFiles;
}
#endif
/*-----------------------------------------------------------------------------------*/ 
