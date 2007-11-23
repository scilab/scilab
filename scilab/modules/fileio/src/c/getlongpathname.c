/*--------------------------------------------------------------------------*/
/* Allan CORNET */
/* INRIA 2007 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "getlongpathname.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifndef MAX_PATH_LONG
#define MAX_PATH_LONG 32767
#endif
/*--------------------------------------------------------------------------*/
char *getlongpathname(char *shortpathname,BOOL *convertok)
{
	char *LongName = NULL;

	if (shortpathname)
	{
		LongName=(char*)MALLOC(MAX_PATH_LONG*sizeof(char));

		#ifdef _MSC_VER
		if (GetLongPathName(shortpathname,LongName,MAX_PATH_LONG))
		{
			*convertok = TRUE;
		}
		else
		{
			/* FAILED */
			strcpy(LongName,shortpathname);
			*convertok = FALSE;
		}
		#else
		/* Linux */
		strcpy(LongName,shortpathname);
		*convertok = FALSE;
		#endif
	}

	return LongName;
}
/*--------------------------------------------------------------------------*/
