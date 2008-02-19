/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "sciprint.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_tclsci.h"
#include "stack-c.h"
#include "inisci-c.h"
#include "scilabmode.h"
#include "callFunctionFromGateway.h"
#include "InitTclTk.h" /* isTkStarted */
/*--------------------------------------------------------------------------*/
 static gw_generic_table Tab[]=
 {
  {C2F(sci_TCL_DoOneEvent),"TCL_DoOneEvent"},
  {C2F(sci_TCL_EvalFile),"TCL_EvalFile"},
  {C2F(sci_TCL_EvalStr),"TCL_EvalStr"},
  {C2F(sci_TCL_GetVar),"TCL_GetVar"},
  {C2F(sci_TCL_SetVar),"TCL_SetVar"},
  {C2F(sci_opentk),"opentk"},
  {C2F(sci_TCL_set),"%s_set"},
  {C2F(sci_TCL_get),"%s_get"},
  {C2F(sci_TCL_gcf),"TCL_gcf"},
  {C2F(sci_TCL_scf),"TCL_scf"},
  {C2F(sci_TCL_GetVersion),"TCL_GetVersion"},
  {C2F(sci_TCL_UnsetVar),"TCL_UnsetVar"},
  {C2F(sci_TCL_ExistVar),"TCL_ExistVar"},
  {C2F(sci_TCL_UpVar),"TCL_UpVar"},
  {C2F(sci_TCL_DeleteInterp),"TCL_DeleteInterp"},
  {C2F(sci_TCL_CreateSlave),"TCL_CreateSlave"},
  {C2F(sci_TCL_ExistInterp),"TCL_ExistInterp"},
  {C2F(sci_TCL_ExistArray),"TCL_ExistArray"}
 };
/*--------------------------------------------------------------------------*/
int gw_tclsci(void)
{  
	if ( getScilabMode() != SCILAB_NWNI )
	{
		if (isTkStarted())
		{
			Rhs = Max(0, Rhs);
			callFunctionFromGateway(Tab);		
		}
		else
		{
			sciprint(_("Warning: Problem(s) with TCL/TK interface. Interface not enabled.\n"));
		}
	}
	else
	{
		Scierror(999,_("Tcl/TK interface disabled in -nogui mode.\n"));
		return 0;
	}
	
  return 0;
}
/*--------------------------------------------------------------------------*/
