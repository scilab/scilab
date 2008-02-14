/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_gui.h"
#include "stack-c.h"
#include "scilabmode.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_x_dialog),"x_dialog"},
	{C2F(sci_x_message),"x_message"},
	{C2F(sci_x_choose),"x_choose"},
	{C2F(sci_x_mdialog),"x_mdialog"},
	{C2F(sci_xchoicesi),"xchoicesi"},
	{sci_delmenu,"delmenu"},
	{sci_setmenu,"setmenu"},
	{sci_unsetmenu,"unsetmenu"},
	{C2F(sci_xgetfile),"xgetfile"},
	{C2F(sci_x_message_modeless),"x_message_modeless"},
 	{C2F(sci_raise_window),"raise_window"},
	{C2F(sci_getlookandfeel),"getlookandfeel"},
	{C2F(sci_getinstalledlookandfeels),"getinstalledlookandfeels"},
	{C2F(sci_setlookandfeel),"setlookandfeel"},
	{C2F(sci_about),"about"},
	{C2F(sci_ClipBoard),"ClipBoard"},
	{C2F(sci_toolbar),"toolbar"},
	{C2F(sci_hidetoolbar),"hidetoolbar"},
	{sci_uigetdir,"uigetdir"},
	{sci_uicontrol,"uicontrol"},
	{sci_uimenu,"uimenu"},
	{sci_mpopup,"uipopup"},
	{C2F(sci_x_choose_modeless),"x_choose_modeless"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_gui)(void)
{  
	Rhs = Max(0, Rhs);

	if ( getScilabMode() == SCILAB_NWNI)
	{
		Scierror(999,_("Scilab graphic module disabled -nogui or -nwni mode."));
		return 0;
	}

	callFunctionFromGateway(Tab);

	return 0;
}
/*--------------------------------------------------------------------------*/
