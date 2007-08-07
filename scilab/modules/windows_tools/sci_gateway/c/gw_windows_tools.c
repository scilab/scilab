/*-----------------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*-----------------------------------------------------------------------------------*/
#include <string.h>
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*-----------------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_winopen),"winopen"},
{C2F(sci_winqueryreg),"winqueryreg"},
{C2F(sci_findfileassociation),"findfileassociation"},
{C2F(sci_dos),"dos"},
{C2F(sci_mcisendstring),"mcisendstring"},
{C2F(sci_oemtochar),"oemtochar"},
{C2F(sci_chartooem),"chartooem"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_windows_tools)(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
