/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TclUiGet.h"
/*-----------------------------------------------------------------------------------*/
extern int GetScreenProperty(char *prop, char *value);
extern int TCL_ArrayExist(Tcl_Interp *TCLinterpreter,char *VarName);
/*
extern int TCL_ArraySize(Tcl_Interp *TCLinterpreter,char *VarName);
*/
extern int TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *m,int *n);
extern char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,int i,int j);
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
			StrValue=(char*)MALLOC((StrMax+1)*sizeof(char));

			if(!GetScreenProperty(StrField,StrValue))
			{
				MatrixOut=String2Matrix(StrValue,&nbelem);
				n1=1;
				m1=nbelem;
				CreateVarFromPtr(Rhs+1, "d", &n1, &m1, &MatrixOut);
				if (MatrixOut) {FREE(MatrixOut); MatrixOut=NULL;}
			}
			else
			{
				if (StrValue) {FREE(StrValue); StrValue=NULL;}
				Scierror(999,TCL_ERROR30,StrField);
				return 0;
			}
			if (StrValue) {FREE(StrValue); StrValue=NULL;}
			bOK=1;
		}
		else
		{
			Scierror(999,TCL_ERROR8);
			return 0;
		}
	}
	else
   	{
		if (Handle>0)
		{
			if (GetType(RhsPropertieField)==sci_strings)
			{
				static int l1,n1,m1;
				char *StrField=NULL;

				GetRhsVar(RhsPropertieField,"c",&m1,&n1,&l1);
				StrField=cstk(l1);
				nocase(StrField);

				if (strcmp(StrField,"userdata")==0)
				{
					char VarName[64];
					sprintf(VarName,"USERDATA_%d",Handle);

					if (TCL_ArrayExist(TCLinterp,VarName))
					{
						int i=0,j=0;
						int nrow=0,ncol=0;
						int k=0;

						char **ReturnArrayString=NULL;

						TCL_ArrayDim(TCLinterp,VarName,&ncol,&nrow);
						ReturnArrayString = (char **) MALLOC(ncol*nrow*sizeof(char **));
						k=0;
						for (j=1;j<ncol+1;j++)	for (i=1;i<nrow+1;i++)
						{
							char *RetStr=NULL;
							char *AsciiFromUTF8=NULL;

							RetStr=TCL_ArrayGetVar(TCLinterp,VarName,i,j);
							AsciiFromUTF8=UTF8toANSI(TCLinterp,RetStr);

							ReturnArrayString[k++]=AsciiFromUTF8;
						}

						CreateVarFromPtr(Rhs+1, "S", &nrow, &ncol, ReturnArrayString);

						for (i=0;i<ncol;i++) for (j=0;j<nrow;j++) { FREE(ReturnArrayString[i+ncol*j]);ReturnArrayString[i+ncol*j]=NULL; }
						FREE(ReturnArrayString);
						bOK=1;

					}
					else
					{
						char *RetStr=NULL;

						RetStr= (char*)Tcl_GetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);
						if ( RetStr )
						{
							char *AsciiFromUTF8=NULL;
							char *output=NULL ;

							AsciiFromUTF8=UTF8toANSI(TCLinterp,RetStr);

							output=(char*)MALLOC((strlen(AsciiFromUTF8)+1)*sizeof(char));
							sprintf(output,"%s",AsciiFromUTF8);
							CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);

							if (output) {FREE(output);output=NULL;}
							if (AsciiFromUTF8){FREE(AsciiFromUTF8);AsciiFromUTF8=NULL;}
							bOK=1;
						}
						else
						{
							Scierror(999,TCL_ERROR18);
							return 0;
						}
					}
				}
				else
				{
					if (strcmp(StrField,"string")==0)
					{
						char VarName[64];
						sprintf(VarName,"STRING_%d",Handle);
						if (TCL_ArrayExist(TCLinterp,VarName))
						{
							int i=0,j=0;
							int nrow=0,ncol=0;
							int k=0;

							char **ReturnArrayString=NULL;

							TCL_ArrayDim(TCLinterp,VarName,&ncol,&nrow);
							ReturnArrayString = (char **) MALLOC(ncol*nrow*sizeof(char **));
							k=0;
							for (j=1;j<ncol+1;j++)	for (i=1;i<nrow+1;i++)
							{
								char *RetStr=NULL;
								char *AsciiFromUTF8=NULL;

								RetStr=TCL_ArrayGetVar(TCLinterp,VarName,i,j);
								AsciiFromUTF8=UTF8toANSI(TCLinterp,RetStr);

								ReturnArrayString[k++]=AsciiFromUTF8;
							}

							/*CreateVarFromPtr(Rhs+1, "S", &nrow, &ncol, ReturnArrayString);*/

							for (i=0;i<ncol;i++) for (j=0;j<nrow;j++) { FREE(ReturnArrayString[i+ncol*j]);ReturnArrayString[i+ncol*j]=NULL; }
							FREE(ReturnArrayString);
							bOK=1;

						}
						else
						{
							char *RetStr=NULL;

							RetStr= (char*)Tcl_GetVar(TCLinterp, VarName, TCL_GLOBAL_ONLY);
							if ( RetStr )
							{
								char *AsciiFromUTF8=NULL;
								char *output=NULL ;

								AsciiFromUTF8=UTF8toANSI(TCLinterp,RetStr);

								output=(char*)MALLOC((strlen(AsciiFromUTF8)+1)*sizeof(char));
								sprintf(output,"%s",AsciiFromUTF8);
								/*CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);*/

								if (output) {FREE(output);output=NULL;}
								if (AsciiFromUTF8){FREE(AsciiFromUTF8);AsciiFromUTF8=NULL;}
								bOK=1;
							}
							else
							{
								Scierror(999,TCL_ERROR18);
								return 0;
							}
						}

						/* Block inutile apres modifications dans get.tcl */
						{
							char *StrValue=NULL;
							sprintf(MyTclCommand,"set TclScilabTmpVar [GetField %d \"%s\"]", Handle, StrField);

							if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
							{
								Scierror(999,TCL_ERROR4,TCLinterp->result);
								return 0;
							}

							StrValue = (char*)Tcl_GetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);

							if (StrValue)
							{
								char *AsciiFromUTF8=NULL;

								AsciiFromUTF8=UTF8toANSI(TCLinterp,StrValue);
								m1=strlen(AsciiFromUTF8);
								n1=1;
								CreateVar(Rhs+1, "c", &m1,&n1,&l1);
								sprintf(cstk(l1),"%s",AsciiFromUTF8);
								bOK=1;
							}
							else
							{
								Scierror(999,TCL_ERROR31);
								return 0;
							}
						}
					}
					else
					{
						char *StrValue=NULL;
						sprintf(MyTclCommand,"set TclScilabTmpVar [GetField %d \"%s\"]", Handle, StrField);

						if ( Tcl_Eval(TCLinterp,MyTclCommand) == TCL_ERROR  )
						{
							Scierror(999,TCL_ERROR4,TCLinterp->result);
							return 0;
						}
						StrValue = (char*)Tcl_GetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);

						if (StrValue)
					{
						char *AsciiFromUTF8=NULL;
						AsciiFromUTF8=UTF8toANSI(TCLinterp,StrValue);

						if ( MustReturnAMatrix(StrField) )
						{
							int nbelem=0;
							double *MatrixOut=NULL;

							MatrixOut=String2Matrix(AsciiFromUTF8,&nbelem);
							n1=1;
							m1=nbelem;
							CreateVarFromPtr(Rhs+1, "d", &n1, &m1, &MatrixOut);
							if (MatrixOut) {FREE(MatrixOut); MatrixOut=NULL;}
							bOK=1;
						}
						else
						if ( MustReturnAString(StrField) )
						{
							m1=strlen(AsciiFromUTF8);
							n1=1;
							CreateVar(Rhs+1, "c", &m1,&n1,&l1);
							sprintf(cstk(l1),"%s",AsciiFromUTF8);
							bOK=1;
						}
						else
						{
							Scierror(999,TCL_ERROR30,StrField);
							return 0;
						}
						if (AsciiFromUTF8){FREE(AsciiFromUTF8);AsciiFromUTF8=NULL;}

						Tcl_UnsetVar(TCLinterp, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
					}
						else
						{
							Scierror(999,TCL_ERROR31);
							return 0;
						}
					}
		}
		}
			else
			{
				Scierror(999,TCL_ERROR8);
				return 0;
			}
		}
		else
		{
			Scierror(999,TCL_ERROR34);
			return 0;
		}
	}
	return bOK;
}
/*-----------------------------------------------------------------------------------*/

