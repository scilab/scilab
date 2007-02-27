/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#ifndef __SCI_TCL_GETVAR__
#define __SCI_TCL_GETVAR__

int TCL_ArrayExist(Tcl_Interp *TCLinterpreter,char *VarName);
int TCL_ArraySize(Tcl_Interp *TCLinterpreter,char *VarName);
int TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *m,int *n);
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,int i,int j);



#endif /* __SCI_TCL_GETVAR__ */
/*-----------------------------------------------------------------------------------*/ 
