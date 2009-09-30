/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/
#include "basename.h"
#include "MALLOC.h"
#include "expandPathVariable.h"
#include "splitpath.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
wchar_t *basenameW(wchar_t *wcfullfilename, BOOL bExpand)
{
	wchar_t *basename_str = NULL;
	if (wcfullfilename)
	{
		wchar_t *expandedPath = expandPathVariableW(wcfullfilename);
		if (expandedPath)
		{
			wchar_t *wcdrv = MALLOC(sizeof(wchar_t*) * ((int)wcslen(expandedPath) + 1));
			wchar_t* wcdir = MALLOC(sizeof(wchar_t*) * ((int)wcslen(expandedPath) + 1));
			wchar_t* wcname = MALLOC(sizeof(wchar_t*) * ((int)wcslen(expandedPath) + 1));
			wchar_t* wcext = MALLOC(sizeof(wchar_t*) * ((int)wcslen(expandedPath) + 1));

			splitpathW(expandedPath, bExpand, wcdrv, wcdir, wcname, wcext);

			if (wcname)
			{
				basename_str = wcname;
			}

			if (wcdrv) {FREE(wcdrv); wcdrv = NULL;}
			if (wcdir) {FREE(wcdir); wcdir = NULL;}
			if (wcext) {FREE(wcext); wcext = NULL;}

			FREE(expandedPath);
			expandedPath = NULL;
		}
	}
	return basename_str;
}
/*--------------------------------------------------------------------------*/
char *basename(char *fullfilename, BOOL bExpand)
{
	char *result = NULL;
	if (fullfilename)
	{
		wchar_t *wcresult = NULL;
		wchar_t *wcfullfilename = to_wide_string(fullfilename);

		wcresult = basenameW(wcfullfilename, bExpand);
		result = wide_string_to_UTF8(wcresult);
		if (wcfullfilename) {FREE(wcfullfilename); wcfullfilename = NULL;}
		if (wcresult) {FREE(wcresult); wcresult = NULL;}
	}
	return result;
}
/*--------------------------------------------------------------------------*/
