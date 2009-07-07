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
#include "charEncoding.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
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
		wchar_t *wLongName = NULL;
		wchar_t *wshortpathname = to_wide_string(shortpathname);
		int length = GetLongPathNameW(wshortpathname, NULL, 0);
		if (length <= 0 ) length = MAX_PATH_LONG;

		wLongName = (wchar_t*)MALLOC((length + 1) * sizeof(wchar_t));

		if (wLongName)
		{
			/* second converts path */
			if (GetLongPathNameW(wshortpathname, wLongName, length))
			{
				LongName = wide_string_to_UTF8(wLongName);
				*convertok = TRUE;
			}
			else
			{
				/* FAILED */
				LongName = strdup(shortpathname);
				*convertok = FALSE;
			}
			if (wLongName) {FREE(wLongName);wLongName = NULL;}
		}
		else
		{
			/* FAILED */
			LongName = strdup(shortpathname);
			*convertok = FALSE;
		}
		if (wshortpathname) { FREE(wshortpathname); wshortpathname = NULL;}
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
