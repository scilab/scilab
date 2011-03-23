/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) DIGITEO - 2010 - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
	switch(nCmdShow)
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
	case SW_SHOWNORMAL: default:
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
	switch(CmdShow)
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

	case SW_SHOWNORMAL: default:
		normalMainWindow();
		break;
	}
}
/*--------------------------------------------------------------------------*/
