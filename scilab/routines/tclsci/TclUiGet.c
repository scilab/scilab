/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TclUiGet.h"
/*-----------------------------------------------------------------------------------*/
extern int GetScreenProperty(char *prop, char *value);
/*-----------------------------------------------------------------------------------*/
int TCL_UiGet(int  Handle,int RhsPropertieField)
{
	#define StrMax 200

	int bOK=0;
	static int l1,n1,m1;
	char MyTclCommand[2048];
	
	if (Handle == 0)
	{
		if (GetType(RhsPropertieField) == sci_strings)
		{
			char *StrField=NULL;
			char *StrValue=NULL;
			int nbelem=0;
			double *MatrixOut=NULL;

			GetRhsVar(RhsPropertieField,"c",&m1,&n1,&l1);
			StrField=cstk(l1);
			StrValue=(char*)malloc((StrMax+1)*sizeof(char));

			if(!GetScreenProperty(StrField,StrValue))
			{
				MatrixOut=String2Matrix(StrValue,&nbelem);
				n1=1;
				m1=nbelem;
				CreateVarFromPtr(Rhs+1, "d", &n1, &m1, &MatrixOut);
				if (MatrixOut) {free(MatrixOut); MatrixOut=NULL;}
			}
			else
			{
				if (StrValue) {free(StrValue); StrValue=NULL;}
				Scierror(999,"Unknown property %s\r\n",StrField);
				return 0;
			}
			if (StrValue) {free(StrValue); StrValue=NULL;}
			bOK=1;
		}
		else
		{
			Scierror(999,"Invalid parameter(s) type.\n");
			return 0;
		}
	}
	else
   	{
		char *StrField=NULL;
		char *StrValue=NULL;

		GetRhsVar(RhsPropertieField,"c",&m1,&n1,&l1);
		StrField=cstk(l1);

		nocase(StrField);

		if ( CheckPropertyField(StrField) )
		{
			if (strcmp(StrField,"userdata")==0)
			{
				sciprint("Not yet implemented\n");
				m1=0;
				n1=0;
				l1=0;
				CreateVar(Rhs+1,"d",  &m1, &n1, &l1);
				bOK=1;
			}
			else
			{
				sprintf(MyTclCommand,"set TclScilabTmpVar [GetField %d \"%s\"]", Handle, StrField);
		
				if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
				{
					Scierror(999,"Tcl Error %s\r\n",TCLinterp->result);
					return 0;
				}
				StrValue = (char*)Tcl_GetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);

				if (StrValue)
				{
					if ( MustReturnAMatrix(StrField) )
					{
						int nbelem=0;
						double *MatrixOut=NULL;

						MatrixOut=String2Matrix(StrValue,&nbelem);
						n1=1;
						m1=nbelem;
						CreateVarFromPtr(Rhs+1, "d", &n1, &m1, &MatrixOut);
						if (MatrixOut) {free(MatrixOut); MatrixOut=NULL;}
						bOK=1;
					}
					else
					if ( MustReturnAString(StrField) )
					{
						m1=strlen(StrValue);
						n1=1;
						CreateVar(Rhs+1, "c", &m1,&n1,&l1);
						sprintf(cstk(l1),"%s",StrValue);
						bOK=1;
					}
					else
					{
						Scierror(999,"Unknown property %s\r\n",StrField);
						return 0;
					}
				}
				else
				{
					Scierror(999,"Error property not found.\n");
					return 0;
				}
			}
		}
		else
		{
			Scierror(999,"Unknown property %s\r\n",StrField);
			return 0;
		}

	
	}

	return bOK;
}
/*-----------------------------------------------------------------------------------*/
