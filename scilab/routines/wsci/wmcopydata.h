/* Allan CORNET 2004 INRIA */

#ifndef __WMCOPYDATA__
#define __WMCOPYDATA__

#include <windows.h>

typedef struct tagMYREC
{
   char  CommandFromAnotherScilab[MAX_PATH];
} 
MYREC;


BOOL SendCommandToAnotherScilab(char *ScilabWindowNameSource,char *ScilabWindowNameDestination,char *CommandLine);
BOOL GetCommandFromAnotherScilab(char *TitleWindowSend,char *CommandLine);
BOOL ReceiveFromAnotherScilab(WPARAM wParam, LPARAM lParam); 
/*see wtext.c case WM_COPYDATA: in CALLBACK WndParentProc */

#endif /*__WMCOPYDATA__*/