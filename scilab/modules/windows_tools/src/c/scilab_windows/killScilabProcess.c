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
#include <windows.h>
#include "killScilabProcess.h"
/*--------------------------------------------------------------------------*/
void killScilabProcess(void)
{
	HANDLE hProcess;

	/* Ouverture de ce Process avec droit pour le tuer */
	hProcess=OpenProcess(PROCESS_TERMINATE,FALSE,GetCurrentProcessId());
	if (hProcess)
	{
		/* Tue ce Process */
		TerminateProcess(hProcess,0);
	}
	else MessageBox(NULL,"Don't Find Scilab Process","Warning",MB_ICONWARNING);
}
/*--------------------------------------------------------------------------*/
