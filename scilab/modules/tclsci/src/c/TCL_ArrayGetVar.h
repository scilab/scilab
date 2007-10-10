/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#ifndef __TCL_ARRAYGETVAR_H__
#define __TCL_ARRAYGETVAR_H__

#include "TCL_Global.h"

/**
* TCL_ArrayGetVar
* @param[in] Tcl_Interp : TCL interpreter
* @param[in] VarName : Variable Name (Tcl Array)
* @param[in] i position
* @param[in] j position
* @return value (string)
*/
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,int i,int j);

#endif /* __TCL_ARRAYGETVAR_H__ */
/*-----------------------------------------------------------------------------------*/
