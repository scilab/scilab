/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/ 
#define  _WIN32_WINNT 0x0500
#include <Windows.h>
#include <stdio.h>
#include "WinConsole.h"
#include "version.h"
#include "scilabmode.h"
#include "MALLOC.h"
#include "scilab_windows/console.h"
/*--------------------------------------------------------------------------*/
static int Windows_Console_State = 0;/* 0 Hide 1 Show */
/*--------------------------------------------------------------------------*/
/*Cache la fenetre Scilex(x) de ce processus */
void HideScilex(void)
{
	HWND hScilex = NULL;
	hScilex = GetConsoleWindow();
	if (hScilex) ShowWindow(hScilex,SW_HIDE);	
}
/*--------------------------------------------------------------------------*/
/*Montre la fenetre Scilex(x) de ce processus */
void ShowScilex(void)
{
  HWND hScilex = NULL;
  hScilex=GetConsoleWindow();
  if (hScilex) ShowWindow(hScilex,SW_SHOWNOACTIVATE);
}
/*--------------------------------------------------------------------------*/
void SwitchConsole(void)
{
	char *ConsoleName =  getScilexConsoleName();

	switch (GetConsoleState())
	{
		/* La fenetre etait cachée , on la restaure */
		case 0:
		{
			ShowScilex();
			SetConsoleState(1);
		}
		break;
		/* La fenetre etait apparente , on la cache */ 
		case 1:
		{
			HideScilex();
			SetConsoleState(0);
		}
		break;
	}

	if (ConsoleName)
	{
		SetConsoleTitle(ConsoleName);
		FREE(ConsoleName);
		ConsoleName = NULL;
	}
}
/*--------------------------------------------------------------------------*/
int GetConsoleState(void)
{
	return Windows_Console_State;
}
/*--------------------------------------------------------------------------*/
void SetConsoleState(int state)
{
	Windows_Console_State=state;
}
/*--------------------------------------------------------------------------*/
