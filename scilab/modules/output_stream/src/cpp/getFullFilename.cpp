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
#include <fstream> 
#include <iostream>
#include <sstream>  
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
static bool fileExists(std::wstring _wfilename);
static int GetFileSize(std::wstring _wfilename);
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
std::wstring getUniqueFilename(std::wstring _wfilename)
{
	std::wstring wfullfilename = getFullFilename(_wfilename);
	std::wstring newfilename;

	if (fileExists(wfullfilename) == true)
	{
		wchar_t wcdrive[PATH_MAX];
		wchar_t wcdirectory[PATH_MAX];
		wchar_t wcname[PATH_MAX];
		wchar_t wcext [PATH_MAX];
		unsigned int id = -1;

		std::wstring prefixFilename;

		wcsplitpath(_wfilename.c_str(), wcdrive, wcdirectory, wcname, wcext);
		prefixFilename.assign(L"");
		prefixFilename.append(wcdrive);
		prefixFilename.append(wcdirectory);
		prefixFilename.append(wcname);

		do
		{
			std::wstringstream StrStream;
			id++;
			StrStream << (unsigned int)id;
			newfilename = prefixFilename + L"_" + StrStream.str() + wcext;
		}
		while( (fileExists(newfilename) == true) && (GetFileSize(newfilename) != 0) );
	}
	else
	{
		newfilename = wfullfilename;
	}
	return newfilename;
}
/*--------------------------------------------------------------------------*/ 
static bool fileExists(std::wstring _wfilename)
{
	bool returnVal = false;
#if _MSC_VER
	std::wifstream f(_wfilename.c_str());
#else
	char *_filename = wide_string_to_UTF8((wchar_t*)_wfilename.c_str());
	if (_filename == NULL) return false;
        std::ifstream f(_filename);
	FREE(_filename); _filename = NULL;
#endif
	if (f.is_open())
	{
		f.close();
		returnVal = true;
	}
	else
	{
		returnVal = false;
	}
	return returnVal;
}
/*--------------------------------------------------------------------------*/ 
static int GetFileSize(std::wstring _wfilename) 
{
#ifdef _MSC_VER
	std::wifstream file(_wfilename.c_str());
#else
	char *_filename = wide_string_to_UTF8((wchar_t*)_wfilename.c_str());
	if (_filename == NULL) return false;
	std::ifstream file(_filename);
#endif
	file.seekg(std::ios::end); 
	return (int)file.tellg();
}
/*--------------------------------------------------------------------------*/ 
