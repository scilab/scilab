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
#include "getFullFilename.hxx"
/*--------------------------------------------------------------------------*/ 
#ifndef _MSC_VER
#include <unistd.h>
#endif
extern "C"
{
#include "machine.h"
#include "PATH_MAX.h"
#include "MALLOC.h"
#include "charEncoding.h"
}
/*--------------------------------------------------------------------------*/ 
static void wcsplitpath(const wchar_t* path, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext);
/*--------------------------------------------------------------------------*/ 
std::wstring getFullFilename(std::wstring _wfilename)
{
	wchar_t wcdrive[PATH_MAX];
	wchar_t wcdirectory[PATH_MAX];
	wchar_t wcname[PATH_MAX];
	wchar_t wcext [PATH_MAX];

	std::wstring wfullfilename(L"");
	std::wstring tmpWstr;

	size_t found = _wfilename.rfind(L"\\");

	while (found != std::wstring::npos)
	{
		_wfilename.replace (found, 1, L"/");
		found = _wfilename.rfind(L"\\");
	}
	wcsplitpath(_wfilename.c_str(), wcdrive, wcdirectory, wcname, wcext);
	wfullfilename.append(tmpWstr.assign(wcdrive));
	wfullfilename.append(tmpWstr.assign(wcdirectory));
	if (wfullfilename.compare(L"") == 0)
	{
		/* to get current directory as wide characters */
#if _MSC_VER
		wchar_t wcCurrentDir[PATH_MAX];
		if ( _wgetcwd(wcCurrentDir, PATH_MAX) != NULL)
		{
			wfullfilename = tmpWstr.assign(wcCurrentDir);
#else
		char CurrentDir[PATH_MAX];
		if (getcwd(CurrentDir, PATH_MAX) != NULL)
		{
			wchar_t *wcCurrentDir = to_wide_string(CurrentDir);
			wfullfilename = tmpWstr.assign(wcCurrentDir);
			FREE(wcCurrentDir);
#endif
			/* replaces separator */
			size_t found = wfullfilename.rfind(L"\\");
			while (found != std::wstring::npos)
			{
				wfullfilename.replace (found, 1, L"/");
				found = wfullfilename.rfind(L"\\");
			}
			wfullfilename.append(L"/");
		}
		else
		{
			wfullfilename.assign(L"");
		}
	}
	wfullfilename.append(tmpWstr.assign(wcname));
	wfullfilename.append(tmpWstr.assign(wcext));

	return wfullfilename;
}
/*--------------------------------------------------------------------------*/ 
static void wcsplitpath(const wchar_t* path, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext)
{
	const wchar_t* end; /* end of processed string */
	const wchar_t* p;   /* search pointer */
	const wchar_t* s;   /* copy pointer */

	/* extract drive name */
	if (path[0] && path[1]==':') 
	{
		if (drv) 
		{
			*drv++ = *path++;
			*drv++ = *path++;
			*drv = L'\0';
		}
	} else if (drv) *drv = L'\0';

	/* search for end of string or stream separator */
	for(end=path; *end && *end!=L':'; )	end++;

	/* search for begin of file extension */
	for(p=end; p>path && *--p!=L'\\' && *p!=L'/'; )
	{
		if (*p == L'.') 
		{
			end = p;
			break;
		}
	}

	if (ext) for(s=end; (*ext=*s++); ) ext++;

	/* search for end of directory name */
	for(p=end; p>path; )
	{
		if (*--p=='\\' || *p=='/') 
		{
			p++;
			break;
		}
	}
	if (name) 
	{
		for(s=p; s<end; ) *name++ = *s++;
		*name = L'\0';
	}

	if (dir) 
	{
		for(s=path; s<p; ) *dir++ = *s++;
		*dir = L'\0';
	}
}
/*--------------------------------------------------------------------------*/ 
