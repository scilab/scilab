/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#ifndef __TCL_ARRAYSIZE_H__
#define __TCL_ARRAYSIZE_H__

#include "TCL_Global.h"

/**
* TCL_ArraySize
* @param[in] Tcl_Interp TCL Interpreter
* @param[in] VarName TCL Variable name
* @return 0 or 1 (OK)
*/
int TCL_ArraySize(Tcl_Interp *TCLinterpreter,char *VarName);

#endif /* __TCL_ARRAYSIZE_H__ */
/*-----------------------------------------------------------------------------------*/
