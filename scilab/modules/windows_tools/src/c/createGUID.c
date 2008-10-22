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
#include <unknwn.h>
#include "createGUID.h"
#include "strdup_Windows.h"
/*--------------------------------------------------------------------------*/
#define _OLEAUT32_
/*--------------------------------------------------------------------------*/
char *createGUID(void)
{
	GUID guid;
	WORD* wstrGUID[100];
	char strGUID[100];

    CoCreateGuid (&guid);
    StringFromCLSID (&guid, wstrGUID);
    WideCharToMultiByte (CP_ACP, 0, *wstrGUID, -1, strGUID, MAX_PATH, NULL, NULL);
    strGUID[strlen(strGUID)-1] = '\0';
    return strdup(strGUID+1);
}
/*--------------------------------------------------------------------------*/
