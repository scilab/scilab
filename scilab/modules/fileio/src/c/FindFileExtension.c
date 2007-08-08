/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")
#else
#include <string.h>
#include <stdio.h>
#endif
#include "FindFileExtension.h"
/*-----------------------------------------------------------------------------------*/ 
char *FindFileExtension(char *filename)
{
	char *extension = NULL;
	if (filename)
	{
		#ifndef _MSC_VER
		int lengthfilename = (int) strlen(filename);
		int  i = lengthfilename;

		while(filename[i] != '.' && i > 0) i--;

		if(i > 0)
		{
			int lengthextension = lengthfilename - i;
			extension = (char*)MALLOC(sizeof(char)*(lengthextension+1));
			if (extension) sprintf(extension,&filename[i]);
		}
		#else
		extension = PathFindExtension(filename);
		#endif
	}
	return extension;
}
/*-----------------------------------------------------------------------------------*/ 
