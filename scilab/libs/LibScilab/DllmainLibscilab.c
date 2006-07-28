/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <windows.h> 
/*-----------------------------------------------------------------------------------*/ 
#ifndef USE_F2C
	#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\ifconsol.lib")
	#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libifport.lib")
	#if _DEBUG
		#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libifcoremdd.lib")
		#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libmmdd.lib")
	#else
		#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libifcoremd.lib")
		#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libmmd.lib")
	#endif
	#pragma comment(lib,"C:\\PROGRA~1\\Intel\\Compiler\\Fortran\\9.1\\IA32\\Lib\\libirc.lib")
#endif
/*-----------------------------------------------------------------------------------*/ 
#if WITH_TK
#pragma comment(lib,"../../bin/tcl84.lib")
#pragma comment(lib,"../../bin/tk84.lib")
#endif
/*-----------------------------------------------------------------------------------*/ 
#if WITH_PVM
#pragma comment(lib,"../../pvm3/lib/WIN32/libpvm3.lib")
#pragma comment(lib,"../../pvm3/lib/WIN32/libgpvm3.lib")
#pragma comment(lib,"../../libs/pvm_f.lib")
#endif
/*-----------------------------------------------------------------------------------*/ 
#pragma comment(lib,"../../bin/atlas.lib")
#pragma comment(lib,"../../bin/arpack.lib")
#pragma comment(lib,"../../bin/lapack.lib")
/*-----------------------------------------------------------------------------------*/ 
#ifndef USE_F2C
#pragma comment(lib,"../../libs/calelm_f.lib")
#pragma comment(lib,"../../libs/control_f.lib")
#pragma comment(lib,"../../libs/dcd_f.lib")
#pragma comment(lib,"../../libs/core_f.lib")
#pragma comment(lib,"../../libs/io_f.lib")
#pragma comment(lib,"../../libs/integ_f.lib")
#pragma comment(lib,"../../libs/interf_f.lib")
#pragma comment(lib,"../../libs/intersci_f.lib")
#pragma comment(lib,"../../libs/int_f.lib")
#pragma comment(lib,"../../libs/metanet_f.lib")
#pragma comment(lib,"../../libs/optim_f.lib")
#pragma comment(lib,"../../libs/os_specific_f.lib")
#pragma comment(lib,"../../libs/poly_f.lib")
#pragma comment(lib,"../../libs/randlib_f.lib")
#pragma comment(lib,"../../libs/scicos_f.lib")
#pragma comment(lib,"../../libs/signal_f.lib")
#pragma comment(lib,"../../libs/slicot_f.lib")
#pragma comment(lib,"../../libs/sparse_f.lib")
#endif
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
	#if USE_F2C
		return TRUE;
	#else
		return FALSE;
	#endif
}
/*-----------------------------------------------------------------------------------*/ 

