/*-----------------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*-----------------------------------------------------------------------------------*/
#ifndef __TCL_ARRAYDIM_H__
#define __TCL_ARRAYDIM_H__

#include "TCL_Global.h"

/**
* TCL_ArrayDim get dimension of a TCL Array
* @param[in] TCLinterpreter : TCL Interpreter
* @param[in] VarName : TCL variable name
* @param[out] m size row 
* @param[out] n size col
* @return FALSE or TRUE (OK)
*/
BOOL TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *m,int *n);

#endif /* __TCL_ARRAYDIM_H__ */
/*-----------------------------------------------------------------------------------*/
