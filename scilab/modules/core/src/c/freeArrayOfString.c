/*-----------------------------------------------------------------------------------*/ 
/* Allan CORNET */
/* INRIA 2007 */
/*-----------------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include "freeArrayOfString.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
BOOL freeArrayOfString(char **Str,int dim)
{
	BOOL bRet = TRUE;

	if (Str)
	{
		int i = 0;
		for (i = 0;i < dim; i++)
		{
			if (Str[i])
			{
				FREE(Str[i]);
				Str[i] = NULL;
			}
			else bRet = FALSE;
		}
		FREE(Str);
		Str = NULL;
	}
	else
	{
		bRet = FALSE;
	}
	return bRet;
}
/*-----------------------------------------------------------------------------------*/ 
