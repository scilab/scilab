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
#ifdef _MSC_VER
	#ifndef MAX_PATH_SHORT
		#define MAX_PATH_SHORT 260
	#endif
#endif
/*--------------------------------------------------------------------------*/
char *getshortpathname(char *longpathname,BOOL *convertok)
{
	char *ShortName = NULL;

	if (longpathname)
	{
		#ifdef _MSC_VER
		/* first we try to call to know path length */
		int length = GetShortPathName(longpathname, NULL, 0);

		if (length <= 0 ) length = MAX_PATH_SHORT;

		ShortName = (char*)MALLOC((length)*sizeof(char));

		if (ShortName) 
		{
			/* second converts path */
			if ( GetShortPathName(longpathname, ShortName, length) )
			{
				*convertok = TRUE;
			}
			else
			{
				/* FAILED */
				strcpy(ShortName, longpathname);
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
		int length = (int)strlen(longpathname) + 1;
		ShortName = (char*)MALLOC((length) * sizeof(char));
		if (ShortName) strcpy(ShortName, longpathname);
		*convertok = FALSE;
		#endif
	}
	else
	{
		/* FAILED */
		*convertok = FALSE;
	}
	return ShortName;
}
/*--------------------------------------------------------------------------*/
