/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_symbolic.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "Scierror.h"
#include "localization.h"
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
		Scierror(39,_("Incorrect number of input arguments.\n"));
		return 0;
	}
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
