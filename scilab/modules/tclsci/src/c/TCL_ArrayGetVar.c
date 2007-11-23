/*--------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "TCL_ArrayGetVar.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
#define TCL_NOT_DEFINE "#NOT DEF.#"
/*--------------------------------------------------------------------------*/
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,int i,int j)
{
	char *RetStr=NULL;
	char * StrValue=NULL;
	char ArrayName[2048];

	sprintf(ArrayName,"%s(%d,%d)",VarName,i,j);

	RetStr = (char*)Tcl_GetVar(TCLinterpreter, ArrayName, TCL_GLOBAL_ONLY);

	if (RetStr)
	{
		StrValue=MALLOC((strlen(RetStr)+1)*sizeof(char));
		sprintf(StrValue,"%s",RetStr);
	}
	else
	{
		StrValue=MALLOC((strlen(TCL_NOT_DEFINE)+1)*sizeof(char));
		sprintf(StrValue,"%s",TCL_NOT_DEFINE);
	}

	return StrValue;
}
/*--------------------------------------------------------------------------*/
