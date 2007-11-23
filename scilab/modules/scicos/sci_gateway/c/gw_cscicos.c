/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_cscicos.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/ 
static gw_generic_table Tab[]={
  {inttimescicos,"scicos_time"},
  {intduplicate,"duplicate"},
  {intdiffobjs,"diffobjs"},
  {intxproperty,"pointer_xproperty"},
  {intphasesim,"phase_simulation"},
  {intsetxproperty,"set_xproperty"},
  {intcpass2,"scicos_cpass2"},
  {intsetblockerror,"set_blockerror"},
  {inttree2,"ctree2"},
  {inttree3,"ctree3"},
  {inttree4,"ctree4"},
  {intscicosimc,"scicosim"},
  {intgetscicosvarsc,"getscicosvars"},
  {intcurblkc,"curblockc"},
  {intbuildouttb,"buildouttb"},
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
