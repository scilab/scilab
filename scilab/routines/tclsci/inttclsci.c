#include "inttclsci.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#if WIN32
#include <Windows.h>
#include "../os_specific/win_mem_alloc.h"
extern char *GetExceptionString(DWORD ExceptionCode);
#endif
extern int GetWITH_GUI(void);
/*-----------------------------------------------------------------------------------*/
extern int TK_Started;
/*-----------------------------------------------------------------------------------*/
extern initTCLTK(void);
extern int ReInitTCL(void);
/*-----------------------------------------------------------------------------------*/
extern int C2F(intTclDoOneEvent) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclEvalFile) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclEvalStr) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclGetVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclSetVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intOpenTk) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intClose) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intFindObj) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclSet) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclGet) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclGcf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclScf) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclGetVersion) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclUnsetVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclExistVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclUpVar) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclDeleteInterp) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclCreateSlave) _PARAMS((char *fname,unsigned long fname_len));
extern int C2F(intTclExistInterp) _PARAMS((char *fname,unsigned long fname_len));
/*-----------------------------------------------------------------------------------*/
 static TCLSCITable Tab[]=
 {
  {C2F(intTclDoOneEvent),"TCL_DoOneEvent"},
  {C2F(intTclEvalFile),"TCL_EvalFile"},
  {C2F(intTclEvalStr),"TCL_EvalStr"},
  {C2F(intTclGetVar),"TCL_GetVar"},
  {C2F(intTclSetVar),"TCL_SetVar"},
  {C2F(intOpenTk),"opentk"},
  {C2F(intClose),"close"},
  {C2F(intFindObj),"findobj"},
  {C2F(intTclSet),"%s_set"},
  {C2F(intTclGet),"%s_get"},
  {C2F(intTclGcf),"TCL_gcf"},
  {C2F(intTclScf),"TCL_scf"},
  {C2F(intTclGetVersion),"TCL_GetVersion"},
  {C2F(intTclUnsetVar),"TCL_UnsetVar"},
  {C2F(intTclExistVar),"TCL_ExistVar"},
  {C2F(intTclUpVar),"TCL_UpVar"},
  {C2F(intTclDeleteInterp),"TCL_DeleteInterp"},
	{C2F(intTclCreateSlave),"TCL_CreateSlave"},
	{C2F(intTclExistInterp),"TCL_ExistInterp"}
 };
/*-----------------------------------------------------------------------------------*/
int C2F(inttclsci)()
{ 

	if ( GetWITH_GUI() )
	{ 
	Rhs = Max(0, Rhs);
	ReInitTCL();  
	#if WIN32
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
  	Scierror(999,"Tcl/TK interface disabled in -nogui mode.\r\n");
		return 0;
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
