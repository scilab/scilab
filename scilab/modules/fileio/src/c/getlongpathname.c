/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
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
#ifdef _MSC_VER
	#ifndef MAX_PATH_LONG
		#define MAX_PATH_LONG 32767
	#endif
#endif
/*--------------------------------------------------------------------------*/
char *getlongpathname(char *shortpathname,BOOL *convertok)
{
	char *LongName = NULL;

	if (shortpathname)
	{
		#ifdef _MSC_VER
		/* first we try to call to know path length */
		int length = GetLongPathName(shortpathname, NULL, 0);
		if (length <= 0 ) length = MAX_PATH_LONG;

		LongName = (char*)MALLOC(length * sizeof(char));

		if (LongName)
		{
			/* second converts path */
			if (GetLongPathName(shortpathname, LongName, length))
			{
				*convertok = TRUE;
			}
			else
			{
				/* FAILED */
				strcpy(LongName, shortpathname);
				*convertok = FALSE;
			}
		}
		else
		{
			/* FAILED */
			*convertok = FALSE;
		}
		#else
		/* Linux */
		int length = (int)strlen(shortpathname) + 1;
		LongName = (char*)MALLOC(length * sizeof(char));
		if (LongName) strcpy(LongName, shortpathname);
		*convertok = FALSE;
		#endif
	}
	else
	{
		*convertok = FALSE;
	}
	return LongName;
}
/*--------------------------------------------------------------------------*/
