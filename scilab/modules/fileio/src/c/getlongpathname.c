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
