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
#include "wmcopydata.h"
/*--------------------------------------------------------------------------*/
static char LineFromAnotherScilab[PATH_MAX];
static BOOL ReceiveDatafromAnotherScilab=FALSE;
static char TitleScilabSend[PATH_MAX];
/*--------------------------------------------------------------------------*/
BOOL GetCommandFromAnotherScilab(char *TitleWindowSend,char *CommandLine)
{
	BOOL Retour=FALSE;

	if (ReceiveDatafromAnotherScilab)
	{
		if (wsprintf(CommandLine,"%s",LineFromAnotherScilab) <= 0) return FALSE;
		wsprintf(TitleWindowSend,"%s",TitleScilabSend);

		ReceiveDatafromAnotherScilab=FALSE;
		Retour=TRUE;
	}
	else
	{
		Retour=FALSE;
	}

	return Retour;
}
/*--------------------------------------------------------------------------*/
BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource,char *ScilabWindowNameDestination,char *CommandLine)
{
   COPYDATASTRUCT MyCDS;
   MYREC MyRec;
   HWND hWndSource=NULL;
   HWND hWndDestination=NULL;
 
   if (wsprintf(MyRec.CommandFromAnotherScilab,"%s",CommandLine) <= 0) return FALSE;
 
   MyCDS.dwData = 0; 
   MyCDS.cbData = sizeof( MyRec );
   MyCDS.lpData = &MyRec;
   
   hWndSource=FindWindow(NULL,ScilabWindowNameSource);
   hWndDestination=FindWindow(NULL, ScilabWindowNameDestination);

   if ( (hWndDestination != NULL) )
   {
	     SendMessage( hWndDestination,
					  WM_COPYDATA,
					  (WPARAM)(HWND) hWndDestination,
					  (LPARAM) (LPVOID) &MyCDS );
   }
   else return FALSE;

   return TRUE;
}
/*--------------------------------------------------------------------------*/
BOOL ReceiveFromAnotherScilab(HWND hWndSend,PCOPYDATASTRUCT MyCopyDataStruct)
{
   BOOL Retour=FALSE;
 
   if (wsprintf(LineFromAnotherScilab,"%s",(LPSTR) ((MYREC *)(MyCopyDataStruct->lpData))->CommandFromAnotherScilab)  <= 0) return FALSE;
   
   GetWindowText(hWndSend,TitleScilabSend,PATH_MAX);
   ReceiveDatafromAnotherScilab=TRUE;
   Retour=TRUE;

   return Retour;
}
/*--------------------------------------------------------------------------*/
