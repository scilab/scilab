/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <windows.h> 
/*-----------------------------------------------------------------------------------*/ 
#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\ifconsol.lib")
#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libifcore.lib")
#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libifport.lib")
#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libm.lib")
#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libirc.lib")
/*-----------------------------------------------------------------------------------*/ 
#pragma comment(lib,"../../bin/tcl84.lib")
#pragma comment(lib,"../../bin/tk84.lib")
/*-----------------------------------------------------------------------------------*/ 
#pragma comment(lib,"../../pvm3/lib/WIN32/libpvm3.lib")
#pragma comment(lib,"../../pvm3/lib/WIN32/libgpvm3.lib")
/*-----------------------------------------------------------------------------------*/ 
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
/*-----------------------------------------------------------------------------------*/ 


