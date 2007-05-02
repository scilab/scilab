/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "TclEvents.h"
#include "TCL_Global.h"
#include "scilabmode.h"
/*-----------------------------------------------------------------------------------*/ 
void flushTKEvents(void)
{
	if( getScilabMode() != SCILAB_NWNI ) 
	{
		while (Tcl_DoOneEvent(TCL_ALL_EVENTS | TCL_DONT_WAIT)==1)
		{
		}
	}
}
/*-----------------------------------------------------------------------------------*/
int tcl_check_one_event(void) 
{
	int bRes=0;

	if( getScilabMode() != SCILAB_NWNI ) 
	{
		bRes=Tcl_DoOneEvent ( TCL_DONT_WAIT);
	}
	return bRes;
}
/*-----------------------------------------------------------------------------------*/
int TclEventsLoop(void)
{
	return (Tcl_DoOneEvent (TCL_ALL_EVENTS | TCL_DONT_WAIT) == 1);
}
/*-----------------------------------------------------------------------------------*/
