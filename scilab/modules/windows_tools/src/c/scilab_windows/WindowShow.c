/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
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
/*--------------------------------------------------------------------------*/
#include <windows.h>
#include "WindowShow.h"
#include "iconifyMainWindow.h"
#include "setVisibleMainWindow.h"
/*--------------------------------------------------------------------------*/
static int CmdShow = -1;
/*--------------------------------------------------------------------------*/
void setWindowShowMode(int nCmdShow)
{
    switch (nCmdShow)
    {
        case SW_HIDE:
        case SW_SHOWMINIMIZED:
        case SW_SHOWMAXIMIZED:
        case SW_SHOWNOACTIVATE:
        case SW_SHOW:
        case SW_MINIMIZE:
        case SW_SHOWMINNOACTIVE:
        case SW_SHOWNA:
        case SW_RESTORE:
        case SW_SHOWDEFAULT:
        case SW_MAX:
            CmdShow = nCmdShow;
            break;
        case SW_SHOWNORMAL:
        default:
            CmdShow = SW_NORMAL;
            break;
    }
}
/*--------------------------------------------------------------------------*/
int getWindowShowMode(void)
{
    return CmdShow;
}
/*--------------------------------------------------------------------------*/
void WindowShow(void)
{
    switch (CmdShow)
    {
        case SW_HIDE:
            setVisibleMainWindow(FALSE);
            break;

        case SW_SHOWMINIMIZED:
        case SW_MINIMIZE:
        case SW_SHOWMINNOACTIVE:
            iconifyMainWindow();
            break;

        case SW_SHOWMAXIMIZED:
        case SW_MAX:
            maximizeMainWindow();
            break;

        case SW_SHOWNORMAL:
        default:
            normalMainWindow();
            break;
    }
}
/*--------------------------------------------------------------------------*/
