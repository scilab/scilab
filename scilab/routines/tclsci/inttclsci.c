#include "inttclsci.h"
/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
extern int C2F(intTclDoOneEvent) _PARAMS((char *fname));
extern int C2F(intTclEvalFile) _PARAMS((char *fname));
extern int C2F(intTclEvalStr) _PARAMS((char *fname));
extern int C2F(intTclGetVar) _PARAMS((char *fname));
extern int C2F(intTclSetVar) _PARAMS((char *fname));
/*-----------------------------------------------------------------------------------*/
 static TCLSCITable Tab[]=
 {
  {C2F(intTclDoOneEvent),"TCL_DoOneEvent"},
  {C2F(intTclEvalFile),"TCL_EvalFile"},
  {C2F(intTclEvalStr),"TCL_EvalStr"},
  {C2F(intTclGetVar),"TCL_GetVar"},
  {C2F(intTclSetVar),"TCL_SetVar"}

 };
/*-----------------------------------------------------------------------------------*/
int C2F(inttclsci)()
{  
  Rhs = Max(0, Rhs);
  (*(Tab[Fin-1].f)) (Tab[Fin-1].name,strlen(Tab[Fin-1].name));
  return 0;
}
/*-----------------------------------------------------------------------------------*/
