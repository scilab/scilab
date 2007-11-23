/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
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
