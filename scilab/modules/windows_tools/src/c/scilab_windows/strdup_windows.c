/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "MALLOC.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
/* strdup is deprecated on windows*/
/* required to fix warnings about strdup */
/*--------------------------------------------------------------------------*/
char *strdup_windows(const char *strSource)
{
	char *retStr = NULL;
	if (strSource)
	{
		retStr = (char *)MALLOC(sizeof(char)*((int)strlen(strSource)+1));
		if (retStr) strcpy(retStr,strSource);
	}
	return retStr;
}
/*--------------------------------------------------------------------------*/