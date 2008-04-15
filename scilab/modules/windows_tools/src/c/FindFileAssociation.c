/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#include <shlwapi.h>
#include "FindFileAssociation.h"
#include "MALLOC.h"
#include "machine.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/ 
#pragma comment(lib,"shlwapi.lib") /* AssocQueryString */
/*--------------------------------------------------------------------------*/ 
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
			if (rc == S_OK) ptrResult = strdup(szDefault);
		}
	}
	return ptrResult;
}
/*--------------------------------------------------------------------------*/

