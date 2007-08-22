/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "ScilabEventsLoop.h"
#include "scilabmode.h"
#include "../../../tclsci/includes/TclEvents.h"
#include "timer.h"
/*-----------------------------------------------------------------------------------*/
int C2F (ScilabEventsLoop) (void)
{
	ScilabEventsLoop();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
BOOL ScilabEventsLoop(void)
{
	BOOL bOK = FALSE;
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (scilab_timer_check())
		{
			flushTKEvents ();
			TclEventsLoop() ;
			bOK = TRUE;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
