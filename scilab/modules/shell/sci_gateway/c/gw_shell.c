/*--------------------------------------------------------------------------*/
/**
 * @author Allan CORNET INRIA 2007
 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_shell.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_clc),"clc"},
{C2F(sci_tohome),"tohome"},
{C2F(sci_lines),"lines"},
{C2F(sci_prompt),"prompt"},
{sci_iswaitingforinput,"iswaitingforinput"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_shell)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
