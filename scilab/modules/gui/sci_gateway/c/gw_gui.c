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
#include "sci_mode.h"
#include "api_scilab.h"
#include "MALLOC.h"
#include "callFunctionFromGateway.h"
#include "localization.h"
#include "Scierror.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{sci_x_dialog,"x_dialog"},
	{sci_x_choose,"x_choose"},
	{sci_x_mdialog,"x_mdialog"},
	{sci_x_choice,"xchoicesi"},
	{sci_delmenu,"delmenu"},
	{sci_setmenu,"setmenu"},
	{sci_unsetmenu,"unsetmenu"},
 	{sci_raise_window,"raise_window"},
	{sci_getlookandfeel,"getlookandfeel"},
	{sci_getinstalledlookandfeels,"getinstalledlookandfeels"},
	{sci_setlookandfeel,"setlookandfeel"},
	{sci_ClipBoard,"ClipBoard"},
	{sci_toolbar,"toolbar"},
	{sci_uigetdir,"uigetdir"},
	{sci_uicontrol,"uicontrol"},
	{sci_uimenu,"uimenu"},
	{sci_mpopup,"uipopup"},
	{sci_x_choose_modeless,"x_choose_modeless"},
	{sci_uicontextmenu,"uicontextmenu"},
	{sci_uiwait,"uiwait"},
	{sci_messagebox,"messagebox"},
	{sci_waitbar,"waitbar"},
	{sci_progressionbar,"progressionbar"},
	{NULL, ""}, // helpbrowser
	{sci_uigetfont,"uigetfont"},
	{sci_uigetcolor,"uigetcolor"},
	{sci_getcallbackobject, "getcallbackobject"},
	{sci_printfigure, "printfigure"},
	{sci_exportUI, "exportUI"},
	{sci_printsetupbox,"printsetupbox"},
	{NULL, ""}, // toprint
	{sci_uigetfile,"uigetfile"},
	{sci_usecanvas,"usecanvas"},
	{sci_displaytree,"uiDisplayTree"},
	{sci_uiputfile,"uiputfile"},
    {NULL, ""} // about
};
/*--------------------------------------------------------------------------*/
int gw_gui(void)
{
	return 0;
}
/*--------------------------------------------------------------------------*/
