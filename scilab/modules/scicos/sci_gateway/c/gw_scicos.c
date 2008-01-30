/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_scicos.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
{C2F(sci_var2vec),"var2vec"},
{C2F(sci_vec2var),"vec2var"},
{C2F(sci_getblocklabel),"getblocklabel"},
{C2F(sci_scicos_debug),"scicos_debug"},
{C2F(sci_scicos_debug_count),"scicos_debug_count"},
{C2F(sci_sctree),"sctree"},
{C2F(sci_sci_tree2),"sci_tree2"},
{C2F(sci_sci_tree3),"sci_tree3"},
{C2F(sci_sci_tree4),"sci_tree4"},
{C2F(sci_curblock),"curblock"}
};
/*--------------------------------------------------------------------------*/ 
int C2F(gw_scicos)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/ 

