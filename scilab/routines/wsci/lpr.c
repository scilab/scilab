/***********************************************************************/
/* Copyright (C) 2005 INRIA Allan CORNET */
/***********************************************************************/
#include <Windows.h>
/***********************************************************************/
typedef int (*MYPROC) (HINSTANCE , HWND , char *, char *); 
/***********************************************************************/
int main (int argc, char *argv[])
{
    HINSTANCE hinstLib; 
    MYPROC gp_printfile; 
    BOOL fFreeResult, fRunTimeLinkSuccess = FALSE; 
    
    hinstLib = LoadLibrary(TEXT("Libscilab")); 	
 
    if (hinstLib != NULL) 
    { 
        gp_printfile = (MYPROC) GetProcAddress(hinstLib, TEXT("gp_printfile")); 
 
        if (NULL != gp_printfile) 
        {
		HWND parent;
		HANDLE x = GetModuleHandleA (0);
						
		fRunTimeLinkSuccess = TRUE;
		parent = GetActiveWindow ();
						
		if (argc == 2)
			(gp_printfile) (x, parent, argv[1], (char *) 0);
		else
			(gp_printfile) (x, parent, (char *) 0, (char *) 0);
        }

        fFreeResult = FreeLibrary(hinstLib); 
    } 
 
    if (! fRunTimeLinkSuccess) MessageBox(NULL,"Lpr : Libscilab.dll not found !","Warning",MB_ICONERROR); 
	
    return (0);
}
/***********************************************************************/
