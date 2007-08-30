/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef _MSC_VER
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#else
#include <Windows.h>
#endif
#include "createdirectory.h"
#include "isdir.h"
/*-----------------------------------------------------------------------------------*/
#define DIRMODE 0777
/*-----------------------------------------------------------------------------------*/
BOOL createdirectory(const char *path)
{
	BOOL bOK=FALSE;
#ifndef _MSC_VER
	if  (!isdir(path)) 
	{
		if (mkdir(path, DIRMODE) == 0) bOK=TRUE;
	}
#else
	if (CreateDirectory(path,NULL)) bOK=TRUE;
#endif
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
