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