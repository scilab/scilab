/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TclUiSet.h"
/*-----------------------------------------------------------------------------------*/
#define NBRSTYLE 9
char *UiStyleInternalName[NBRSTYLE] = {"button",
					           "checkbutton",
					           "checkbutton",
					           "entry",
					           "label",
		                       "scale",
					           "frame",
					           "scrolllistbox",
					           "popupmenu"};
char *UiStyleExternalName[NBRSTYLE] = {"pushbutton",
                               "radiobutton",
                               "checkbox",
                               "edit",
                               "text",
                               "slider",
                               "frame",
                               "listbox",
				               "popupmenu"};
/*-----------------------------------------------------------------------------------*/
int GetStyleInternalName(char *StyleStr);
/*-----------------------------------------------------------------------------------*/
int TCL_UiSet(int Handle,char *PropertieField,char *PropertieValue)
{
	#define CommandLenMax 2048	
	int bOK=0;
	int LenStr=0;
	
	char MyTclCommand[CommandLenMax];

	LenStr=(strlen(PropertieField)+strlen(PropertieValue)+strlen("SetField              ")+1);

	if (LenStr >= CommandLenMax)
	{
		Scierror(999,"Tcl Command > %d characters \r\n",CommandLenMax);
		return 0;
	}
	nocase(PropertieField);

	if ( CheckPropertyField(PropertieField) )
	{
		if (strcmp(PropertieField,"userdata")==0)
		{
			sciprint("Not yet implemented\n");
		}
		else
		{
			if (strcmp(PropertieField,"style")==0)
			{
				int StyleValue=-1;

				StyleValue=GetStyleInternalName(PropertieValue);
				if (StyleValue!=-1)
				{
					sprintf(MyTclCommand,"SetField %d \"%s\" \"%s\"",Handle,PropertieField,UiStyleExternalName[StyleValue]);
				}
				else
				{
					Scierror(999,"Invalid parameter(s) type (Style).\n");
					return 0;
				}
			}
			else
			{
				sprintf(MyTclCommand,"SetField %d \"%s\" \"%s\"",Handle,PropertieField,PropertieValue);
			}

			if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
			{
				Scierror(999,"Tcl Error %s\r\n",TCLinterp->result);
				return 0;
			}

			bOK=1;
		}
	}
	else
	{
		Scierror(999,"Unknown property %s.\r\n",PropertieField);
		return 0;
	}
	
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

		char *TmpPropertieField=NULL;

		GetRhsVar(RhsPropertieField,"c",&m1,&n1,&l1);
		TmpPropertieField=cstk(l1);

		PropertieField=(char*)malloc((strlen(TmpPropertieField)+1)*sizeof(char));
		sprintf(PropertieField,"%s",TmpPropertieField);

		if (GetType(RhsPropertieValue) == sci_strings)
		{
			char *TmpCharValue=NULL;

			GetRhsVar(RhsPropertieValue,"c",&m1,&n1,&l1);
			TmpCharValue=cstk(l1);

            PropertieValue=(char*)malloc((strlen(TmpCharValue)+1)*sizeof(char));
			sprintf(PropertieValue,"%s",TmpCharValue);
		}
		else if (GetType(RhsPropertieValue) == sci_matrix)
		{
			if (PropertieValue){free(PropertieValue);PropertieValue=NULL;}
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
int GetStyleInternalName(char *StyleStr)
{
	int IndexInternalName=-1;
	int i=0;
	for(i=0;i<NBRSTYLE;i++)
	{
		if ( strcmp(StyleStr,UiStyleExternalName[i])==0 )
		{
			IndexInternalName=i;
			return IndexInternalName;
		}
	}
	return IndexInternalName;
}
/*-----------------------------------------------------------------------------------*/
