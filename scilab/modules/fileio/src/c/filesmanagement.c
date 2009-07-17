/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2007 - INRIA - Allan CORNET
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include <string.h>
#include <stdlib.h>
#include "PATH_MAX.h"
#include "filesmanagement.h"
#include "core_math.h" /* Min Max */
#include "MALLOC.h"
#ifdef _MSC_VER
#include "strdup_windows.h"
#endif
#include "fullpath.h"
/*--------------------------------------------------------------------------*/
typedef struct {
	FILE *ftformat;
	int ftswap; /* swap status for each file */
	int ftmode; /* mode for each file */
	int fttype; /* type (Fortran,C) for each file must be zero initialized */
	char *ftname; /* name for each file */

} scilabfile;
/*--------------------------------------------------------------------------*/
static scilabfile *ScilabFileList = NULL;
static int CurFile = -1;
static int PreviousFile = -1;
static int CurrentMaxFiles = DEFAULT_MAX_FILES;
/*--------------------------------------------------------------------------*/
FILE *GetFileOpenedInScilab(int Id)
{
	int fd1 = 0;

	fd1 = (Id != -1) ?  Min(Max(Id,0),GetMaximumFileOpenedInScilab()-1) : CurFile ;

	if ( fd1 != -1 )
	{
		return  ScilabFileList[fd1].ftformat;
	}
	return NULL;
}
/*--------------------------------------------------------------------------*/
int GetCurrentFileId(void)
{
	return CurFile;
}
/*--------------------------------------------------------------------------*/
int GetPreviousFileId(void)
{
	return PreviousFile;
}
/*--------------------------------------------------------------------------*/
void SetCurrentFileId(int Id)
{
	if (Id == -1) PreviousFile = -1;
	else PreviousFile = CurFile;

	CurFile = Id;
}
/*--------------------------------------------------------------------------*/
void SetFileOpenedInScilab(int Id,FILE *fptr)
{
	ScilabFileList[Id].ftformat=fptr;
}
/*--------------------------------------------------------------------------*/
int GetSwapStatus(int Id)
{
	int fd1;
	fd1 = (Id != -1) ?  Min(Max(Id,0),GetMaximumFileOpenedInScilab()-1) : GetCurrentFileId() ;
	if ( fd1 != -1 ) return(ScilabFileList[fd1].ftswap);
	return(0);
}
/*--------------------------------------------------------------------------*/
void SetSwapStatus(int Id,int newswap)
{
	ScilabFileList[Id].ftswap =  newswap;
}
/*--------------------------------------------------------------------------*/
int GetMaximumFileOpenedInScilab(void)
{
	return CurrentMaxFiles;
}
/*--------------------------------------------------------------------------*/
int GetFileModeOpenedInScilab(int Id)
{
	return ScilabFileList[Id].ftmode;
}
/*--------------------------------------------------------------------------*/
void SetFileModeOpenedInScilab(int Id,int mode)
{
	ScilabFileList[Id].ftmode = mode;
}
/*--------------------------------------------------------------------------*/
int GetFileTypeOpenedInScilab(int Id)
{
	return ScilabFileList[Id].fttype;
}
/*--------------------------------------------------------------------------*/
void SetFileTypeOpenedInScilab(int Id,int Type)
{
	ScilabFileList[Id].fttype = Type;
}
/*--------------------------------------------------------------------------*/
char* GetFileNameOpenedInScilab(int Id)
{
	if (GetFileOpenedInScilab(Id) != NULL) return ScilabFileList[Id].ftname;
	return NULL;
}
/*--------------------------------------------------------------------------*/
BOOL SetFileNameOpenedInScilab(int Id,char *name)
{
	BOOL bOK=FALSE;
	char *ptrName=NULL;
	char fullpath[PATH_MAX*4];

	/* A exception for Id 5 and 6 */
	/* no filename */
	if ( strcmp(name,"") == 0 )
	{
		ptrName = strdup(name);
		if (ptrName)
		{
			bOK=TRUE;
		}
	}
	else
	{
		if( get_full_path( fullpath, name, PATH_MAX*4 ) != NULL )
		{
			ptrName = strdup(fullpath);
			if (ptrName)
			{
				bOK=TRUE;
			}
		}
		else
		{
			ptrName = strdup(name);
			if (ptrName)
			{
				bOK=TRUE;
			}
		}
	}
	ScilabFileList[Id].ftname = ptrName;
	return bOK;
}
/*--------------------------------------------------------------------------*/
BOOL FreeFileNameOpenedInScilab(int Id)
{
	char *ptr = ScilabFileList[Id].ftname;
	if (ptr) { FREE(ptr);  ptr = NULL; return TRUE;}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL InitializeScilabFilesList(void)
{
	if (!ScilabFileList)
	{
		CurrentMaxFiles=DEFAULT_MAX_FILES;
		ScilabFileList=(scilabfile *)MALLOC(sizeof(scilabfile)*CurrentMaxFiles);

		if (ScilabFileList)
		{
			int i=0;
			for (i=0;i<CurrentMaxFiles;i++)
			{
				ScilabFileList[i].ftformat=NULL;
				ScilabFileList[i].ftmode=0;
				ScilabFileList[i].ftname=NULL;
				ScilabFileList[i].ftswap=0;
				ScilabFileList[i].fttype=0;
			}
			return TRUE;
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL TerminateScilabFilesList(void)
{
	if (ScilabFileList)
	{
		FREE(ScilabFileList);
		ScilabFileList=NULL;
		return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL ExtendScilabFilesList(int NewSize)
{
	if (ScilabFileList)
	{
		if (NewSize > CurrentMaxFiles)
		{

			scilabfile *ScilabFileListTmp=NULL;
			ScilabFileListTmp=(scilabfile *)REALLOC(ScilabFileList,NewSize*sizeof(scilabfile));
			if (ScilabFileListTmp)
			{
				int i=0;
				ScilabFileList=ScilabFileListTmp;
				for (i=CurrentMaxFiles;i<NewSize;i++)
				{
					ScilabFileList[i].ftformat=NULL;
					ScilabFileList[i].ftmode=0;
					ScilabFileList[i].ftname=NULL;
					ScilabFileList[i].ftswap=0;
					ScilabFileList[i].fttype=0;
				}
				CurrentMaxFiles=NewSize;
				return TRUE;
			}
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
BOOL IsAlreadyOpenedInScilab(char *filename)
{
	if (ScilabFileList)
	{
		char fullpath[PATH_MAX*4];
		int i=0;

		if( get_full_path( fullpath, filename, PATH_MAX*4 ) == NULL )
		{
			/* if we are a problem */
			strcpy(fullpath,filename);
		}

		for (i=0;i<CurrentMaxFiles;i++)
		{
			if ( (ScilabFileList[i].ftformat) && ScilabFileList[i].ftname)
			{
				if (strcmp(ScilabFileList[i].ftname,fullpath) == 0) return TRUE;
			}
		}
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
int GetIdFromFilename(char *filename)
{
	if (ScilabFileList)
	{
		char fullpath[PATH_MAX*4];
		int i=0;
		if( get_full_path( fullpath, filename, PATH_MAX*4 ) == NULL )
		{
			/* if we are a problem */
			strcpy(fullpath,filename);
		}

		for (i = 0; i < CurrentMaxFiles; i++)
		{
			if ( (ScilabFileList[i].ftformat) && ScilabFileList[i].ftname)
			{
				if (strcmp(ScilabFileList[i].ftname,fullpath) == 0) return i;
			}
		}
	}
	return FILE_ID_NOT_DEFINED;
}
/*--------------------------------------------------------------------------*/
