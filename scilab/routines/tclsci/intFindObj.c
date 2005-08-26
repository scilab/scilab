/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intFindObj.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intFindObj) _PARAMS((char *fname))
{
	int *paramoutINT=NULL;

	CheckLhs(1,1);
	CheckRhs(2,2);

	if ( (GetType(1) == sci_strings ) && (GetType(2) == sci_strings ) )
	{
		static int l1,n1,m1;
		
		char MyTclCommand[2048];
		char *StrHandle;
		int Handle=0;
		char *field=NULL;
		char *value=NULL;
			
		GetRhsVar(1,"c",&m1,&n1,&l1);
		field=cstk(l1);

		GetRhsVar(2,"c",&m1,&n1,&l1);
		value=cstk(l1);

		/* Removed to make example given in findobj.xml work */
		/* nocase(value); */
		/* Added to be compatible with TclUiSet.c */
		nocase(field);

		sprintf(MyTclCommand, "set TclScilabTmpVar [FindObj \"%s\" \"%s\"];", field, value);
		if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
		{
			Scierror(999,"Tcl Error %s\r\n",TCLinterp->result);
			return 0;
		}
		StrHandle = (char*)Tcl_GetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
		
		Handle = (int)atoi(StrHandle);

		if (Handle == -1)  
		{
			/* object not found */
			m1=0;
			n1=0;
			l1=0;
			CreateVar(1,"d",  &m1, &n1, &l1);
		}
		else
		{
			/* objet found */
			paramoutINT=(int*)MALLOC(sizeof(int));
			*paramoutINT=(int)(Handle);
			n1=1;
			CreateVarFromPtr(1, "i", &n1, &n1, &paramoutINT);
			Tcl_UnsetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
		}
	}
	else
	{
		Scierror(999,TCL_ERROR8);
		return 0;
	}
	
	LhsVar(1) = 1;
	C2F(putlhsvar)();	

    if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}

	return 0;
}
/*-----------------------------------------------------------------------------------*/
