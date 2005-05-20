/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclGcf.h"
/*-----------------------------------------------------------------------------------*/
int GetTclCurrentFigure(void);
int SetTclCurrentFigure(int num);
/*-----------------------------------------------------------------------------------*/
extern int cf_type;
/*-----------------------------------------------------------------------------------*/
int C2F(intTclGcf) _PARAMS((char *fname))
{
	static int n1;
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
	char MyTclCommand[2048];
	char *StrHandle=NULL;

	sprintf(MyTclCommand, "set TclScilabTmpVar [GetGcf];"); 

	if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
    {
		Scierror(999,"Tcl Error %s\r\n",TCLinterp->result);
		return 0;
	}

	StrHandle = (char *) Tcl_GetVar(TCLinterp, "TclScilabTmpVar",TCL_GLOBAL_ONLY);

	if ( StrHandle == NULL  )
    {
		Scierror(999,"Tcl Error : GetTclCurrentFigure.\r\n");
		return 0;
	}

	ValRet=(int)atoi(StrHandle);

	return ValRet;
}
/*-----------------------------------------------------------------------------------*/
int C2F(intTclScf) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)malloc(sizeof(int));

	CheckRhs(1,1);
	CheckLhs(1,1);
	
	if ( (Rhs== 1) && (IsAScalar(1)) )
	{
		int *Handle=NULL;

		GetRhsVar(1,"i",&m1,&n1,&l1);
		Handle=istk(l1);

		*paramoutINT=SetTclCurrentFigure(*Handle);
	}
	else
	{
		Scierror(999,"Invalid parameter type.\n");
		return 0;
	}
	
	n1=1;
	CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);

	LhsVar(1) = 1;
	C2F(putlhsvar)();	

	if (paramoutINT) {free(paramoutINT);paramoutINT=NULL;}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int SetTclCurrentFigure(int num)
{
	int ValRet=-1;
	char MyTclCommand[2048];
	char *StrHandle=NULL;

	sprintf(MyTclCommand, "set TclScilabTmpVar [FigureSelect %d];",num); 

	if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
    {
		Scierror(999,"Tcl Error %s\r\n",TCLinterp->result);
		return 0;
	}

	StrHandle = (char *) Tcl_GetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);

	if ( StrHandle == NULL  )
    {
		Scierror(999,"Tcl Error : SetTclCurrentFigure.\r\n");
		return 0;
	}

	cf_type=0; /* Current handle is a TCL Handle */

	ValRet=(int)atoi(StrHandle);

	return ValRet;

}
/*-----------------------------------------------------------------------------------*/
