/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre Ledru
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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

#ifndef __NOGUI_H__
#define __NOGUI_H__

#ifdef _MSC_VER
#include "dynlib_gui.h"
#include "BOOL.h"

GUI_IMPEXP BOOL InitializeGUI(void);

GUI_IMPEXP int gw_gui(void);

GUI_IMPEXP BOOL TerminateGUI(void);

/**
* set Main Window visibility
* @param[in] new state
*/
GUI_IMPEXP void setVisibleMainWindow(BOOL newVisibleState);

/**
* isVisibleMainWindow
* @return current state
*/
GUI_IMPEXP BOOL isVisibleMainWindow(void);

GUI_IMPEXP void deiconifyMainWindow(void);

/**
* Iconify Main Window
*/
GUI_IMPEXP void iconifyMainWindow(void);

/**
* Maximize Main Window
*/
GUI_IMPEXP void maximizeMainWindow(void);

/**
* Main Window is in the "normal" state.
*/
GUI_IMPEXP void normalMainWindow(void);

#else

BOOL InitializeGUI(void);

/**
* This function is a wrapper / fake when Scilab is compiled without thegraphic
* graphic Interface
*
* @return 0
*/
int gw_gui(void);

/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
*
* @return TRUE
*/
BOOL TerminateGUI(void);

/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
* @param fname unused (just reproduce the profile)
* @return 0
*/

int XClearScreenConsole(char *fname);

/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
* @param string unused (just reproduce the profile)
* @param nbytes unused (just reproduce the profile)
*/
void str_to_xterm ( register char * string, int nbytes);


/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
* @param n unused (just reproduce the profile)
*/
void Click_menu(int n);


/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
* @param prop unused (just reproduce the profile)
* @param value unused (just reproduce the profile)
* @return 0
*/
int GetScreenProperty(char *prop, char *value);


/**
* Read the next character
* This function is really used here !
* @param interrupt  unused (just reproduce the profile)
* @return the result of getchar
*/
int Xorgetchar(int interrupt);


/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
* @param interrupt  unused (just reproduce the profile)
* @return 0
*/
int XEvorgetchar(int interrupt);

/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
* @param startup   unused (just reproduce the profile)
* @param lstartup  unused (just reproduce the profile)
* @param memory    unused (just reproduce the profile)
*/
void main_sci (char *startup, int lstartup, int memory);

/**
* This function is a wrapper when /fake Scilab is compiled without the
* graphic Interface
*/
void InitXsession(void);

#endif /* #ifndef _MSC_VER */

#endif /* __NOGUI_H__ */
