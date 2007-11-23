/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_data_structures1.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(error)();
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_scilist),"list"},
	{C2F(sci_scitlist),"tlist"},
	{C2F(sci_rlist),"rlist"},
	{C2F(sci_lsslist),"lsslist"},
	{C2F(sci_glist),"glist"},
	{C2F(sci_lstcat),"lstcat"},
	{C2F(sci_scimlist),"mlist"},
	{C2F(sci_definedfields),"definedfields"},
	{C2F(sci_lstsize),"lstsize"}
};
/*--------------------------------------------------------------------------*/
int C2F(gwdatastructures1)()
{ 
	Rhs=Max(Rhs,0);
	if (Top - Rhs + Lhs + 1 >= Bot) 
	{
		static integer codeerror = 18;
		C2F(error)(&codeerror);
		return 0;
	}

	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
