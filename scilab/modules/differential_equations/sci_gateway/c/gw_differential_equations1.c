/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#ifdef _MSC_VER
	#include <stdio.h>
#endif
/*--------------------------------------------------------------------------*/
#include "gw_differential_equations1.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static int C2F(sci_none_empty) _PARAMS((char *fname,unsigned long fname_len))
{
	return 0;
}
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_ode),"ode"},
	{C2F(sci_none_empty),"sci_none_empty"},
	{C2F(sci_intg),"intg"},
	{C2F(sci_feval),"feval"},
	{C2F(sci_bvode),"bvode"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_differential_equations1)(void)
{  
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
