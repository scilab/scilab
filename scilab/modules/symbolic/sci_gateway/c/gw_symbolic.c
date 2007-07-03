/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_symbolic.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_addf),"addf"},
{C2F(sci_subf),"subf"},
{C2F(sci_mulf),"mulf"},
{C2F(sci_ldivf),"ldivf"},
{C2F(sci_rdivf),"rdivf"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_symbolic)()
{  
	if (Rhs != 2)
	{
		Scierror(39,"incorrect number of arguments.\r\n");
		return 0;
	}
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
