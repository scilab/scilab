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
#endif
#include <string.h>
#include "isDrive.h"
#include "isdir.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
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
		char *pathTmp = NULL;
		pathTmp = MALLOC(sizeof(char)*((int)strlen(path)+1));
		if (pathTmp)
		{
			WIN32_FIND_DATA ffd;
			HANDLE sh = NULL;
			strcpy(pathTmp,path);

			if ( (pathTmp[strlen(pathTmp)-1]=='\\') || (pathTmp[strlen(pathTmp)-1]=='/') )
			{
				pathTmp[strlen(pathTmp)-1]='\0';
			}

			sh = FindFirstFile(pathTmp, &ffd);
			FREE(pathTmp);pathTmp=NULL;
			if(INVALID_HANDLE_VALUE == sh) 
			{
				FindClose(sh);
				return FALSE;
			}
			if(ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) 
			{
				FindClose(sh);
				return TRUE;
			}
		}
	}
#endif
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
