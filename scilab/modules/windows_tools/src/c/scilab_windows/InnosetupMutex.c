/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - 2008 - Allan CORNET
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
#include "InnosetupMutex.h"
#include "BOOL.h"
#include "version.h"
/*--------------------------------------------------------------------------*/
static HANDLE hMutexScilabID;
/*--------------------------------------------------------------------------*/
void createInnosetupMutex(void)
{
	/* http://www.vincenzo.net/isxkb/index.php?title=Application_considerations */
	/* creates a named mutex used by Innosetup */
	hMutexScilabID = CreateMutex (NULL, FALSE,SCI_VERSION_STRING );
}
/*--------------------------------------------------------------------------*/
void closeInnosetupMutex(void)
{
	/* close named mutex */
	CloseHandle(hMutexScilabID);
}
/*--------------------------------------------------------------------------*/
