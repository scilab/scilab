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
#include <windows.h>
#include <stdlib.h>
#include "copyBlasFile.h"
#include "GetPathOfThisExe.h"
#include "FileExist.h"
/*--------------------------------------------------------------------------*/
#define BLAS_FILENAME_DEST L"blasplus.dll"
#define ATLAS_DIRECTORY L"Atlas"
#define FILE_SEPARATOR L"\\"
#define FORMAT_DESTINATION_FILENAME L"%s\\%s"
#define FORMAT_SOURCE_FILENAME L"%s\\%s\\%s"
/*--------------------------------------------------------------------------*/
static wchar_t* buildDestinationFilename(wchar_t* execpath);
static wchar_t* buildSourceFilename(wchar_t* execpath,wchar_t* blasDllName);
/*--------------------------------------------------------------------------*/
copy_blas_error copyBlasFile(wchar_t *blasDllName)
{
	copy_blas_error err = COPY_OK;
	wchar_t *currentexecpath = GetPathOfThisExe();
	if (currentexecpath)
	{
		wchar_t *destFilename = buildDestinationFilename(currentexecpath);
		wchar_t *srcFilename = buildSourceFilename(currentexecpath, blasDllName);
		if (destFilename && srcFilename)
		{
			if (FileExist(srcFilename))
			{
				if (!CopyFileW(srcFilename,destFilename,FALSE))
				{
					err = COPY_FILE_FAILED;
				}
				else
				{
					err = COPY_OK;
				}
			}
			else
			{
				err = COPY_FILE_SRC_NOT_EXISTS;
			}
		}

		if (destFilename) {free(destFilename);destFilename = NULL;}
		if (srcFilename) {free(srcFilename);srcFilename = NULL;}

		free(currentexecpath);
		currentexecpath = NULL;
	}
	else
	{
		err = COPY_DESTINATION_NOT_EXISTS;
	}
	return err;
}
/*--------------------------------------------------------------------------*/
wchar_t* buildDestinationFilename(wchar_t* execpath)
{
	wchar_t* DestinationFilename = NULL;
	if (execpath)
	{
		int len = (int)(wcslen(FORMAT_DESTINATION_FILENAME) +
			wcslen(execpath) + wcslen(BLAS_FILENAME_DEST) + 1);

		DestinationFilename = (wchar_t *) calloc(len, sizeof(wchar_t));
		if (DestinationFilename)
		{
			wsprintfW(DestinationFilename, FORMAT_DESTINATION_FILENAME,
				execpath, BLAS_FILENAME_DEST);
		}
	}
	return DestinationFilename;
}
/*--------------------------------------------------------------------------*/
wchar_t* buildSourceFilename(wchar_t* execpath,wchar_t* blasDllName)
{
	wchar_t* SourceFilename = NULL;
	if (execpath && blasDllName)
	{
		int len = (int)(wcslen(FORMAT_SOURCE_FILENAME) +
					wcslen(execpath) + wcslen(ATLAS_DIRECTORY) +
					wcslen(blasDllName) + 1);

		SourceFilename = (wchar_t *) calloc(len, sizeof(wchar_t));
		if (SourceFilename)
		{
			wsprintfW(SourceFilename, FORMAT_SOURCE_FILENAME,
					execpath, ATLAS_DIRECTORY, blasDllName);
		}
	}
	return SourceFilename;
}
/*--------------------------------------------------------------------------*/
