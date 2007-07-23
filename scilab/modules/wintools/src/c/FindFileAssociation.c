/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <shlwapi.h>
#include "FindFileAssociation.h"
#include "MALLOC.h"
#include "machine.h"
/*-----------------------------------------------------------------------------------*/ 
char *FindFileAssociation (char *ptrFindStr,char *Extra) 
{
	char *ptrResult = NULL ;

	if ( ptrFindStr )
	{
		char szDefault[PATH_MAX];
		DWORD ccDefault = PATH_MAX;
		HRESULT rc = AssocQueryString (0, ASSOCSTR_EXECUTABLE,ptrFindStr, Extra, szDefault, &ccDefault);
		if (ccDefault)
		{
			if (rc==S_OK)
			{
				ptrResult=(char*)MALLOC(sizeof(char)*(strlen(szDefault)+1));
				strcpy(ptrResult,szDefault);
			}
		}
	}
	return ptrResult;

}
/*-----------------------------------------------------------------------------------*/

