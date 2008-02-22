/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/

#include "TCL_Global.h"
#include "gw_tclsci.h"
/*--------------------------------------------------------------------------*/
int sci_opentk(char *fname,unsigned long l)
{
	Tcl_Interp *TCLinterpLocal=NULL;

	CheckRhs(0,0);
	CheckLhs(1,1);

	TCLinterpLocal = Tcl_CreateInterp();
    Tcl_Init(TCLinterpLocal);
    Tk_Init(TCLinterpLocal);
    TKmainWindow = Tk_MainWindow(TCLinterpLocal);
    Tk_GeometryRequest(TKmainWindow,200,200);
    Tk_SetWindowBackground(TKmainWindow, WhitePixelOfScreen(Tk_Screen(TKmainWindow)));

	LhsVar(1) = 0;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
