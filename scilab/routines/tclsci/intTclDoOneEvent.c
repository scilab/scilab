/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclDoOneEvent.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclDoOneEvent) _PARAMS((char *fname))
{
	CheckRhs(0,0);
	CheckLhs(1,1);

	/* wait for events and invoke event handlers */
	Tcl_DoOneEvent(TCL_ALL_EVENTS | TCL_DONT_WAIT);

	LhsVar(1) = 0;
	C2F(putlhsvar)();	
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
