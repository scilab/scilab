/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "TerminateLocalization.h"
/*--------------------------------------------------------------------------*/ 
#include "localization.h"
/*--------------------------------------------------------------------------*/ 
BOOL TerminateLocalization(void)
{
	DisposeHashTableScilabErrors();
	DisposeHashTableScilabMessages();
	DisposeHashTableScilabMenus();
	return TRUE;
}
/*--------------------------------------------------------------------------*/ 
