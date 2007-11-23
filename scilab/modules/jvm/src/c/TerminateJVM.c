/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "TerminateJVM.h"
#include "JVM.h"
#include "createMainScilabObject.h"
/*--------------------------------------------------------------------------*/ 
BOOL TerminateJVM(void)
{
	BOOL bOK=FALSE;

	finishMainScilabObject();
	bOK=finishJVM();

	

	return bOK;
}
/*--------------------------------------------------------------------------*/ 
