/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "deleteafile.h"
#ifndef _MSC_VER
	#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#else
	#include <Windows.h>
#endif
/*-----------------------------------------------------------------------------------*/
BOOL deleteafile(char *filename)
{
	BOOL bOK = FALSE;
	#ifndef _MSC_VER
	{
		FILE *f = fopen(filename, "r") ;
		if (! f) return bOK;
		fclose(f) ;
		chmod(filename, S_IWRITE) ;
		if (remove(filename) == 0)  bOK = TRUE;
	}
	#else
	{
		bOK = DeleteFile(filename);
	}
	#endif
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
