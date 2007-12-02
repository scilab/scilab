/*--------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*--------------------------------------------------------------------------*/
#include "TclUiSet.h"
#include "Scierror.h"
#include "localization.h"
#include "setvar.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
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
/*--------------------------------------------------------------------------*/
static int GetStyleInternalName(char *StyleStr);
/*--------------------------------------------------------------------------*/
int InterfaceScilabToUiSet(int Handle,int RhsPropertieField,int RhsPropertieValue)
{
	int bOK=0;

	if (Handle>0)
	{
		if (GetType(RhsPropertieField)==sci_strings)
		{
			static int l1,n1,m1;
			char *StrField=NULL;

			GetRhsVar(RhsPropertieField,STRING_DATATYPE,&m1,&n1,&l1);
			StrField=cstk(l1);
			nocase(StrField);

			if (strcmp(StrField,"userdata")==0)
			{
				if (GetType(RhsPropertieValue)==sci_strings)
				{
					char **Str=NULL;
					char VarName[64];
					int i=0;

					GetRhsVar(RhsPropertieValue,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
					sprintf(VarName,"USERDATA_%d",Handle);

					/* Erase the previous value */
					Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

					if ( (m1==1) && (n1==1) )
					{
						SetVarAString(TCLinterp,VarName,Str);
					}
					else
					{
						SetVarStrings(TCLinterp,VarName,Str,m1,n1);
					}
					freeArrayOfString(Str,m1*n1);
				}
				else if (GetType(RhsPropertieValue)==sci_matrix)
				{
					char VarName[64];

					sprintf(VarName,"USERDATA_%d",Handle);

					GetRhsVar(RhsPropertieValue,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);

					if ( (m1==0) && (n1==0) )
					{
						Scierror(999,_("[] doesn't work with Tcl/Tk.\n"));
						return 0;
					}

					if ( (m1==1) && (n1==1) )
					{
						SetVarScalar(TCLinterp,VarName,(double)*stk(l1));
					}
				else 
					{
						SetVarMatrix(TCLinterp,VarName,l1,m1,n1);
					}
				}
				else
				{
					Scierror(999,_("Invalid parameter(s) type.\n"));
					return 0;
				}
			}
			else
			{
				if(strcmp(StrField,"string")==0)
				{
					if (GetType(RhsPropertieValue)==sci_strings)
					{
						char **Str=NULL;
						char VarName[64];
						int i=0;

						GetRhsVar(RhsPropertieValue,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);
						sprintf(VarName,"STRING_%d",Handle);

						/* Efface valeur precedente */
						Tcl_UnsetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);

						if ( (m1==1) && (n1==1) )
						{
							/* Les donnees sont dans STRING_%d */
							SetVarAString(TCLinterp,VarName,Str);

							/* Block inutile apres modifications dans set.tcl */
							{
								#define CommandLenMax 2048	
							

								char MyTclCommand[CommandLenMax];
								sprintf(MyTclCommand,_("SetField %d \"%s\" \"%s\""),Handle,StrField,Str[0]);

								if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
								{
									Scierror(999,_("Tcl Error : %s\n"),TCLinterp->result);
									return 0;
								}
							}
						}
						else
						{
							SetVarStrings(TCLinterp,VarName,Str,m1,n1);

							/* Block inutil apres modifications dans set.tcl */
							{
								#define CommandLenMax 2048	

								char MyTclCommand[CommandLenMax];
								sprintf(MyTclCommand,_("SetField %d \"%s\" \"%s\""),Handle,StrField,Str[0]);

								if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
								{
									Scierror(999,_("Tcl Error : %s\n"),TCLinterp->result);
									return 0;
								}
							}
						}
						freeArrayOfString(Str,m1*n1);
						bOK=1;
					}
					else
					{
						/* doit une chaine ou matrice de chaine */
						Scierror(999,_("Input argument type incorrect: Must be a string.\n"));
						return 0;
					}

				}
				else
				{
					/* Cas general */
					char *PropertieField=NULL;
					char *PropertieValue=NULL;

					PropertieField=(char*)MALLOC((strlen(StrField)+1)*sizeof(char));
					strcpy(PropertieField,StrField);

					if (GetType(RhsPropertieValue) == sci_strings)
					{
						char *TmpCharValue=NULL;

						GetRhsVar(RhsPropertieValue,STRING_DATATYPE,&m1,&n1,&l1);
						TmpCharValue=cstk(l1);

						PropertieValue=(char*)MALLOC((strlen(TmpCharValue)+1)*sizeof(char));
						strcpy(PropertieValue,TmpCharValue);
					}
					else if (GetType(RhsPropertieValue) == sci_matrix)
					{
						if (PropertieValue){FREE(PropertieValue);PropertieValue=NULL;}
						PropertieValue=Matrix2String(RhsPropertieValue);
					}
					else
					{
						Scierror(999,_("Invalid parameter(s) type.\n"));
						return 0;
					}
					bOK=TCL_UiSet(Handle,PropertieField,PropertieValue);

					if (PropertieField){FREE(PropertieField);PropertieField=NULL;}
					if (PropertieValue){FREE(PropertieValue);PropertieValue=NULL;}
				}
			}
		}
		else
		{
			/* champ propertie doit etre une chaine */
			Scierror(999,_("Field parameter must be a string.\n"));
			return 0;
		}
	}
	else
	{
		/* Handle numero incorrect */
		Scierror(999,_("Invalid Handle. it must be > 0.\n"));
		return 0;
	}
	return bOK;
}
/*--------------------------------------------------------------------------*/
int TCL_UiSet(int Handle,char *PropertieField,char *PropertieValue)
{
	#define CommandLenMax 2048	
	int bOK=0;
	int LenStr=0;
	
	char MyTclCommand[CommandLenMax];

	LenStr=(int)(strlen(PropertieField)+strlen(PropertieValue)+strlen("SetField              ")+1);

	if (LenStr >= CommandLenMax)
	{
		Scierror(999,_("Tcl Command > %d characters.\n"),CommandLenMax);
		return 0;
	}
	nocase(PropertieField);

	if ( CheckPropertyField(PropertieField) )
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
				Scierror(999,_("Invalid parameter(s) type (Style).\n"));
				return 0;
			}
		}
		else
		{
			sprintf(MyTclCommand,"SetField %d \"%s\" \"%s\"",Handle,PropertieField,PropertieValue);
		}

		if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
		{
			Scierror(999,_("Tcl Error : %s\n"),TCLinterp->result);
			return 0;
		}
		bOK=1;
	}
	else
	{
		Scierror(999,_("Unknown property %s.\n"),PropertieField);
		return 0;
	}
	
	return bOK;
}
/*--------------------------------------------------------------------------*/
static int GetStyleInternalName(char *StyleStr)
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
/*--------------------------------------------------------------------------*/
