/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_optim.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_optim),"optim"},
{C2F(sciquapro),"qpqpqp"}, /* undocumented primitive used by quapro macro*/
{C2F(sci_semidef),"semidef"},
{C2F(sci_fsolve),"fsolve"},
{C2F(sci_sqrsolve),"sqrsolve"},
{C2F(sci_qld),"qld"},
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_optim)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
