/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_data_structures2.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(error)();
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_getfield),"getfield"},
	{C2F(sci_setfield),"setfield"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gwdatastructures2)(void)
{ 
	Rhs=Max(Rhs,0);
	if (C2F(vstk).top - C2F(com).rhs + C2F(com).lhs + 1 >= C2F(vstk).bot) 
	{
		static integer codeerror = 18;
		C2F(error)(&codeerror);
		return 0;
	}

	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
