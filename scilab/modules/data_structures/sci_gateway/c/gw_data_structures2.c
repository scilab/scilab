/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_data_structures2.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_getfield),"getfield"},
	{C2F(sci_setfield),"setfield"}
};
/*--------------------------------------------------------------------------*/
int C2F(gwdatastructures2)(void)
{ 
	Rhs=Max(Rhs,0);
	if (Top - Rhs + Lhs + 1 >= Bot) 
	{
		static integer codeerror = 18;
		Error(codeerror);
		return 0;
	}

	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
