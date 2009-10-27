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
#include "getFullFilename.h"
#include "charEncoding.h"
#include "MALLOC.h"
#include "splitpath.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
wchar_t *getFullFilenameW(wchar_t* FilenameInput)
{
	wchar_t *pStwcFullFilename = NULL;

	pStwcFullFilename = (wchar_t *)MALLOC(sizeof(wchar_t) * (PATH_MAX * 2));
	if (pStwcFullFilename)
	{
		int i = 0;
		int lenPath = 0;

		wchar_t wcDrv[PATH_MAX * 2];
		wchar_t wcDir[PATH_MAX * 2];
		wchar_t wcName[PATH_MAX * 2];
		wchar_t wcExt[PATH_MAX * 2];

		wchar_t wcNameExt[PATH_MAX * 2];
		wchar_t wcPath[PATH_MAX * 2];

		splitpathW(FilenameInput, TRUE, wcDrv, wcDir,  wcName, wcExt);

		wcscpy(wcNameExt, wcName);
		wcscat(wcNameExt, wcExt);

		wcscpy(wcPath, wcDrv);
		wcscat(wcPath, wcDir);

		if (wcscmp(wcPath, L"") == 0)
		{
#ifdef _MSC_VER
			wchar_t wcCurrentDir[PATH_MAX];
			if ( _wgetcwd(wcCurrentDir, PATH_MAX) != NULL)
			{
				wcscpy(wcPath, wcCurrentDir);
			}
#else
			char CurrentDir[PATH_MAX];
			if (getcwd(CurrentDir, PATH_MAX) != NULL)
			{
				wchar_t *wcCurrentDir = to_wide_string(CurrentDir);
				wcscpy(wcPath, wcCurrentDir);
				FREE(wcCurrentDir);
			}
#endif
		}
		lenPath = (int)wcslen(wcPath);
		if (lenPath - 1 >= 0)
		{
			if ( (wcPath[lenPath - 1 ] != L'/') && (wcPath[lenPath - 1 ] != L'\\') )
			{
				wcscat(wcPath, L"/");
				lenPath = (int)wcslen(wcPath);
			}
		}

		for( i = 0; i < lenPath; i++)
		{
			#ifdef _MSC_VER
			if (wcPath[i] == L'/') wcPath[i] = L'\\';
			#else
			if (wcPath[i] == L'\\') wcPath[i] = L'/';
			#endif
		}

		wcscpy(pStwcFullFilename, wcPath);
		wcscat(pStwcFullFilename, wcNameExt);
	}
	return pStwcFullFilename;

}
/*--------------------------------------------------------------------------*/
char *getFullFilename(char* Filename)
{
	char *pStFullFilename = NULL;

	if (Filename)
	{
		wchar_t *wcFilename = to_wide_string(Filename);
		if (wcFilename)
		{
			wchar_t *pStwcFullFilename = NULL;
			pStwcFullFilename = getFullFilenameW(wcFilename);
			FREE(wcFilename);
			wcFilename = NULL;
			if (pStwcFullFilename)
			{
				pStFullFilename = wide_string_to_UTF8(pStwcFullFilename);
				FREE(pStwcFullFilename);
				pStwcFullFilename = NULL;
			}
		}
	}
	return pStFullFilename;
}
/*--------------------------------------------------------------------------*/
