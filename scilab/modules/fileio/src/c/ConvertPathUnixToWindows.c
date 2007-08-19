/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "ConvertPathUnixToWindows.h"
/*-----------------------------------------------------------------------------------*/
#define UNIX_SEPATATOR '/'
#define WINDOWS_SEPATATOR '\\'
/*-----------------------------------------------------------------------------------*/
BOOL ConvertPathUnixToWindowsFormat(char *pathunix,char *pathwindows)
{
	BOOL bOK=TRUE;
	if ( (pathunix) && (pathwindows) )
	{
		int i=0;
		strcpy(pathwindows,pathunix);
		for (i=0;i<(int)strlen(pathwindows);i++)
		{
			if (pathwindows[i] == UNIX_SEPATATOR) pathwindows[i] = WINDOWS_SEPATATOR;
		}
	}
	else bOK=FALSE;

	return bOK;

}
/*-----------------------------------------------------------------------------------*/
