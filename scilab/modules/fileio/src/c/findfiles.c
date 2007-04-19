/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
	#include <Windows.h>
#else
#endif
#include <stdio.h>
#include "findfiles.h"
#include "MALLOC.h"
/*-----------------------------------------------------------------------------------*/ 
#ifdef _MSC_VER
char **findfiles(char *path,char *filespec,int *sizeListReturned)
{
	char **ListFiles=NULL;
	char *strPattern=NULL;
	HANDLE hFile;
	WIN32_FIND_DATA FileInformation;
	int nbElements=0;

	strPattern=(char*)MALLOC(sizeof(char)*(strlen(path)+strlen(filespec)+8));
	sprintf(strPattern,"%s/%s",path,filespec);

	hFile = FindFirstFile(strPattern, &FileInformation);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ( strcmp(FileInformation.cFileName,".") && strcmp(FileInformation.cFileName,"..") )	
			{
				nbElements++;
				if (ListFiles) ListFiles=(char**)REALLOC(ListFiles,sizeof(char**)*(nbElements));
				else ListFiles=(char**)MALLOC(sizeof(char**)*(nbElements));
				ListFiles[nbElements-1]=(char*)MALLOC(sizeof(char)*(strlen(FileInformation.cFileName)+1));
				strcpy(ListFiles[nbElements-1],FileInformation.cFileName);
			}

		} while(FindNextFile(hFile, &FileInformation) == TRUE);
	}
	FindClose(hFile);

	*sizeListReturned=nbElements;
	return ListFiles;
}
#else
/*-----------------------------------------------------------------------------------*/ 
char **findfiles(char *path,char *filespec,int *sizeListReturned)
{
	char **ListFiles=NULL;
	*sizeListReturned=0;

	return ListFiles;
}
#endif
/*-----------------------------------------------------------------------------------*/ 
