/*
 * Scilab (http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2008 - INRIA - Vincent COUVERT
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

#ifndef __GW_GUI__
#define __GW_GUI__
#include "dynlib_gui.h"
/*--------------------------------------------------------------------------*/
GUI_IMPEXP int gw_gui(void);
/*--------------------------------------------------------------------------*/
GUI_IMPEXP int sci_x_dialog(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_x_choose(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_x_mdialog(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_x_choice(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_delmenu(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_setmenu(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_unsetmenu(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uigetdir(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_getlookandfeel(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_getinstalledlookandfeels(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_setlookandfeel(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_ClipBoard(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_toolbar(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uicontrol(char* fname, void *pvApiCtx);
GUI_IMPEXP int sci_uimenu(char* fname, void *pvApiCtx);
GUI_IMPEXP int sci_x_choose_modeless(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_messagebox(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_waitbar(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_progressionbar(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uigetfont(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uigetcolor(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_getcallbackobject(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uicontextmenu(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uiwait(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_printfigure(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_exportUI(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_printsetupbox(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_toprint(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uigetfile(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_usecanvas(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_uiputfile(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_raise_window (char* fname, void* pvApiCtx);

GUI_IMPEXP int sci_useeditor(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_openged(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatipcreate(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatipremove(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_plotbrowser(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatip_manager_mode(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatip_toggle(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatipmove(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatip_set_style(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatip_set_display(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatip_set_interp(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_datatip_set_orient(char *fname, void* pvApiCtx);

// Scilab 6
GUI_IMPEXP int sci_about(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_mpopup(char* fname, void* pvApiCtx);
GUI_IMPEXP int sci_fire_closing_finished (char* fname, void* pvApiCtx);

GUI_IMPEXP int sci_figure(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_saveGui(char *fname, void* pvApiCtx);
GUI_IMPEXP int sci_loadGui(char *fname, void* pvApiCtx);

/*--------------------------------------------------------------------------*/
#endif /*  __GW_GUI__ */
/*--------------------------------------------------------------------------*/

