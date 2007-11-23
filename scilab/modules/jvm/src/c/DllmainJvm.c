/*--------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/ 
#include <windows.h> 
/*--------------------------------------------------------------------------*/ 
#pragma comment(lib,"../../bin/libxml2.lib")
#pragma comment(lib,"../../bin/intl.lib")
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

