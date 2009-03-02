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
#include "cluni0.h"
#include "Scierror.h"
#include "PATH_MAX.h"
/*--------------------------------------------------------------------------*/
BOOL winopen(char *scilabfilename)
{
	BOOL bOK = FALSE;
	char filename[PATH_MAX + FILENAME_MAX];
	int out_n = 0;
	long int lout = 0;
	HINSTANCE error = NULL;

	lout = PATH_MAX + FILENAME_MAX;
	C2F(cluni0)(scilabfilename, filename, &out_n,(int)strlen(scilabfilename),lout);
	error = ShellExecute(NULL, "open", filename, NULL, NULL, SW_SHOWNORMAL);
	if ( error <= (HINSTANCE)32) bOK = FALSE;
	else bOK = TRUE;

	return bOK;
}
/*--------------------------------------------------------------------------*/
