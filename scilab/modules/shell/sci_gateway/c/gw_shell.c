/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_shell.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_completion),"completion"},
{C2F(sci_clc),"clc"},
{C2F(sci_tohome),"tohome"},
{C2F(sci_loadhistory),"loadhistory"},
{C2F(sci_savehistory),"savehistory"},
{C2F(sci_gethistory),"gethistory"},
{C2F(sci_resethistory),"resethistory"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_shell)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
