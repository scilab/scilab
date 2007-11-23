/*--------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_completion.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_completion),"completion"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_completion)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);

	return 0;
}
/*--------------------------------------------------------------------------*/
