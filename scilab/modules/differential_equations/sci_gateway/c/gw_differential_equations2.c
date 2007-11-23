/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#ifdef _MSC_VER
#include <stdio.h>
#endif
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_differential_equations2.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sciimpl),"impl"},
	{C2F(dassli),"dassl"},
	{C2F(dasrti),"dasrt"},
};

/*--------------------------------------------------------------------------*/
int C2F(gw_differential_equations2)(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
