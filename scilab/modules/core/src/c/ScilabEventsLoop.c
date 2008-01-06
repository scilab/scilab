/*--------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include "ScilabEventsLoop.h"
#include "scilabmode.h"
#include "../../../tclsci/src/c/InitTclTk.h"
#include "../../../tclsci/includes/TclEvents.h"
#include "timerEvents.h"
/*--------------------------------------------------------------------------*/
int C2F (ScilabEventsLoop) (void)
{
	ScilabEventsLoop();
	return 0;
}
/*--------------------------------------------------------------------------*/
BOOL ScilabEventsLoop(void)
{
	if (scilab_timer_check())
		{
			if (isTkStarted()){
				TclEventsLoop() ;
				return TRUE;
			}
		}
	return FALSE;
}
/*--------------------------------------------------------------------------*/
