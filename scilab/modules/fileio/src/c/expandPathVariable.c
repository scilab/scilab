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
#include "expandPathVariable.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "PATH_MAX.h"
#include "tmpdir.h"
#include "setgetSCIpath.h"
#include "getenvc.h"
/*--------------------------------------------------------------------------*/
static wchar_t *SCI_words[]  = 
{
	L"SCI/" ,
	L"sci/" ,
	L"$SCI" ,
	L"SCI\\" ,
	L"sci\\" ,
	(wchar_t *) NULL
};

static wchar_t *HOME_words[] = 
{
	L"HOME/" ,
	L"home/" ,
	L"~/" ,
	L"HOME\\" ,
	L"home\\" ,
	L"~\\" ,
	L"$HOME" ,
	(wchar_t *) NULL
};

static wchar_t *TMP_words[]  = 
{
	L"TMPDIR/" ,
	L"tmpdir/" ,
	L"TMPDIR\\" ,
	L"tmpdir\\" ,
	L"$TMPDIR" ,
	(wchar_t *) NULL
};
/*--------------------------------------------------------------------------*/
static wchar_t *SCIDIR = NULL;
static wchar_t *TMPDIR = NULL;
static wchar_t *HOMEDIR = NULL;
/*--------------------------------------------------------------------------*/
static wchar_t *findAliasInString(wchar_t *wcStr, wchar_t *wcAlias);
static wchar_t *replaceAliasInString(wchar_t *wcValue, wchar_t *wcBegin);
static wchar_t *getHomeDirW(void);
static wchar_t *convertFileSeparators(wchar_t *wcStr);
/*--------------------------------------------------------------------------*/
wchar_t *expandPathVariableW(wchar_t *wcstr)
{
	int i = 0;

	wchar_t *wcexpanded = NULL;
	wchar_t *wcTmp = NULL;

	if (wcstr == NULL) return wcexpanded;
	if (SCIDIR == NULL) SCIDIR = getSCIpathW();
	if (TMPDIR == NULL) TMPDIR = getTMPDIRW();
	if (HOMEDIR == NULL) HOMEDIR = getHomeDirW();

	i = 0;
	while(SCI_words[i] != NULL)
	{
		wcTmp = findAliasInString(wcstr, SCI_words[i]);
		if (wcTmp)
		{
			return replaceAliasInString(SCIDIR, wcTmp);
		}
		i++;
	}

	i = 0;
	while(HOME_words[i] != NULL)
	{
		wcTmp = findAliasInString(wcstr, HOME_words[i]);
		if (wcTmp)
		{
			return replaceAliasInString(HOMEDIR, wcTmp);
		}
		i++;
	}

	i = 0;
	while(TMP_words[i] != NULL)
	{
		wcTmp = findAliasInString(wcstr, TMP_words[i]);
		if (wcTmp)
		{
			return replaceAliasInString(TMPDIR, wcTmp);
		}
		i++;
	}
	
	/* Variables not founded returns a copy of input */
	wcexpanded = (wchar_t*)MALLOC(sizeof(wchar_t)* ((int)wcslen(wcstr) + 1));
	wcscpy(wcexpanded, wcstr);
	return convertFileSeparators(wcexpanded);
}
/*--------------------------------------------------------------------------*/
char *expandPathVariable(char* str)
{
	char *expanded = NULL;
	wchar_t *wstr = to_wide_string(str);

	if (wstr)
	{
		wchar_t *wcexpanded = expandPathVariableW(wstr);
		if (wcexpanded)
		{
			expanded = wide_string_to_UTF8(wcexpanded);
			FREE(wcexpanded);
			wcexpanded = NULL;
		}
		FREE(wstr);
		wstr = NULL;
	}
	return expanded;
}
/*--------------------------------------------------------------------------*/
static wchar_t *findAliasInString(wchar_t *wcStr, wchar_t *wcAlias)
{
	if (wcStr && wcAlias)
	{
		int lenwcAlias = (int)wcslen(wcAlias);
		int lenwcStr = (int)wcslen(wcStr);
		if (lenwcStr >= lenwcAlias)
		{
			wchar_t *wcBegin = (wchar_t *)MALLOC(sizeof(wchar_t) * (lenwcAlias + 1));
			wcsncpy(wcBegin, wcStr, lenwcAlias);
			wcBegin[lenwcAlias] = 0;
			if (wcscmp(wcBegin, wcAlias) == 0)
			{
				FREE(wcBegin); wcBegin = NULL;
				return(&wcStr[lenwcAlias]);
			}
			FREE(wcBegin); wcBegin = NULL;
		}
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
static wchar_t *replaceAliasInString(wchar_t *wcValue, wchar_t *wcBegin)
{
	wchar_t *wcexpanded = NULL;
	int lenwcBegin = (int)wcslen(wcBegin);
	int lenwcValue = (int)wcslen(wcValue);

	wcexpanded = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenwcBegin + lenwcValue + 1 + 1));

	wcscpy(wcexpanded, wcValue);
	wcscat(wcexpanded, L"/");
	wcscat(wcexpanded, wcBegin);

	return convertFileSeparators(wcexpanded);
}
/*--------------------------------------------------------------------------*/
static wchar_t *getHomeDirW(void)
{
	int ierr = 0;
	char result[PATH_MAX];
	int lenMAX = PATH_MAX;
	int iflag = 0; /* no warnings */

	C2F(getenvc)(&ierr, "home", result, &lenMAX, &iflag);
	if (ierr == 0)
	{
		return to_wide_string(result);
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
static wchar_t *convertFileSeparators(wchar_t *wcStr)
{
	if (wcStr)
	{
		int k = 0;
		int len = (int)wcslen(wcStr);

#ifdef _MSC_VER
		for (k=0 ; k < len ;k++) if (wcStr[k] == L'/') wcStr[k] = L'\\';
#else
		for (k=0 ; k < len ;k++) if (wcStr[k] == L'\\') wcStr[k] = L'/';
#endif
	}
	return wcStr;
}
/*--------------------------------------------------------------------------*/
