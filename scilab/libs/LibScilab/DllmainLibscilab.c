/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <windows.h> 
#ifdef USE_F2C
	#include "f2c.h"
#endif
/*-----------------------------------------------------------------------------------*/ 
#pragma comment(lib,"../../bin/libxml2.lib")
#pragma comment(lib,"../../bin/libintl.lib")
/*-----------------------------------------------------------------------------------*/ 
/* force linking fortran libraries */
#pragma comment(lib,"../../bin/blasplus.lib")
#pragma comment(lib,"../../bin/lapack.lib")
#pragma comment(lib,"../../bin/linear_algebra_f.lib")
#pragma comment(lib,"../../bin/sparse_f.lib")
#pragma comment(lib,"../../bin/symbolic_f.lib")
#pragma comment(lib,"../../bin/slicot_f.lib")
#pragma comment(lib,"../../bin/slatec_f.lib")
#pragma comment(lib,"../../bin/string_f.lib")
#pragma comment(lib,"../../bin/io_f.lib")
#pragma comment(lib,"../../libs/data_structures_f.lib")
#pragma comment(lib,"../../libs/boolean_f.lib")
#pragma comment(lib,"../../libs/double_f.lib")
#pragma comment(lib,"../../libs/differential_equations_f.lib")
#pragma comment(lib,"../../libs/special_functions_f.lib")
#pragma comment(lib,"../../libs/elementary_functions_f.lib")
#pragma comment(lib,"../../libs/core_f.lib")
#pragma comment(lib,"../../libs/cacsd_f.lib")
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
BOOL BuildWithVS8ExpressF2C(void)
{
	#ifdef USE_F2C
		return TRUE;
	#else
		return FALSE;
	#endif
}
/*-----------------------------------------------------------------------------------*/ 

