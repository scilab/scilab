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
#include <stdio.h>
#include "FindFileAssociation.h"
#include "MALLOC.h"
#include "strdup_windows.h"
#include "PATH_MAX.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/ 
#pragma comment(lib,"shlwapi.lib") /* AssocQueryString */
/*--------------------------------------------------------------------------*/ 
char *FindFileAssociation (char *ptrFindStr,char *Extra) 
{
	char *ptrResult = NULL ;

	if ( ptrFindStr )
	{
		wchar_t *wcptrFindStr = to_wide_string(ptrFindStr);
		wchar_t *wcExtra = to_wide_string(Extra);
		wchar_t szDefault[PATH_MAX + FILENAME_MAX];
		DWORD ccDefault = PATH_MAX + FILENAME_MAX;

		if (wcptrFindStr && wcptrFindStr)
		{
			HRESULT rc = AssocQueryStringW (0, ASSOCSTR_EXECUTABLE,wcptrFindStr, wcExtra, szDefault, &ccDefault);
			if (ccDefault)
			{
				if (rc == S_OK) ptrResult = wide_string_to_UTF8(szDefault);
			}
		}
	}
	return ptrResult;
}
/*--------------------------------------------------------------------------*/

