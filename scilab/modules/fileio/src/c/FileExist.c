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

#ifdef _MSC_VER
	WIN32_FIND_DATA FindFileData;
	HANDLE handle = FindFirstFile (filename, &FindFileData);
	if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose (handle);
			return TRUE;
		}
	else return FALSE;
#else
	FILE* tmpFile=fopen(filename,"r");
	if(tmpFile) 
		{ 
			fclose(tmpFile); 
			return TRUE;
		} 
	else 
		{ 
			return FALSE;
		} 
#endif

}
/*-----------------------------------------------------------------------------------*/ 
