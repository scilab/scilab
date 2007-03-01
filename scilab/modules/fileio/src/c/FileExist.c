/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <stdio.h>
#endif
#include "FileExist.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL FileExist(char *filename)
{
	BOOL retour=FALSE;	

	#ifdef _MSC_VER
		WIN32_FIND_DATA FindFileData;
		HANDLE handle = FindFirstFile (filename, &FindFileData);
		if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose (handle);
			retour=TRUE;
		}
		else retour=FALSE;
	#else
	FILE* tmpFile; 
	if( (tmpFile=fopen(filename,"r")) == FALSE ) 
	{ 
		retour=FALSE;
	} 
	else 
	{ 
		fclose(tmpFile); 
		retour=TRUE;
	} 
	#endif

	return retour;
}
/*-----------------------------------------------------------------------------------*/ 
