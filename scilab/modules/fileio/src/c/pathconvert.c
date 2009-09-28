/*--------------------------------------------------------------------------*/ 
/*
* ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2009 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
/*--------------------------------------------------------------------------*/ 
#include "pathconvert.h"
#include "MALLOC.h"
#include "expandPathVariable.h"
#include "splitpath.h"
#include "charEncoding.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/ 
#define CYGWINSTART L"/cygdrive/"
/*--------------------------------------------------------------------------*/ 
static wchar_t *cygwintowindowspath(wchar_t *cygwinpath, BOOL *bConverted);
static wchar_t *windowstocygwinpath(wchar_t *windowspath, BOOL *bConverted);
/*--------------------------------------------------------------------------*/ 
wchar_t *pathconvertW(wchar_t* wcpath, BOOL flagtrail, BOOL flagexpand, PathConvertType PType)
{
	wchar_t *convertedPath = NULL;
	if (wcpath)
	{
		BOOL bOK = FALSE;
		BOOL bConvCyg = FALSE;
		wchar_t *tmppath = NULL;
		PathConvertType PTypelocal = PType;
		int i = 0;

		if (PType == AUTO_STYLE)
		{
			#ifdef _MSC_VER
				PTypelocal = WINDOWS_STYLE;
			#else
				PTypelocal = UNIX_STYLE;
			#endif
		}

		if (PTypelocal == WINDOWS_STYLE)
		{
			tmppath = cygwintowindowspath(wcpath, &bConvCyg);
		}
		else
		{
			tmppath = windowstocygwinpath(wcpath, &bConvCyg);
		}

		if (tmppath)
		{
			if (flagexpand)
			{
				convertedPath = expandPathVariableW(tmppath);
			}
			else
			{
				convertedPath = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(tmppath) + 1));
				wcscpy(convertedPath, tmppath);
			}
			if (tmppath) {FREE(tmppath); tmppath = NULL;}

			if (flagtrail)
			{
				int currentLen = (int) wcslen(convertedPath);
				if ( (convertedPath[currentLen - 1] != L'/') && (convertedPath[currentLen - 1] != L'\\') )
				{
					convertedPath = (wchar_t*)REALLOC(convertedPath, (currentLen + 2) * sizeof(wchar_t));
					if (PTypelocal == WINDOWS_STYLE)
					{
						wcscat(convertedPath, L"\\");
					}
					else
					{
						wcscat(convertedPath, L"/");
					}
				}
			}
			else
			{
				int currentLen = (int) wcslen(convertedPath);
				if ( (convertedPath[currentLen - 1] == L'/') || (convertedPath[currentLen - 1] == L'\\') )
				{
					convertedPath[currentLen - 1] = L'\0';
				}
			}

			for(i = 0; i < (int)wcslen(convertedPath); i++)
			{
				if (PTypelocal == WINDOWS_STYLE)
				{
					if (convertedPath[i] == L'/') convertedPath[i] = L'\\';
				}
				else
				{
					if (convertedPath[i] == L'\\') convertedPath[i] = L'/';
				}
			}
		}
	}
	return convertedPath;
}
/*--------------------------------------------------------------------------*/ 
char *pathconvert(char* path, BOOL flagtrail, BOOL flagexpand, PathConvertType PType)
{
	char *convertedPath = NULL;
	if (path)
	{
		wchar_t *wcpath = to_wide_string(path);
		if (wcpath)
		{
			wchar_t *wcconvertedPath = pathconvertW(wcpath,flagtrail,flagexpand,PType);
			if (wcconvertedPath)
			{
				convertedPath = wide_string_to_UTF8(wcconvertedPath);
				FREE(wcconvertedPath);
				wcconvertedPath = NULL;
			}
		}
	}
	return convertedPath;
}
/*--------------------------------------------------------------------------*/ 
static wchar_t *cygwintowindowspath(wchar_t *cygwinpath, BOOL *bConverted)
{
	wchar_t *windowspath = NULL;
	*bConverted	= FALSE;
	if (cygwinpath)
	{
		int lenBegin = (int)wcslen(CYGWINSTART);
		int lenPath = (int)wcslen(cygwinpath);

		windowspath = (wchar_t*)MALLOC(sizeof(wchar_t) * (lenPath + 1));

		if ( wcsncmp(cygwinpath, CYGWINSTART, lenBegin) == 0)
		{
			if (lenBegin == lenPath)
			{
				if (windowspath)
				{
					wcscpy(windowspath, cygwinpath);
					*bConverted	= FALSE;
				}
			}
			else
			{
				if ( (lenPath > lenBegin) && iswalpha(cygwinpath[lenBegin]) )
				{
					if ( (lenPath >= lenBegin + 1) && 
						((cygwinpath[lenBegin + 1]== L'/') || (cygwinpath[lenBegin + 1]== L'\\')) )
					{
						windowspath[0] = cygwinpath[lenBegin];
						windowspath[1] = L':';
						windowspath[2] = L'\0';

						if (lenPath > lenBegin + 1)
						{
							wcscat(windowspath,&cygwinpath[lenBegin + 1]);
						}
						*bConverted	= TRUE;
					}
					else
					{
						if (windowspath)
						{
							wcscpy(windowspath, cygwinpath);
						}
						*bConverted	= FALSE;
					}
				}
				else
				{
					if (windowspath)
					{
						wcscpy(windowspath, cygwinpath);
					}
					*bConverted	= FALSE;
				}
			}
		}
		else
		{
			if (windowspath)
			{
				wcscpy(windowspath, cygwinpath);
			}
			*bConverted	= FALSE;
		}
	}
	return windowspath;
}
/*--------------------------------------------------------------------------*/ 
static wchar_t *windowstocygwinpath(wchar_t *windowspath, BOOL *bConverted)
{
	wchar_t *cygwinpath = NULL;
	*bConverted = FALSE;

	if (windowspath)
	{
		wchar_t *wcdrv = MALLOC(sizeof(wchar_t*) * ((int)wcslen(windowspath) + 1));
		wchar_t* wcdir = MALLOC(sizeof(wchar_t*) * ((int)wcslen(windowspath) + 1));
		wchar_t* wcname = MALLOC(sizeof(wchar_t*) * ((int)wcslen(windowspath) + 1));
		wchar_t* wcext = MALLOC(sizeof(wchar_t*) * ((int)wcslen(windowspath) + 1));

		splitpathW(windowspath, FALSE, wcdrv, wcdir, wcname, wcext);
		if (wcscmp(wcdrv, L"") != 0)
		{
			int len = (int)wcslen(CYGWINSTART) + (int)wcslen(wcdrv) +
					(int)wcslen(wcdir) + (int)wcslen(wcname) + 
					(int)wcslen(wcext) + 3;

			cygwinpath = (wchar_t*)MALLOC(sizeof(wchar_t) * len);
			if (cygwinpath)
			{
				wcscpy(cygwinpath, CYGWINSTART);
				wcscat(cygwinpath, wcdrv);
				len = (int)wcslen(cygwinpath);
				if (cygwinpath[len - 1] == L':')
				{
					cygwinpath[len - 1] = L'\0';
				}
				if (wcscmp(wcdir, L"") != 0)	
				{
					wcscat(cygwinpath, wcdir);
					
					if (wcscmp(wcname, L"") != 0)	
					{
						wcscat(cygwinpath, wcname);
						if (wcscmp(wcext, L"") != 0)	
						{
							wcscat(cygwinpath, wcext);
						}
					}
				}
			}
			*bConverted = TRUE;
		}
		else
		{
			cygwinpath = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(windowspath) + 1));
			if (cygwinpath)
			{
				wcscpy(cygwinpath, windowspath);
			}
		}

		if (wcdrv){FREE(wcdrv);wcdrv = NULL;}
		if (wcdir){FREE(wcdir);wcdir = NULL;}
		if (wcname){FREE(wcname);wcname = NULL;}
		if (wcext){FREE(wcext);wcext =  NULL;}
	}
	return cygwinpath;
}
/*--------------------------------------------------------------------------*/ 
