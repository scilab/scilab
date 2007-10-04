/*-----------------------------------------------------------------------------------*/
/* INRIA 2007 */
/* Sylvestre LEDRU */
/*-----------------------------------------------------------------------------------*/

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param Str   
 * @param m 
 * @param n 
 * @return <ReturnValue>
 */
int SetVarStrings(Tcl_Interp *TCLinterpreter,char *VarName,char **Str,int m,int n);

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param Str   
 * @return <ReturnValue>
 */
int SetVarAString(Tcl_Interp *TCLinterpreter,char *VarName,char **Str);

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param ptrValues 
 * @param m 
 * @param n 
 * @return <ReturnValue>
 */
int SetVarMatrix(Tcl_Interp *TCLinterpreter,char *VarName,int ptrValues,int m,int n);

/**
 * @TODO: add comment
 *
 * @param TCLinterpreter    
 * @param VarName   
 * @param VarValue  
 * @return <ReturnValue>
 */
int SetVarScalar(Tcl_Interp *TCLinterpreter,char *VarName,double VarValue);
