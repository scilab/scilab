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
#include <windows.h>
#include "InitializeWindows_tools.h"
#include "fromc.h"
/*--------------------------------------------------------------------------*/ 
BOOL InitializeWindows_tools(void)
{
	BOOL bOK = FALSE;

	HWND hScilab = GetConsoleWindow();

	if ( (hScilab) && IsFromC() )
	{
		/* force redirect stdout, stderr in console */
		freopen("CONOUT$", "wb", stdout); /* redirect stdout --> CONOUT$*/
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/ 
