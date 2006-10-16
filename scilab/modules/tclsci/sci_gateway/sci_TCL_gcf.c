/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "sci_TCL_gcf.h"
/*-----------------------------------------------------------------------------------*/
int GetTclCurrentFigure(void);
int SetTclCurrentFigure(int num);
/*-----------------------------------------------------------------------------------*/
extern void flushTKEvents(void);
extern void set_cf_type(int val);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_gcf) _PARAMS((char *fname,unsigned long l))
{
	static int n1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	CheckRhs(0,0);
	CheckLhs(1,1);
	
	*paramoutINT=GetTclCurrentFigure();
	n1=1;
	CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);

	LhsVar(1) = 1;
	C2F(putlhsvar)();	

	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
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
		Scierror(999,TCL_ERROR15);
		return 0;
	}

	
	ValRet=(int)atoi(StrHandle);
	Tcl_UnsetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);

	return ValRet;
}
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_scf) _PARAMS((char *fname,unsigned long l))
{
	static int l1,n1,m1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	CheckRhs(1,1);
	CheckLhs(1,1);
	
	if ( (Rhs== 1) && (IsAScalar(1)) )
	{
		int *Handle=NULL;

		GetRhsVar(1,"i",&m1,&n1,&l1);
		Handle=istk(l1);

		*paramoutINT=SetTclCurrentFigure(*Handle);
		flushTKEvents();
	}
	else
	{
		Scierror(999,TCL_ERROR8);
		return 0;
	}
	
	n1=1;
	CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);

	LhsVar(1) = 1;
	C2F(putlhsvar)();	

	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
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
		Scierror(999,TCL_ERROR16);
		return 0;
	}
	
	set_cf_type(0); /* Current handle is a TCL Handle */

	ValRet=(int)atoi(StrHandle);
	Tcl_UnsetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);

	return ValRet;

}
/*-----------------------------------------------------------------------------------*/
