/*--------------------------------------------------------------------------*/
/* Allan CORNET 2004 INRIA */
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
