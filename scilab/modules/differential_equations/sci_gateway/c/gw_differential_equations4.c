/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
#include <stdio.h>
#endif
#include "gw_differential_equations4.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_int2d),"int2d"},
	{C2F(sci_int3d),"int3d"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_differential_equations4)(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
