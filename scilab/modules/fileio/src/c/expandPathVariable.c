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
#include "stack-c.h"
#include "expandPathVariable.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "PATH_MAX.h"
#include "api_string.h"
#include "api_common.h"
/*--------------------------------------------------------------------------*/
static wchar_t *SCI_words[]  = 
{
	L"SCI/" ,
	L"SCI\\" ,
	(wchar_t *) NULL
};
/*--------------------------------------------------------------------------*/
static wchar_t *HOME_words[] = 
{
	L"HOME/" ,
	L"home/" ,
	L"~/" ,
	L"HOME\\" ,
	L"home\\" ,
	L"~\\" ,
	(wchar_t *) NULL
};
/*--------------------------------------------------------------------------*/
static wchar_t *TMPDIR_words[]  = 
{
	L"TMPDIR/" ,
	L"TMPDIR\\" ,
	(wchar_t *) NULL
};
/*--------------------------------------------------------------------------*/
static wchar_t *SCIHOME_words[]  = 
{
	L"SCIHOME/" ,
	L"SCIHOME\\",
	(wchar_t *) NULL
};
/*--------------------------------------------------------------------------*/
static wchar_t *getVariableValueDefinedInScilab(char *varname);
static wchar_t *findAliasInString(wchar_t *wcStr, wchar_t *wcAlias);
static wchar_t *replaceAliasInString(wchar_t *wcValue, wchar_t *wcBegin);
static wchar_t *convertFileSeparators(wchar_t *wcStr);
/*--------------------------------------------------------------------------*/
#define expandPath(PathStr, VariableNames, VariableValue) \
i = 0; \
while(VariableNames[i] != NULL) \
{ \
	wcTmp = findAliasInString(PathStr, VariableNames[i]); \
	if (wcTmp) \
	{ \
		if (VariableValue) \
		{ \
			wchar_t * replaced = replaceAliasInString(VariableValue, wcTmp); \
			if (SCIDIR) {FREE(SCIDIR); SCIDIR = NULL;} \
			if (TMPDIR) {FREE(TMPDIR); TMPDIR = NULL;} \
			if (HOMEDIR) {FREE(HOMEDIR); HOMEDIR = NULL;} \
			if (SCIHOMEDIR) {FREE(SCIHOMEDIR); SCIHOMEDIR = NULL;} \
			return replaced; \
		} \
	} \
	i++; \
} \
/*--------------------------------------------------------------------------*/
wchar_t *expandPathVariableW(wchar_t *wcstr)
{
	int i = 0;

	wchar_t *wcexpanded = NULL;
	wchar_t *wcTmp = NULL;
	
	wchar_t *SCIDIR = NULL;
	wchar_t *HOMEDIR = NULL;
	wchar_t *SCIHOMEDIR = NULL;
	wchar_t *TMPDIR = NULL;

	/* search SCIHOME Alias and expand */
	SCIHOMEDIR = getVariableValueDefinedInScilab("SCIHOME");
	expandPath(wcstr, SCIHOME_words, SCIHOMEDIR)

	/* search SCI Alias and expand */
	SCIDIR = getVariableValueDefinedInScilab("SCI");
	expandPath(wcstr, SCI_words, SCIDIR)

	/* search HOME Alias and expand */
	HOMEDIR = getVariableValueDefinedInScilab("home");
	expandPath(wcstr, HOME_words, HOMEDIR)

	/* search TMPDIR Alias and expand */
	TMPDIR = getVariableValueDefinedInScilab("TMPDIR");
	expandPath(wcstr, TMPDIR_words, TMPDIR)

	if (SCIDIR) {FREE(SCIDIR); SCIDIR = NULL;}
	if (TMPDIR) {FREE(TMPDIR); TMPDIR = NULL;}
	if (HOMEDIR) {FREE(HOMEDIR); HOMEDIR = NULL;}
	if (SCIHOMEDIR) {FREE(SCIHOMEDIR); SCIHOMEDIR = NULL;}
	
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
wchar_t *getVariableValueDefinedInScilab(char *varname)
{
	wchar_t *VARVALUE = NULL;
	if (getNamedVarType(varname) == sci_strings)
	{
		int VARVALUElen = 0;
		int m = 0, n = 0;
		if (readNamedMatrixOfWideString(varname, &m, &n, &VARVALUElen, &VARVALUE) == 0)
		{
			if ( (m == 1) && (n == 1) )
			{
				VARVALUE = (wchar_t*)MALLOC(sizeof(wchar_t)*(VARVALUElen + 1));
				if (VARVALUE)
				{
					readNamedMatrixOfWideString(varname, &m, &n, &VARVALUElen, &VARVALUE);
				}
			}
		}
	}
	return VARVALUE;
}
/*--------------------------------------------------------------------------*/
