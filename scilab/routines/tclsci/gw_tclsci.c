#include "gw_tclsci.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if _MSC_VER
#include <Windows.h>
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
extern int GetWITH_GUI(void);
extern void sciprint __PARAMS((char *fmt,...));
/*-----------------------------------------------------------------------------------*/
extern int TK_Started;
/*-----------------------------------------------------------------------------------*/
extern int C2F(sci_TCL_DoOneEvent) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_EvalFile) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_EvalStr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_GetVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_SetVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_opentk) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_close) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_findobj) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_set) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_get) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_gcf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_scf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_GetVersion) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_UnsetVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_ExistVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_UpVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_DeleteInterp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_CreateSlave) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_ExistInterp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(sci_TCL_ExistArray) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
 static TCLSCITable Tab[]=
 {
  {C2F(sci_TCL_DoOneEvent),"TCL_DoOneEvent"},
  {C2F(sci_TCL_EvalFile),"TCL_EvalFile"},
  {C2F(sci_TCL_EvalStr),"TCL_EvalStr"},
  {C2F(sci_TCL_GetVar),"TCL_GetVar"},
  {C2F(sci_TCL_SetVar),"TCL_SetVar"},
  {C2F(sci_opentk),"opentk"},
  {C2F(sci_close),"close"},
  {C2F(sci_findobj),"findobj"},
  {C2F(sci_TCL_set),"%s_set"},
  {C2F(sci_TCL_get),"%s_get"},
  {C2F(sci_TCL_gcf),"TCL_gcf"},
  {C2F(sci_TCL_scf),"TCL_scf"},
  {C2F(sci_TCL_GetVersion),"TCL_GetVersion"},
  {C2F(sci_TCL_UnsetVar),"TCL_UnsetVar"},
  {C2F(sci_TCL_ExistVar),"TCL_ExistVar"},
  {C2F(sci_TCL_UpVar),"TCL_UpVar"},
  {C2F(sci_TCL_DeleteInterp),"TCL_DeleteInterp"},
  {C2F(sci_TCL_CreateSlave),"TCL_CreateSlave"},
  {C2F(sci_TCL_ExistInterp),"TCL_ExistInterp"},
  {C2F(sci_TCL_ExistArray),"TCL_ExistArray"}
 };
/*-----------------------------------------------------------------------------------*/
int C2F(gw_tclsci)()
{  
	if ( GetWITH_GUI() )
	{
		if (TK_Started)
		{
			Rhs = Max(0, Rhs);
#if _MSC_VER
#ifndef _DEBUG
			_try
			{
				(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
			}
			_except (EXCEPTION_EXECUTE_HANDLER)
			{	
				char *ExceptionString=GetExceptionString(GetExceptionCode());
				sciprint("Warning !!!\nScilab has found a critical error (%s)\nwith \"%s\" function.\nScilab may become unstable.\n",ExceptionString,Tab[Fin-1].name);
				if (ExceptionString) {FREE(ExceptionString);ExceptionString=NULL;}
			}
#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
#else
			(*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
#endif
		}
		else
		{
			sciprint(TCL_WARNING6);
		}
	}
	else
	{
		Scierror(999,"Tcl/TK interface disabled in -nogui mode.\r\n");
		return 0;
	}
	
  return 0;
}
/*-----------------------------------------------------------------------------------*/
