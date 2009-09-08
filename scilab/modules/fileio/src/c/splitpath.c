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
/*--------------------------------------------------------------------------*/ 
void splitpathW(const wchar_t* path, wchar_t* drv, wchar_t* dir, wchar_t* name, wchar_t* ext)
{
	const wchar_t* end; /* end of processed string */
	const wchar_t* p;   /* search pointer */
	const wchar_t* s;   /* copy pointer */

	/* extract drive name */
	if (path[0] && path[1]==L':') 
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
		if (*--p==L'\\' || *p==L'/') 
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
void splitpath(const char* path, char* drv, char* dir, char* name, char* ext)
{
	wchar_t *wcpath = to_wide_string(path);
	wchar_t *wcdrv = to_wide_string(path);
	wchar_t *wcdir = to_wide_string(path);
	wchar_t *wcname = to_wide_string(path);
	wchar_t *wcext = to_wide_string(path);

	char *buffer = NULL;

	splitpathW(wcpath, wcdrv, wcdir, wcname, wcext);

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
