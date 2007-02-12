/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef _MSC_VER
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h> 
#else
	#include <Windows.h>
#endif
#include <stdio.h>
#include "createdir.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#define DIRMODE 0777
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
int DeleteDirectory(char *refcstrRootDirectory);
#endif
/*-----------------------------------------------------------------------------------*/ 
BOOL ExistDir(char * path)
{
	BOOL bOK=FALSE;
	#ifndef _MSC_VER
		struct stat buf;
		if (path == NULL) return FALSE;

		if ( (stat (path, &buf)) && (S_ISDIR (buf.st_mode)) )
		{
			bOK=TRUE;
		}
	#else
		WIN32_FIND_DATA ffd;
		HANDLE sh = FindFirstFile(path, &ffd);

		if(INVALID_HANDLE_VALUE == sh) return FALSE;
		if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) bOK=TRUE;
	#endif

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL CreateDir(const char* path) 
{
	BOOL bOK=FALSE;
	#ifndef _MSC_VER
		if  (!ExistDir(path)) 
		{
			if (mkdir(path, DIRMODE) == 0) 
			{
				bOK=TRUE;
			}
		}
	#else
		if (CreateDirectory(path,NULL)) bOK=TRUE;
	#endif
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL RemoveDir(char *path)
{
	BOOL bOK=FALSE;
	if (ExistDir(path))
	{
		#ifndef _MSC_VER
			
			if (rmdir(path) == 0 ) 
			{
				bOK=TRUE;
			}
			else
			{
				char buf[1024];
				sprintf(buf,"rm -rf %s >/dev/null  2>/dev/null",path);
				system(buf);
			}
		#else
			DeleteDirectory(path); 
			if (!ExistDir(path)) bOK=TRUE;
		#endif
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
#if _MSC_VER
int DeleteDirectory(char *refcstrRootDirectory)
{
	BOOL bDeleteSubdirectories=TRUE;
	BOOL bSubdirectory = FALSE;
	HANDLE hFile;
	char *strFilePath=NULL;
	char *strPattern=NULL;
	WIN32_FIND_DATA FileInformation;
	DWORD dwError;

	strPattern = MALLOC(sizeof(char)*(strlen(refcstrRootDirectory)+5));
	sprintf(strPattern,"%s\\*.*",refcstrRootDirectory);

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
/*-----------------------------------------------------------------------------------*/ 
