/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef _MSC_VER
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <fcntl.h>
	#include <unistd.h>
	#include <dirent.h>
#else
	#include <Windows.h>
#endif
#include <stdlib.h>
#include <stdio.h>
#include "directories.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#define DIRMODE 0777
/*-----------------------------------------------------------------------------------*/ 
int DeleteDirectory(char *refcstrRootDirectory);
#ifndef _MSC_VER
static void removefile(char *filename);
#else
static BOOL isDrive(const char *strname);
#endif
/*-----------------------------------------------------------------------------------*/ 
BOOL isdir(const char * path)
{
	BOOL bOK=FALSE;
	#ifndef _MSC_VER
		struct stat buf;
		if (path == NULL) return FALSE;
		if (stat(path, &buf) == 0 && S_ISDIR(buf.st_mode)) bOK=TRUE;
	#else
		if (isDrive(path)) return TRUE;
		else
		{
			char *pathTmp=NULL;

			pathTmp=MALLOC(sizeof(char)*((int)strlen(path)+1));
			if (pathTmp)
			{
				WIN32_FIND_DATA ffd;
				HANDLE sh=NULL;
				strcpy(pathTmp,path);
				if ( (pathTmp[strlen(pathTmp)-1]=='\\') || (pathTmp[strlen(pathTmp)-1]=='/') )
				{
					pathTmp[strlen(pathTmp)-1]='\0';
				}

				sh = FindFirstFile(pathTmp, &ffd);
				FREE(pathTmp);pathTmp=NULL;
				if(INVALID_HANDLE_VALUE == sh) return FALSE;
				if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) bOK=TRUE;
			}
		}
	#endif

	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
BOOL CreateDir(const char* path) 
{
	BOOL bOK=FALSE;
	#ifndef _MSC_VER
		if  (!isdir(path)) 
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
	if (isdir(path))
	{
		DeleteDirectory(path);
		if (!isdir(path)) bOK=TRUE; 
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
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
#ifndef _MSC_VER
static void removefile(char *filename)
{
	FILE *f = fopen(filename, "r") ;
	if (! f) return ;
	fclose(f) ;
	chmod(filename, S_IWRITE) ;
	remove(filename) ;
}
#endif 
/*-----------------------------------------------------------------------------------*/
#ifndef _MSC_VER
int DeleteDirectory(char *refcstrRootDirectory)
{
	DIR *dir;
	struct dirent *ent;

	dir = opendir(refcstrRootDirectory) ;

	if (!dir)
	{
		removefile(refcstrRootDirectory) ;
	} 
	else
	{
		while((ent = readdir(dir)) != NULL)
		{
			char *filename = NULL;
			if (ent->d_name[0] == '.') continue ;

			filename = MALLOC(strlen(refcstrRootDirectory) + 1 + strlen(ent->d_name) + 1 + 1) ;
			sprintf(filename,"%s/%s",refcstrRootDirectory,ent->d_name);
			removefile(filename);
			if (filename) {FREE(filename);filename=NULL;}
		}
		rmdir(refcstrRootDirectory);
	}
	return 0;
}
#endif
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
BOOL isDrive(const char *strname)
{
	BOOL bOK=FALSE;
	if (strname)
	{
		if ( (strlen(strname) == 2) || (strlen(strname) == 3) && (strname[1]== ':') )
		{
			if (strlen(strname) == 3)
			{
				if ( (strname[2]!= '\\') && (strname[2]!= '/') )
				{
					return FALSE;
				}
			}

			if ( ( strname[0] >= 'A' && strname[0] <= 'Z' ) || ( strname[0] >= 'a' && strname[0] <= 'z' ) )
			{
				bOK =TRUE;
			}
		}
	}
	return bOK;
}
#endif
/*-----------------------------------------------------------------------------------*/
