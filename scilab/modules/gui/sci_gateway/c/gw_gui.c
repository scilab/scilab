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
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_x_dialog),"x_dialog"},
	{C2F(sci_x_message),"x_message"},
	{C2F(sci_x_choose),"x_choose"},
	{C2F(sci_x_mdialog),"x_mdialog"},
	{C2F(sci_xchoicesi),"xchoicesi"},
	{C2F(sci_addmenu_old),"addmenu_old"},
	{C2F(sci_delmenu_old),"delmenu_old"},
	{C2F(sci_setmenu_old),"setmenu_old"},
	{C2F(sci_unsetmenu_old),"unsetmenu_old"},
	{C2F(sci_xgetfile),"xgetfile"},
	{C2F(sci_x_message_modeless),"x_message_modeless"},
 	{C2F(sci_raise_window),"raise_window"},
	{C2F(sci_getlookandfeel),"getlookandfeel"},
	{C2F(sci_getinstalledlookandfeels),"getinstalledlookandfeels"},
	{C2F(sci_setlookandfeel),"setlookandfeel"},
	{C2F(sci_about),"about"},
	{C2F(sci_ClipBoard),"ClipBoard"},
	{C2F(sci_toolbar),"toolbar"},
	{C2F(sci_hidetoolbar),"hidetoolbar"}
};
/*--------------------------------------------------------------------------*/
int C2F(gw_gui)(void)
{  
	Rhs = Max(0, Rhs);

	if ( getScilabMode() == SCILAB_NWNI)
	{
		Scierror(999,_("Module disabled in -nogui mode."));
		return 0;
	}

	callFunctionFromGateway(Tab);

	return 0;
}
/*--------------------------------------------------------------------------*/
