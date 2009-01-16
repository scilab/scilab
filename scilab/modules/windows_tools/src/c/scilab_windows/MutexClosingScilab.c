/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2008 - Allan CORNET
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
#include "MALLOC.h"
#include "MutexClosingScilab.h"
#include "version.h"
/*--------------------------------------------------------------------------*/
static HANDLE hMutexClosingScilabID = NULL;
/*--------------------------------------------------------------------------*/
static char *getClosingScilabMutexName(void);
/*--------------------------------------------------------------------------*/
void createMutexClosingScilab(void)
{
	char *mutexname = getClosingScilabMutexName();
	if (mutexname)
	{
		HANDLE hMutex = NULL;
		hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexname);
		/* checks if a previous Mutex exists */
		if (!hMutex)
		{
			hMutexClosingScilabID = CreateMutex (NULL, FALSE, mutexname);
		}
		FREE(mutexname); mutexname = NULL;
	}
}
/*--------------------------------------------------------------------------*/
void terminateMutexClosingScilab(void)
{
	/* close named mutex */
	if (hMutexClosingScilabID) CloseHandle(hMutexClosingScilabID);
}
/*--------------------------------------------------------------------------*/
BOOL haveMutexClosingScilab(void)
{
	char *mutexname = getClosingScilabMutexName();
	if (mutexname)
	{
		HANDLE hMutex;
		hMutex = OpenMutex(MUTEX_ALL_ACCESS, FALSE, mutexname);
		FREE(mutexname); mutexname = NULL;
		if (hMutex) return TRUE;
	}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
static char *getClosingScilabMutexName(void)
{
	int lenmutexname = (int)(strlen(CLOSING_SCILAB_MUTEX_NAME)+strlen(SCI_VERSION_STRING)+1);
	char *mutexname = (char*)MALLOC(sizeof(char)*lenmutexname);
	if (mutexname)
	{
		strcpy(mutexname, CLOSING_SCILAB_MUTEX_NAME);
		strcat(mutexname, SCI_VERSION_STRING);
	}
	return mutexname;
}
/*--------------------------------------------------------------------------*/
