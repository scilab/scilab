/*--------------------------------------------------------------------------*/
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
#include "api_scilab.h"
#include "getlongpathname.h"
/*--------------------------------------------------------------------------*/
struct VARIABLEALIAS
{
	wchar_t *Alias;
	wchar_t *VariableName;
};
/*--------------------------------------------------------------------------*/
#define NB_ALIAS 7
static struct VARIABLEALIAS VARIABLES_words[NB_ALIAS] =
{
	{L"SCIHOME", L"SCIHOME"},
	{L"WSCI", L"WSCI"},
	{L"SCI", L"SCI"},
	{L"~", L"home"},
	{L"HOME", L"home"},
	{L"home", L"home"},
	{L"TMPDIR", L"TMPDIR"}
};
/*--------------------------------------------------------------------------*/
static wchar_t *getVariableValueDefinedInScilab(wchar_t *wcVarName);
static wchar_t *convertFileSeparators(wchar_t *wcStr);
/*--------------------------------------------------------------------------*/
wchar_t *expandPathVariableW(wchar_t *wcstr)
{
	wchar_t *wcexpanded = NULL;
	if (wcstr)
	{
		int i = 0;
		int lenStr = (int)wcslen(wcstr);

		for (i = 0; i < NB_ALIAS; i++)
		{
			int lenAlias = 0;

			/* input is ALIAS without subdirectory */
			if (wcscmp(VARIABLES_words[i].Alias, wcstr) == 0)
			{
				wchar_t *wcexpanded = getVariableValueDefinedInScilab(VARIABLES_words[i].VariableName);
				if (wcexpanded)
				{
					 return convertFileSeparators(wcexpanded);
				}
			}

			lenAlias = (int)wcslen(VARIABLES_words[i].Alias);

			if (lenStr > lenAlias)
			{
				wchar_t *wcBegin = (wchar_t *)MALLOC(sizeof(wchar_t) * (lenAlias + 1));
				if (wcBegin)
				{
					wcsncpy(wcBegin, wcstr, lenAlias);
					wcBegin[lenAlias] = 0;

					if (wcscmp(wcBegin,VARIABLES_words[i].Alias)== 0 )
					{
						if ( (wcstr[lenAlias] == L'/') || (wcstr[lenAlias] == L'\\') )
						{
							wchar_t * newBegin = getVariableValueDefinedInScilab(VARIABLES_words[i].VariableName);
							if (newBegin)
							{
								int lengthnewBegin = (int)wcslen(newBegin);
								wcexpanded = (wchar_t *)MALLOC(sizeof(wchar_t)* (lengthnewBegin + (int)wcslen(&wcstr[lenAlias]) + 1));
								if (wcexpanded)
								{
									wcscpy(wcexpanded, newBegin);
									wcscat(wcexpanded, &wcstr[lenAlias]);
									FREE(wcBegin); wcBegin = NULL;
									FREE(newBegin); newBegin = NULL;
									return convertFileSeparators(wcexpanded);
								}
								FREE(newBegin); newBegin = NULL;
							}
						}
					}
					FREE(wcBegin);wcBegin = NULL;
				}
			}
		}

		/* Variables not founded returns a copy of input */
		wcexpanded = (wchar_t*)MALLOC(sizeof(wchar_t)* ((int)wcslen(wcstr) + 1));
		if (wcexpanded) 
		{
			wcscpy(wcexpanded, wcstr);
			return convertFileSeparators(wcexpanded);
		}
	}
	return wcexpanded;
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
wchar_t *getVariableValueDefinedInScilab(wchar_t *wcVarName)
{
	wchar_t *VARVALUE = NULL;
	char *varname = NULL;
	int iType	= 0;

	if (wcVarName)
	{
		varname = wide_string_to_UTF8(wcVarName);
		if (varname)
		{
			StrErr strErr = getNamedVarType(pvApiCtx, varname, &iType);
			if(strErr.iErr)
			{
				return NULL;
			}

			if (iType == sci_strings)
			{
				
				int VARVALUElen = 0;
				int m = 0, n = 0;

				strErr = readNamedMatrixOfWideString(pvApiCtx, varname, &m, &n, &VARVALUElen, &VARVALUE);
				if(strErr.iErr)
				{
					return NULL;
				}

				if ( (m == 1) && (n == 1) )
				{
					VARVALUE = (wchar_t*)MALLOC(sizeof(wchar_t)*(VARVALUElen + 1));
					if (VARVALUE)
					{
						BOOL bConvLong = FALSE;
						wchar_t *LongName = NULL;
						strErr = readNamedMatrixOfWideString(pvApiCtx, varname, &m, &n, &VARVALUElen, &VARVALUE);
						if(strErr.iErr)
						{
							FREE(VARVALUE);
							VARVALUE = NULL;
							return 0;
						}
						LongName = getlongpathnameW(VARVALUE, &bConvLong);
						if (LongName)
						{
							FREE(VARVALUE);
							VARVALUE = LongName;
						}
					}
				}
			}
		}
	}
	return VARVALUE;
}
/*--------------------------------------------------------------------------*/
wchar_t *convertFileSeparators(wchar_t *wcStr)
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
