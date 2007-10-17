/*-----------------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "getshortpathname.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/
#ifndef MAX_PATH_SHORT
#define MAX_PATH_SHORT 260
#endif
/*-----------------------------------------------------------------------------------*/
char *getshortpathname(char *longpathname,BOOL *convertok)
{
	char *ShortName = NULL;

	if (longpathname)
	{
		ShortName=(char*)MALLOC((strlen(longpathname)+1)*sizeof(char));

		#ifdef _MSC_VER
		if (GetShortPathName(longpathname,ShortName,(DWORD)strlen(longpathname)+1))
		{
			*convertok = TRUE;
		}
		else
		{
			/* FAILED */
			strcpy(ShortName,longpathname);
			*convertok = FALSE;
		}
		#else
		/* Linux */
		strcpy(ShortName,longpathname);
		*convertok = FALSE;
		#endif
	}
	return ShortName;
}
/*-----------------------------------------------------------------------------------*/
