/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <stdio.h>
#endif
#include "gw_differential_equations5.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_feval),"feval"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_differential_equations5)(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
