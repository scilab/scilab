/*--------------------------------------------------------------------------*/
// Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
// Copyright (C) INRIA - Allan CORNET
// 
// This file must be used under the terms of the CeCILL.
// This source file is licensed as described in the file COPYING, which
// you should have received as part of this distribution.  The terms
// are also available at    
// http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
/*--------------------------------------------------------------------------*/ 
#include <windows.h>
#include <stdlib.h>
#include "machine.h"
#include "getScilabDirectory.h"
#include "MALLOC.h"
#include "setgetSCIpath.h"
/*--------------------------------------------------------------------------*/ 
char *getScilabDirectory(BOOL UnixStyle)
{
	LPSTR ScilabModuleName=NULL;
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	char *SciPathName=NULL;
	char *DirTmp=NULL;

	ScilabModuleName = (LPSTR) MALLOC (MAX_PATH + 1);

	if (!GetModuleFileName ((HINSTANCE)GetModuleHandle("libScilab"), (LPSTR) ScilabModuleName, MAX_PATH))
	{
		if (ScilabModuleName) {FREE(ScilabModuleName);ScilabModuleName=NULL;}
		return NULL;
	}

	_splitpath(ScilabModuleName,drive,dir,fname,ext);
	if (ScilabModuleName) {FREE(ScilabModuleName);ScilabModuleName=NULL;}
	if (dir[strlen(dir)-1] == '\\') dir[strlen(dir)-1] = '\0';

	DirTmp=strrchr (dir, '\\');
	if (strlen(dir)-strlen(DirTmp)>0)
	{
		dir[strlen(dir)-strlen(DirTmp)] = '\0';
	}
	else return NULL;

	SciPathName = (char*)MALLOC((int)( strlen(drive) + strlen(dir) + 5)*sizeof(char));
	if (SciPathName)
	{
		_makepath(SciPathName,drive,dir,NULL,NULL);

		if ( UnixStyle )
		{	
			int i=0;
			for (i=0;i<(int)strlen(SciPathName);i++)
			{
				if (SciPathName[i]=='\\') SciPathName[i]='/';
			}
		}
		SciPathName[strlen(SciPathName)-1]='\0';
		setSCIpath(SciPathName);

	}
	return SciPathName;
}
/*--------------------------------------------------------------------------*/ 
