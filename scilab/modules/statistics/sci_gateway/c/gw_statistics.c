/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_statistics.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{ 
	{cdfbetI, "cdfbet"},
	{cdfbinI, "cdfbin"},
	{cdfchiI, "cdfchi"},
	{cdfchnI, "cdfchn"},
	{cdffI, "cdff"},
	{cdffncI, "cdffnc"},
	{cdfgamI, "cdfgam"},
	{cdfnbnI, "cdfnbn"},
	{cdfnorI, "cdfnor"},
	{cdfpoiI, "cdfpoi"},
	{cdftI, "cdft"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_statistics)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
