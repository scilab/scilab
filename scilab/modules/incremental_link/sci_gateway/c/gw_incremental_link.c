/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_incremental_link.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_getdynlibext),"getdynlibext"},
	{C2F(sci_addinter),"addinter"},
	{C2F(sci_fort),"fort"},
	{C2F(sci_call),"call"},
	{C2F(sci_link),"link"},
	{C2F(sci_ulink),"ulink"},
	{C2F(sci_c_link),"c_link"},
};
/*--------------------------------------------------------------------------*/
int gw_dynamic_link()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
