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
	#include <sys/types.h>
	#include <dirent.h>
#endif
#include <stdio.h>
#include <string.h>
#include "findfiles.h"
#include "MALLOC.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/ 
#ifndef _MSC_VER
static BOOL find_spec( char *filename ,char *filespec);
#endif
/*--------------------------------------------------------------------------*/ 
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
	if (strPattern) {FREE(strPattern);strPattern=NULL;}
	if(hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			if ( strcmp(FileInformation.cFileName,".") && strcmp(FileInformation.cFileName,"..") )	
			{
				nbElements++;
				if (ListFiles) ListFiles=(char**)REALLOC(ListFiles,sizeof(char*)*(nbElements));
				else ListFiles=(char**)MALLOC(sizeof(char*)*(nbElements));
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
/*--------------------------------------------------------------------------*/ 
char **findfiles(char *path,char *filespec,int *sizeListReturned)
{
	char **ListFiles=NULL;
	int nbElements=0;
	DIR *folder=NULL;
	struct dirent *read=NULL;

	*sizeListReturned=0;

	folder = opendir(path);
	if (folder)
	{
		while ((read = readdir(folder)))
		{
			if ( strcmp(read->d_name,".") && strcmp(read->d_name,"..") )
			{
				if ( find_spec(read->d_name ,filespec) )
				{
					nbElements++;
					if (ListFiles) ListFiles=(char**)REALLOC(ListFiles,sizeof(char*)*(nbElements));
					else ListFiles=(char**)MALLOC(sizeof(char*)*(nbElements));

					ListFiles[nbElements-1]=(char*)MALLOC(sizeof(char)*(strlen(read->d_name)+1));
					strcpy(ListFiles[nbElements-1],read->d_name);
				}
			}
		}
		closedir(folder);
	}

	*sizeListReturned=nbElements;
	return ListFiles;
}
#endif
/*--------------------------------------------------------------------------*/ 
#ifndef _MSC_VER
static BOOL find_spec( char *filename ,char *filespec)
{
	char *any = NULL;

	while( ( *filespec ) || ( *filename ) )
	{
		switch( *filespec )
		{
		case '*':
			any = filespec;
			filespec++;
			while( ( *filename != *filespec ) && ( *filename ) ) filename++;
			break;

		case '?':
			filespec++;
			if( *filename ) filename++;
			break;

		default:
			if( *filespec != *filename )
			{
				if( ( any ) && ( *filename ) ) filespec = any;
				else return FALSE;
			}
			else
			{
				filespec++;
				filename++;
			}
			break;
		}
	}

	return TRUE;
}
#endif
/*--------------------------------------------------------------------------*/ 
