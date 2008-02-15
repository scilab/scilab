/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
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
#include "getshortpathname.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#ifndef MAX_PATH_SHORT
#define MAX_PATH_SHORT 260
#endif
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
