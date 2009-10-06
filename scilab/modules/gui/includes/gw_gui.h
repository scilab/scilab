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

#ifndef __GW_GUI__
#define __GW_GUI__
#include "dynlib_gui.h"
#include "api_scilab.h"
/*--------------------------------------------------------------------------*/
GUI_IMPEXP int gw_gui(void);
/*--------------------------------------------------------------------------*/
GUI_IMPEXP int sci_x_dialog(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_x_choose(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_x_mdialog(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_x_choice(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_delmenu(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_setmenu(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_unsetmenu(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_uigetdir(char *fname,unsigned long l);
GUI_IMPEXP int sci_raise_window ( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_getlookandfeel( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_getinstalledlookandfeels( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_setlookandfeel( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_ClipBoard(char *fname,unsigned long l);
GUI_IMPEXP int sci_toolbar(char *fname,unsigned long l);
GUI_IMPEXP int sci_uicontrol(char *fname,unsigned long l);
GUI_IMPEXP int sci_uimenu(char *fname,unsigned long l);
GUI_IMPEXP int sci_x_choose_modeless(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_messagebox(char *fname,unsigned long l);
GUI_IMPEXP int sci_waitbar(char *fname,unsigned long l);
GUI_IMPEXP int sci_progressionbar(char *fname,unsigned long l);
GUI_IMPEXP int sci_helpbrowser(char *fname,unsigned long l);
GUI_IMPEXP int sci_uigetfont(char *fname,unsigned long l);
GUI_IMPEXP int sci_uigetcolor(char *fname,unsigned long l);
GUI_IMPEXP int sci_getcallbackobject(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_uicontextmenu(char * fname, unsigned long fname_len) ;
GUI_IMPEXP int sci_uiwait( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_mpopup( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_printfigure(char *fname,unsigned long fname_len);
GUI_IMPEXP int sci_exportUI( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_printsetupbox(char *fname,unsigned long l);
GUI_IMPEXP int sci_toprint(char *fname,unsigned long l);
GUI_IMPEXP int sci_uigetfile( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_usecanvas(char *fname,unsigned long l);
GUI_IMPEXP int sci_displaytree(char *fname,unsigned long l);
GUI_IMPEXP int sci_uiputfile( char * fname, unsigned long fname_len ) ;
GUI_IMPEXP int sci_raise_window ( char * fname, unsigned long fname_len ) ;
/*--------------------------------------------------------------------------*/
#endif /*  __GW_GUI__ */
/*--------------------------------------------------------------------------*/

