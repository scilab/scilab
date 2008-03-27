/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

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
	{C2F(sci_x_choose),"x_choose"},
	{C2F(sci_x_mdialog),"x_mdialog"},
	{C2F(sci_x_choice),"xchoicesi"},
	{sci_delmenu,"delmenu"},
	{sci_setmenu,"setmenu"},
	{sci_unsetmenu,"unsetmenu"},
	{C2F(sci_xgetfile),"xgetfile"},
 	{C2F(sci_raise_window),"raise_window"},
	{C2F(sci_getlookandfeel),"getlookandfeel"},
	{C2F(sci_getinstalledlookandfeels),"getinstalledlookandfeels"},
	{C2F(sci_setlookandfeel),"setlookandfeel"},
	{C2F(sci_ClipBoard),"ClipBoard"},
	{sci_toolbar,"toolbar"},
	{sci_uigetdir,"uigetdir"},
	{sci_uicontrol,"uicontrol"},
	{sci_uimenu,"uimenu"},
	{sci_mpopup,"uipopup"},
	{C2F(sci_x_choose_modeless),"x_choose_modeless"},
	{sci_uicontextmenu,"uicontextmenu"},
	{sci_uiwait,"uiwait"},
	{sci_messagebox,"messagebox"},
	{sci_waitbar,"waitbar"},
	{sci_progressionbar,"progressionbar"},
	{sci_helpbrowser,"helpbrowser"},
	{sci_uigetfont,"uigetfont"},
	{sci_uigetcolor,"uigetcolor"}
};
/*--------------------------------------------------------------------------*/
int gw_gui(void)
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
