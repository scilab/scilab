/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <windows.h> 
#include "tcl.h"
/*--------------------------------------------------------------------------*/ 
#pragma comment(lib,"../../bin/intl.lib")
/*--------------------------------------------------------------------------*/ 
#if defined(TCL_MAJOR_VERSION) && defined(TCL_MAJOR_VERSION)
	#if TCL_MAJOR_VERSION == 8 
		#if TCL_MINOR_VERSION == 4
			#pragma comment(lib,"../../bin/tcl84.lib")
			#pragma comment(lib,"../../bin/tk84.lib")
		#else
			#if TCL_MINOR_VERSION == 5
				#pragma comment(lib,"../../bin/tcl85.lib")
				#pragma comment(lib,"../../bin/tk85.lib")
			#else
				#pragma message ("TCL/TK 8.4 or more required.")
			#endif
		#endif
	#else
		#pragma message ("TCL/TK 8.4 or more required.")
	#endif
#endif
/*--------------------------------------------------------------------------*/ 
int WINAPI DllMain (HINSTANCE hInstance , DWORD reason, PVOID pvReserved)
{
	switch (reason) 
	{
	case DLL_PROCESS_ATTACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	}
	return 1;
}
/*--------------------------------------------------------------------------*/ 
