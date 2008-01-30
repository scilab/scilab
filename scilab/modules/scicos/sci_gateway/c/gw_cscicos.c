/*--------------------------------------------------------------------------*/
/* INRIA 2008 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_cscicos.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
	{sci_scicos_time,"scicos_time"},
	{sci_duplicate,"duplicate"},
	{sci_diffobjs,"diffobjs"},
	{sci_pointer_xproperty,"pointer_xproperty"},
	{sci_phase_simulation,"phase_simulation"},
	{sci_set_xproperty,"set_xproperty"},
	{sci_set_blockerror,"set_blockerror"},
	{sci_ctree2,"ctree2"},
	{sci_ctree3,"ctree3"},
	{sci_ctree4,"ctree4"},
	{sci_scicosim,"scicosim"},
	{sci_getscicosvars,"getscicosvars"},
	{sci_curblockc,"curblockc"},
	{sci_buildouttb,"buildouttb"},
	{sci_permutobj,"permutobj"},
	{sci_xstringb2,"xstringb2"},
	{sci_end_scicosim,"end_scicosim"}
};
/*--------------------------------------------------------------------------*/ 
int C2F(gw_cscicos)()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	C2F(putlhsvar)();
	
	return 0;
}
/*--------------------------------------------------------------------------*/ 

