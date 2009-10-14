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
#include "splitpath.h"
#include "charEncoding.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "expandPathVariable.h"
/*--------------------------------------------------------------------------*/ 
void splitpathW(const wchar_t* path, BOOL bExpand, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext)
{
	wchar_t* duplicate_path = NULL;
	wchar_t* begin_duplicate_path = NULL;
	const wchar_t* lastslash = NULL;
	const wchar_t* lastdot = NULL;
	const wchar_t* begin = NULL;

	if (drv) wcscpy(drv,L"");
	if (dir) wcscpy(dir,L"");
	if (name) wcscpy(name,L"");
	if (ext) wcscpy(ext,L"");

	/* pathconvert hardcoded */
	if (path)
	{
		int i = 0;

		if (bExpand)
		{
			duplicate_path = expandPathVariableW((wchar_t*)path);
		}
		else
		{
			duplicate_path = (wchar_t*)MALLOC(sizeof(wchar_t) * ((int)wcslen(path) + 1));
			if (duplicate_path) wcscpy(duplicate_path, path);
		}
		
		if (duplicate_path == NULL) return;

		begin_duplicate_path = duplicate_path;
		
		for(i = 0; i < (int)wcslen(duplicate_path); i++)
		{
#ifdef _MSC_VER
			if (duplicate_path[i] == L'/') duplicate_path[i] = L'\\';
#else
			if (duplicate_path[i] == L'\\') duplicate_path[i] = L'/';
#endif
		}
	}
	else
	{
		return;
	}

	if (duplicate_path)
	{
		if (wcslen(duplicate_path) > 2)
		{
			if (duplicate_path[0] && duplicate_path[1]==L':' && ( (duplicate_path[2]==L'\\') || (duplicate_path[2]==L'/') ) )
			{
				if (drv)
				{
					wcsncpy(drv, duplicate_path, 2);
					drv[2] = L'\0';
					duplicate_path = duplicate_path + 2;
				}
			}
		}
	}


	/* find the last slash in the path */
	#ifdef _MSC_VER
	lastslash = wcsrchr(duplicate_path, L'\\');
	#else
	lastslash = wcsrchr(duplicate_path, L'/');
	#endif

	/* fill in directory */
	if(dir) 
	{
		if(lastslash == 0) wcscpy(dir, L"");
		else
		{
			int len = (int)wcslen(duplicate_path) - (int)wcslen(lastslash) + 1;
			wcsncpy(dir, duplicate_path, len);
			dir[len] = L'\0';
		}
	}

	/* Get the last dot in the filename */
	begin = (lastslash != 0) ? lastslash+1 : duplicate_path;
	lastdot = wcsrchr(begin, L'.');

	/* fill filename and extension */
	if(lastdot == 0) 
	{
		if(name) wcscpy(name, begin);
	} 
	else 
	{
		if(name) 
		{
			if (lastdot)
			{
				int len = (int)wcslen(begin) - (int)wcslen(lastdot);
				wcsncpy(name, begin, len);
				name[len] = L'\0';
			}
		}
		if(ext) wcscpy(ext, lastdot);
	}

	if (begin_duplicate_path) { FREE(begin_duplicate_path); begin_duplicate_path = NULL;}

	/* swap name & extension if no name */
	if ( (name[0] == 0) && (wcslen(ext)>0) )
	{
		wcscpy(name, ext);
		wcscpy(ext, L"");
	}
}
/*--------------------------------------------------------------------------*/ 
void splitpath(const char* path, BOOL bExpand, char* drv, char* dir, char* name, char* ext)
{
	wchar_t *wcpath = to_wide_string((char*)path);
	wchar_t *wcdrv = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));
	wchar_t *wcdir = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));
	wchar_t *wcname = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));
	wchar_t *wcext = (wchar_t*)MALLOC(sizeof(wchar_t) * (PATH_MAX + 1));

	char *buffer = NULL;

	if (drv) strcpy(drv, "");
	if (dir) strcpy(dir, "");
	if (name) strcpy(name, "");
	if (ext) strcpy(ext, "");

	splitpathW(wcpath, bExpand, wcdrv, wcdir, wcname, wcext);

	buffer = wide_string_to_UTF8(wcdrv);
	if (buffer)
	{
		strcpy(drv, buffer);
		FREE(buffer);
		buffer = NULL;
	}
	FREE(wcpath); wcpath = NULL;


	buffer = wide_string_to_UTF8(wcdir);
	if (buffer)
	{
		strcpy(dir, buffer);
		FREE(buffer);
		buffer = NULL;
	}
	FREE(wcdir); wcdir = NULL;

	buffer = wide_string_to_UTF8(wcname);
	if (buffer)
	{
		strcpy(name, buffer);
		FREE(buffer);
		buffer = NULL;
	}
	FREE(wcname); wcname = NULL;

	buffer = wide_string_to_UTF8(wcext);
	if (buffer)
	{
		strcpy(ext, buffer);
		FREE(buffer);
		buffer = NULL;
	}
	FREE(wcext); wcext = NULL;
}
/*--------------------------------------------------------------------------*/ 
