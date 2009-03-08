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
/*--------------------------------------------------------------------------*/ 
static int DeleteDirectory(char *refcstrRootDirectory);
/*--------------------------------------------------------------------------*/ 
BOOL removedir(char *path)
{
	if (isdir(path))
	{
		DeleteDirectory(path);
		if (!isdir(path)) return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
static int DeleteDirectory(char *refcstrRootDirectory)
{
	#define DEFAULT_PATTERN "%s\\*.*"
	BOOL bDeleteSubdirectories = TRUE;
	BOOL bSubdirectory = FALSE;
	HANDLE hFile;
	char *strFilePath = NULL;
	char *strPattern = NULL;
	WIN32_FIND_DATA FileInformation;
	DWORD dwError;

	strPattern = (char*)MALLOC(sizeof(char)*((int)strlen(refcstrRootDirectory) + (int)strlen(DEFAULT_PATTERN) + 1));
	sprintf(strPattern, DEFAULT_PATTERN, refcstrRootDirectory);

	hFile = FindFirstFile(strPattern, &FileInformation);
	if (strPattern) { FREE(strPattern);strPattern=NULL;}

	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if(FileInformation.cFileName[0] != '.')
			{
				if (strFilePath) {FREE(strFilePath);strFilePath=NULL;}
				strFilePath = MALLOC(sizeof(char)*(strlen(refcstrRootDirectory)+5+strlen(FileInformation.cFileName)));
				sprintf(strFilePath,"%s\\%s",refcstrRootDirectory,FileInformation.cFileName);

				if(FileInformation.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
				{
					if(bDeleteSubdirectories)
					{
						int iRC = DeleteDirectory(strFilePath);
						if(iRC) return iRC;
					}
					else bSubdirectory = TRUE;
				}
				else
				{
					if(SetFileAttributes(strFilePath,FILE_ATTRIBUTE_NORMAL) == FALSE) return GetLastError();
					if(DeleteFile(strFilePath) == FALSE) return GetLastError();
				}
			}
		} while(FindNextFile(hFile, &FileInformation) == TRUE);

		FindClose(hFile);

		dwError = GetLastError();
		if(dwError != ERROR_NO_MORE_FILES) return dwError;
		else
		{
			if(!bSubdirectory)
			{
				if(SetFileAttributes(refcstrRootDirectory,FILE_ATTRIBUTE_NORMAL) == FALSE) return GetLastError();
				if(RemoveDirectory(refcstrRootDirectory) == FALSE)	return GetLastError();
			}
		}
	}

	if (strFilePath) {FREE(strFilePath);strFilePath=NULL;}
	if (strFilePath) {FREE(strFilePath);strFilePath=NULL;}

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
