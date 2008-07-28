/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) DIGITEO - Allan CORNET
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
#include <strsafe.h>
#include <stdlib.h>
#include "setDOCBOOK_ROOT.h"
#include "win_mem_alloc.h" /* MALLOC */
/*--------------------------------------------------------------------------*/
void setDOCBOOK_ROOT(void)
{
	#define DOCBOOK_ROOT_ENV "DOCBOOK_ROOT"
	#define DOCBOOK_ROOT_FORMAT "%s\\thirdparty\\docbook"
	char ScilabModuleName[MAX_PATH + 1];
	
	if (GetModuleFileNameA ((HINSTANCE)GetModuleHandle(TEXT("libScilab")), (char*) ScilabModuleName, MAX_PATH))
	{
		char drive[_MAX_DRIVE];
		char dir[_MAX_DIR];
		char fname[_MAX_FNAME];
		char ext[_MAX_EXT];
		char *ScilabPath = NULL;
		char *DirTmp = NULL;

		_splitpath(ScilabModuleName,drive,dir,fname,ext);

		if (dir[strlen(dir)-1] == '\\') dir[strlen(dir)-1] = '\0';

		DirTmp = strrchr (dir, '\\');
		if (strlen(dir)-strlen(DirTmp)>0)
		{
			dir[strlen(dir)-strlen(DirTmp)] = '\0';
			ScilabPath = (char*)MALLOC((int)( strlen(drive) + strlen(dir) + 5)*sizeof(char));
			if (ScilabPath)
			{
				int length_DOCBOOKStr = 0;
				char *DOCBOOKStr = NULL;
				_makepath(ScilabPath,drive,dir,NULL,NULL);
				ScilabPath[strlen(ScilabPath)-1]='\0';

				length_DOCBOOKStr = (int)( strlen(ScilabPath)+ strlen(DOCBOOK_ROOT_FORMAT) );
				DOCBOOKStr = (char*)MALLOC(sizeof(char)*(length_DOCBOOKStr)+1);
				if (DOCBOOKStr)
				{
					StringCchPrintfA(DOCBOOKStr,length_DOCBOOKStr,DOCBOOK_ROOT_FORMAT,ScilabPath);
					SetEnvironmentVariableA(DOCBOOK_ROOT_ENV,DOCBOOKStr);
					FREE(DOCBOOKStr);
					DOCBOOKStr = NULL;
				}
			}
		}
	}
}
/*--------------------------------------------------------------------------*/