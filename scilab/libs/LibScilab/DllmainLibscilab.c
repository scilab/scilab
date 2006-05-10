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


