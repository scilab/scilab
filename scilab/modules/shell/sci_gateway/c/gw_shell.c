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
{C2F(sci_resethistory),"resethistory"},
{C2F(sci_displayhistory),"displayhistory"},
{C2F(sci_addhistory),"addhistory"},
{C2F(sci_saveconsecutivecommands),"saveconsecutivecommands"},
{C2F(sci_saveafterncommands),"saveafterncommands"},
{C2F(sci_gethistoryfile),"gethistoryfile"},
{C2F(sci_sethistoryfile),"sethistoryfile"},
{C2F(sci_removelinehistory),"removelinehistory"},
{C2F(sci_historysize),"historysize"}
};
/*-----------------------------------------------------------------------------------*/
int C2F(gw_shell)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*-----------------------------------------------------------------------------------*/
