/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclGcf.h"
/*-----------------------------------------------------------------------------------*/
int GetTclCurrentFigure(void);
/*-----------------------------------------------------------------------------------*/
int C2F(intTclGcf) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)malloc(sizeof(int));

	CheckRhs(0,0);
	CheckLhs(1,1);
	
	*paramoutINT=GetTclCurrentFigure();
	n1=1;
	CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);

	LhsVar(1) = 1;
	C2F(putlhsvar)();	

	if (paramoutINT) {free(paramoutINT);paramoutINT=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int GetTclCurrentFigure(void)
{
	int ValRet=-1;
	char MyTclCommand[2000];
	char *StrHandle;

	sprintf(MyTclCommand, "set TclScilabTmpVar [GetGcf];"); 
	Tcl_Eval(TCLinterp,MyTclCommand);
	StrHandle = (char *) Tcl_GetVar(TCLinterp, "TclScilabTmpVar", 0);

	ValRet=(int)atoi(StrHandle);

	return ValRet;
}
/*-----------------------------------------------------------------------------------*/
