/* Allan CORNET 2004 INRIA */
#include "wmcopydata.h"

#include <strsafe.h>
#pragma comment(lib, "strsafe.lib")

static char LineFromAnotherScilab[MAX_PATH];
static BOOL ReceiveDatafromAnotherScilab=FALSE;
static char TitleScilabSend[MAX_PATH];

/*-----------------------------------------------------------------------------------*/
BOOL GetCommandFromAnotherScilab(char *TitleWindowSend,char *CommandLine)
{
	BOOL Retour=FALSE;

	if (ReceiveDatafromAnotherScilab)
	{
		HRESULT hr1,hr2;

		hr1 = StringCbPrintf(CommandLine, (strlen(LineFromAnotherScilab)+1)*sizeof(char),"%s",LineFromAnotherScilab);
		hr2 = StringCbPrintf(TitleWindowSend, (strlen(TitleScilabSend)+1)*sizeof(char),"%s",TitleScilabSend);
		if ( (hr1!= S_OK) || (hr2!= S_OK) ) return FALSE,

		ReceiveDatafromAnotherScilab=FALSE;
		Retour=TRUE;
	}
	else
	{
		Retour=FALSE;
	}

	return Retour;
}
/*-----------------------------------------------------------------------------------*/
BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource,char *ScilabWindowNameDestination,char *CommandLine)
{
   COPYDATASTRUCT MyCDS;
   MYREC MyRec;
   HWND hWndSource=NULL;
   HWND hWndDestination=NULL;
   HRESULT hResult;

   hResult = StringCbCopy( MyRec.CommandFromAnotherScilab, sizeof(MyRec.CommandFromAnotherScilab),CommandLine);
   if (hResult != S_OK)	return FALSE;

   MyCDS.dwData = 0; 
   MyCDS.cbData = sizeof( MyRec );
   MyCDS.lpData = &MyRec;
   
   hWndSource=FindWindow(NULL,ScilabWindowNameSource);
   hWndDestination=FindWindow(NULL, ScilabWindowNameDestination);

   if ( (hWndSource != NULL) && (hWndDestination != NULL) )
   {
	     SendMessage( hWndDestination,
					  WM_COPYDATA,
					  (WPARAM)(HWND) hWndDestination,
					  (LPARAM) (LPVOID) &MyCDS );
   }
   else return FALSE;

   return TRUE;
}
/*-----------------------------------------------------------------------------------*/
BOOL ReceiveFromAnotherScilab(WPARAM wParam, LPARAM lParam)
{
   BOOL Retour=FALSE;
  
   PCOPYDATASTRUCT pMyCopyDataStructure;
   HWND hWndSend=NULL;

   HRESULT hr;

   pMyCopyDataStructure = (PCOPYDATASTRUCT) lParam;

   hr = StringCbPrintf(LineFromAnotherScilab,
					(strlen((LPSTR) ((MYREC *)(pMyCopyDataStructure->lpData))->CommandFromAnotherScilab)+1)*sizeof(char),"%s",
					(LPSTR) ((MYREC *)(pMyCopyDataStructure->lpData))->CommandFromAnotherScilab);
    
   hWndSend=(HWND) wParam;
   
   if ( GetWindowText(hWndSend,TitleScilabSend,MAX_PATH) )
   {
	   ReceiveDatafromAnotherScilab=TRUE;
	   Retour=TRUE;
   }
   else Retour=FALSE;

   return Retour;
}
/*-----------------------------------------------------------------------------------*/
