/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TclUiSet.h"
/*-----------------------------------------------------------------------------------*/
int TCL_UiSet(int Handle,char *PropertieField,char *PropertieValue)
{
	#define CommandLenMax 2048	
	int bOK=0;
	int LenStr=0;
	
	char MyTclCommand[CommandLenMax];
	
	sciprint("TCL_UiSet handle %d propertie %s value %s\n",Handle,PropertieField,PropertieValue);

	LenStr=(strlen(PropertieField)+strlen(PropertieValue)+strlen("SetField    ")+1);

	if (LenStr >= CommandLenMax)
	{
		Scierror(999,"Tcl Command > %d characters \r\n",CommandLenMax);
		return 0;
	}
	nocase(PropertieField);

	sprintf(MyTclCommand,"SetField %d \"%s\" \"%s\"",Handle,PropertieField,PropertieValue);

	if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
	{
		Scierror(999,"Tcl Error %s\r\n",TCLinterp->result);
		return 0;
	}

	bOK=1;

	
	return bOK;
}
/*-----------------------------------------------------------------------------------*/
int InterfaceScilabToUiSet(int  Handle,int RhsPropertieField,int RhsPropertieValue)
{
	static int l1,n1,m1;
	int bOK=0;
	
	if ( (Handle > 0) && (GetType(RhsPropertieField) == sci_strings) )
	{
		char *PropertieField=NULL;
		char *PropertieValue=NULL;

		GetRhsVar(RhsPropertieField,"c",&m1,&n1,&l1);
		PropertieField=cstk(l1);

		if (GetType(RhsPropertieValue) == sci_strings)
		{
			GetRhsVar(RhsPropertieValue,"c",&m1,&n1,&l1);
			PropertieValue=cstk(l1);
		}
		else if (GetType(RhsPropertieValue) == sci_matrix)
		{
			PropertieValue=Matrix2String(RhsPropertieValue);
		}
		else
		{
			Scierror(999,"Invalid parameter(s) type.\n");
			return 0;
		}

		bOK=TCL_UiSet(Handle,PropertieField,PropertieValue);

		if (PropertieField){free(PropertieField);PropertieField=NULL;}
		if (PropertieValue){free(PropertieValue);PropertieValue=NULL;}
	}
	else
	{
		if (Handle <= 0)
		{
			Scierror(999,"Invalid Handle. it must be > 0.\n");
			return 0;
		}
		if (GetType(RhsPropertieField) != sci_strings)
		{
			Scierror(999,"Invalid parameter(s) type.\n");
			return 0;
		}
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
