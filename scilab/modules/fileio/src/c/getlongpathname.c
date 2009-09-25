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
	wchar_t *wcshortpathname = to_wide_string(shortpathname);
	if (wcshortpathname)
	{
		wchar_t *wcLongName = getlongpathnameW(wcshortpathname, convertok);
		if (wcLongName)
		{
			LongName = wide_string_to_UTF8(wcLongName);
			FREE(wcLongName); wcLongName = NULL;
		}
		else
		{
			LongName = strdup(shortpathname);
			*convertok = FALSE;
		}
	}
	else
	{
		LongName = strdup(shortpathname);
		*convertok = FALSE;
	}
	return LongName;
}
/*--------------------------------------------------------------------------*/
wchar_t *getlongpathnameW(wchar_t *wcshortpathname,BOOL *convertok)
{
	wchar_t *wcLongName = NULL;

	#ifdef _MSC_VER
	/* first we try to call to know path length */
	int length = GetLongPathNameW(wcshortpathname, NULL, 0);
	if (length <= 0 ) length = MAX_PATH_LONG;

	wcLongName = (wchar_t*)MALLOC((length + 1) * sizeof(wchar_t));

	if (wcLongName)
	{
		/* second converts path */
		if (GetLongPathNameW(wcshortpathname, wcLongName, length))
		{
			*convertok = TRUE;
		}
		else
		{
			/* FAILED */
			if (wcLongName) wcscpy(wcLongName, wcshortpathname);
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
	int len = (int)wcslen(wcshortpathname) + 1;
	wcLongName = (wchar_t*)MALLOC(len * sizeof(wchar_t));
	if (wcLongName) wcscpy(wcLongName, wcshortpathname);
	*convertok = FALSE;	
	#endif
	return wcLongName;
}
/*--------------------------------------------------------------------------*/
