#include "inttclsci.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
extern int TK_Started;
/*-----------------------------------------------------------------------------------*/
extern void sci_tk_activate(void);
extern initTCLTK(void);
/*-----------------------------------------------------------------------------------*/
static int first =0;
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
  {C2F(intTclUpVar),"TCL_UpVar"}
 };
/*-----------------------------------------------------------------------------------*/
int C2F(inttclsci)()
{  
  Rhs = Max(0, Rhs);
  ReInitTCL();  
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  return 0;
}
/*-----------------------------------------------------------------------------------*/
int ReInitTCL(void)
{
  if (TK_Started != 1 )
  {
      if ( first == 0) 
	  {
	    sci_tk_activate();
		first++;
	    if ( TK_Started != 1 ) 
	    {
		  initTCLTK();
		  /* Derniere chance ;) d'initialisation */
		  if ( TK_Started != 1 ) 
	      {
	        Scierror(999,"You have started Scilab in a mode in which TCL/TK not initialized.\n");
	        return 0;
		  }
	    }
	  }
  }
  return 0;
}
/*-----------------------------------------------------------------------------------*/
