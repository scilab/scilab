/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <Windows.h>
#include "winopen.h"
#include "expandPathVariable.h"
#include "Scierror.h"
#include "PATH_MAX.h"
#include "charEncoding.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
BOOL winopen(char *scilabfilename)
{
	BOOL bOK = FALSE;
	char *filename = NULL;
	wchar_t *wcfilename = NULL;
	int out_n = 0;
	long int lout = 0;
	HINSTANCE error = NULL;

	filename = expandPathVariable(scilabfilename);
	if (filename)
	{
		wcfilename = to_wide_string(filename);
		FREE(filename); filename = NULL;
		if (wcfilename)
		{
			error = ShellExecuteW(NULL, L"open", wcfilename, NULL, NULL, SW_SHOWNORMAL);
			if ( error <= (HINSTANCE)32) bOK = FALSE;
			else bOK = TRUE;
		}
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
