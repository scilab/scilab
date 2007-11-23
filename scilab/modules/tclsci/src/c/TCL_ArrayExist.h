/*--------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*--------------------------------------------------------------------------*/
#ifndef __TCL_ARRAYEXIST_H__
#define __TCL_ARRAYEXIST_H__

#include "TCL_Global.h"

/**
* TCL_ArrayExist
* check if "VarName" array exist
* @param[in] TCLinterpreter
* @param[in] VarName
* @return FALSE or TRUE (Exist) 
*/
BOOL TCL_ArrayExist(Tcl_Interp *TCLinterpreter,char *VarName);

#endif /* __TCL_ARRAYEXIST_H__ */
/*--------------------------------------------------------------------------*/
