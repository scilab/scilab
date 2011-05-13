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
#include <string.h>
#include "FindScilab.h"
#include "version.h"
#include "win_mem_alloc.h" /* MALLOC */
#include "WndThread.h"
#include "strdup_windows.h"
/*--------------------------------------------------------------------------*/
#define LineMax 255
#define NumberScilabMax 10
/*--------------------------------------------------------------------------*/
static char BeginningHiddenScilabWindow[LineMax];
static char ListScilabName[NumberScilabMax][LineMax];
static char ListHiddenScilabName[NumberScilabMax][LineMax];
static int NumberScilab = 0;
static BOOL MoreMaxNumberScilabMax = FALSE;
/*--------------------------------------------------------------------------*/
BOOL HaveAnotherWindowScilab(void)
{
	BOOL Retour=FALSE;
	HWND CurrenthWnd=NULL;
	
	wsprintf(BeginningHiddenScilabWindow,FORMAT_TITLE_HIDDEN_WINDOWS,SCI_VERSION_STRING,0);
	/* scilab-5.0 hidden window */
	BeginningHiddenScilabWindow[strlen(BeginningHiddenScilabWindow)-4] = '\0';
	
	CurrenthWnd = GetWindow(GetDesktopWindow(),GW_CHILD);
	CurrenthWnd = GetWindow(CurrenthWnd,GW_HWNDFIRST);

	while ( CurrenthWnd != NULL )
	{
		char Title[LineMax];

		GetWindowText(CurrenthWnd,Title,(int)strlen(BeginningHiddenScilabWindow)+1);
		if (strcmp(Title,BeginningHiddenScilabWindow) == 0)
		{
			GetWindowText(CurrenthWnd,Title,LineMax);
			if (NumberScilab<NumberScilabMax)
			{
				wsprintf(ListHiddenScilabName[NumberScilab],"%s",Title);
			}
			else MoreMaxNumberScilabMax = TRUE;
			NumberScilab++;
			Retour = TRUE;
		}
		CurrenthWnd = GetWindow(CurrenthWnd,GW_HWNDNEXT);
	}
	return Retour;
}
/*--------------------------------------------------------------------------*/
char * getLastScilabFound(void)
{
	return strdup(ListHiddenScilabName[0]);
}
/*--------------------------------------------------------------------------*/
char * getFirstScilabFound(void)
{
	return strdup(ListHiddenScilabName[NumberScilab-1]);
}
/*--------------------------------------------------------------------------*/