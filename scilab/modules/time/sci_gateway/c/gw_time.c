/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "gw_time.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]={
    {C2F(sci_getdate),"getdate"},
	{C2F(sci_calendar),"Calendar"},
	{C2F(sci_timer),"timer"},
	{C2F(sci_sleep),"sleep"},
	{C2F(sci_xpause),"xpause"},
	{C2F(sci_realtimeinit),"realtimeinit"},
	{C2F(sci_realtime),"realtime"},
};
/*--------------------------------------------------------------------------*/ 
int C2F(gw_time)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/ 
