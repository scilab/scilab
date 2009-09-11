/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
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
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <errno.h>
#endif
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "localization.h"
#include "sciprint.h"
#include "removedir.h"
#include "isdir.h"
#include "MALLOC.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
static int DeleteDirectory(wchar_t *refcstrRootDirectory);
#else
static int DeleteDirectory(char *refcstrRootDirectory);
#endif
/*--------------------------------------------------------------------------*/ 
BOOL removedir(char *path)
{
	if (isdir(path))
	{
#ifdef _MSC_VER
		{
			wchar_t *pstPath = to_wide_string(path);
			if(pstPath)
			{
				removedirW(pstPath);
				FREE(pstPath);
				pstPath = NULL;
			}
		}
#else
		DeleteDirectory(path);
#endif
		if (!isdir(path)) return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
BOOL removedirW(wchar_t *pathW)
{
	if (isdirW(pathW))
	{
#ifdef _MSC_VER
		DeleteDirectory(pathW);
#else
		char *path = wide_string_to_UTF8(pathW);
		if (path)
		{
			DeleteDirectory(path);
			FREE(path);
			path = NULL;
		}
#endif
		if (!isdirW(pathW)) return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
static int DeleteDirectory(wchar_t *refcstrRootDirectory)
{
#define DEFAULT_PATTERN L"%s/*.*"
	BOOL bDeleteSubdirectories = TRUE;
	BOOL bSubdirectory = FALSE;
	HANDLE hFile;
	WIN32_FIND_DATAW FileInformation;
	DWORD dwError;
	wchar_t	*strPattern		= NULL;
	wchar_t	*strFilePath	= NULL;

	strPattern = (wchar_t*)MALLOC(sizeof(wchar_t)*((int)wcslen(refcstrRootDirectory) + (int)wcslen(DEFAULT_PATTERN) + 1));
	swprintf(strPattern,((int)wcslen(refcstrRootDirectory) + (int)wcslen(DEFAULT_PATTERN) + 1), DEFAULT_PATTERN, refcstrRootDirectory);

	hFile = FindFirstFileW(strPattern, &FileInformation);
	if (strPattern) { FREE(strPattern);strPattern=NULL;}

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ( (wcscmp(FileInformation.cFileName,L".") != 0) && (wcscmp(FileInformation.cFileName,L"..") != 0) )
			{
				strFilePath = (wchar_t*)MALLOC(sizeof(wchar_t)*(wcslen(refcstrRootDirectory)+5+wcslen((wchar_t*)(FileInformation.cFileName))));
				swprintf(strFilePath,wcslen(refcstrRootDirectory)+5+wcslen((wchar_t*)(FileInformation.cFileName)),L"%s\\%s",refcstrRootDirectory,FileInformation.cFileName);

				if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(bDeleteSubdirectories)
					{
						int iRC = DeleteDirectory(strFilePath);
						if (strFilePath) {FREE(strFilePath); strFilePath = NULL;}
						if (strPattern) {FREE(strPattern); strPattern = NULL;}

						if(iRC) 
						{
							return iRC;
						}
					}
					else bSubdirectory = TRUE;
				}
				else
				{
					if(SetFileAttributesW(strFilePath,FILE_ATTRIBUTE_NORMAL) == FALSE) 
					{
						if (strFilePath) {FREE(strFilePath); strFilePath = NULL;}
						if (strPattern) {FREE(strPattern); strPattern = NULL;}
						return GetLastError();
					}

					if(DeleteFileW(strFilePath) == FALSE) 
					{
						if (strFilePath) {FREE(strFilePath); strFilePath = NULL;}
						if (strPattern) {FREE(strPattern); strPattern = NULL;}
						return GetLastError();
					}
				}
			}
		} while(FindNextFileW(hFile, &FileInformation) == TRUE);

		FindClose(hFile);
		if (strFilePath) {FREE(strFilePath); strFilePath = NULL;}
		if (strPattern) {FREE(strPattern); strPattern = NULL;}

		dwError = GetLastError();
		if(dwError != ERROR_NO_MORE_FILES) 
		{
			return dwError;
		}
		else
		{
			if(!bSubdirectory)
			{
				if(SetFileAttributesW(refcstrRootDirectory,FILE_ATTRIBUTE_NORMAL) == FALSE) 
				{
					return GetLastError();
				}
				if(RemoveDirectoryW(refcstrRootDirectory) == FALSE)	
				{
					return GetLastError();
				}
			}
		}
	}

	if (strFilePath) {FREE(strFilePath); strFilePath = NULL;}
	if (strPattern) {FREE(strPattern); strPattern = NULL;}
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
#ifndef _MSC_VER
static int DeleteDirectory(char *refcstrRootDirectory)
{
	DIR *dir;
	struct dirent *ent;
	dir = opendir(refcstrRootDirectory) ;

	if (dir==NULL)
	{
		sciprint(_("Warning: Error while opening %s: %s\n"),refcstrRootDirectory, strerror(errno));
		return -1;
	}
	while((ent = readdir(dir)) != NULL)
	{
		char *filename = NULL;
		if (strcmp(ent->d_name, ".") == 0 || strcmp(ent->d_name, "..") == 0) continue ;
		filename = MALLOC(sizeof(char)*(strlen(refcstrRootDirectory) + 1 + strlen(ent->d_name) + 1 + 1)) ;
		sprintf(filename,"%s/%s",refcstrRootDirectory,ent->d_name);
		if (isdir(filename)) {
			/* Delete recursively */
			DeleteDirectory(filename);
		} else {
			/* Not a directory... It must be a file (at least, I hope it is a file */
			if (remove(filename) != 0) {
				sciprint(_("Warning: Could not remove file %s: %s\n"),filename, strerror(errno));
			}
			if (filename) {FREE(filename);filename=NULL;}
		}
	}
	if (rmdir(refcstrRootDirectory) != 0) {
		sciprint(_("Warning: Could not remove directory %s: %s\n"),refcstrRootDirectory, strerror(errno));
	}
	return 0;
}
#endif
/*--------------------------------------------------------------------------*/
