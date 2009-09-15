/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * ...
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <Windows.h>
#else
#include <stdio.h>
#endif
#include "FileExist.h"
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
BOOL FileExist(char *filename)
{
#ifdef _MSC_VER
	wchar_t *wcFilename = to_wide_string(filename);
	if (wcFilename)
	{
		BOOL bOK = FileExistW(wcFilename);
		FREE(wcFilename);
		return bOK;
	}
	return FALSE;
#else
	FILE* tmpFile=fopen(filename,"r");
	if(tmpFile) 
		{ 
			fclose(tmpFile); 
			return TRUE;
		} 
	else 
		{ 
			return FALSE;
		} 
#endif

}
/*--------------------------------------------------------------------------*/ 
BOOL FileExistW(wchar_t *wcfilename)
{
#ifdef _MSC_VER
	if (wcfilename)
	{
		WIN32_FIND_DATAW FindFileData;
		HANDLE handle = FindFirstFileW (wcfilename, &FindFileData);
		if (handle != INVALID_HANDLE_VALUE)
		{
			FindClose (handle);
			return TRUE;
		}
		else return FALSE;
	}
	else return FALSE;
	
#else
	char *filename = wide_string_to_UTF8(wcfilename);
	if (filename)
	{
		BOOL bOK = FileExist(filename);
		FREE(filename);
		return bOK;
	}
	return FALSE;
#endif
}
/*--------------------------------------------------------------------------*/ 
