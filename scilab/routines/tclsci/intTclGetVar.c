/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclGetVar.h"
/*-----------------------------------------------------------------------------------*/
int TCL_ArrayExist(Tcl_Interp *TCLinterpreter,char *VarName);
int TCL_ArraySize(Tcl_Interp *TCLinterpreter,char *VarName);
int TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *m,int *n);
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,int i,int j);
/*-----------------------------------------------------------------------------------*/
int C2F(intTclGetVar) _PARAMS((char *fname))
{
	static int l1,n1,m1;
	static int l2,n2,m2;
	
	Tcl_Interp *TCLinterpreter=NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);
	
	if (GetType(1) == sci_strings)
	{
		char *VarName=NULL;

		GetRhsVar(1,"c",&m1,&n1,&l1);
		VarName=cstk(l1);

		if (TCLinterp == NULL)
		{
			Scierror(999,TCL_ERROR13,fname);
			return 0;
		}

		if (Rhs==2)
		{
			/* two arguments given - get a pointer on the slave interpreter */
			if (GetType(2) == sci_strings)
			{
				GetRhsVar(2,"c",&m2,&n2,&l2)
				TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
				if (TCLinterpreter==NULL)
				{
					Scierror(999,TCL_ERROR17,fname);
					return 0;
				}
			}
			else
			{
				 Scierror(999,TCL_ERROR14,fname);
				 return 0;
			}
		}
		else
		{
			/* only one argument given - use the main interpreter */
			TCLinterpreter=TCLinterp;
		}

		if (TCL_ArrayExist(TCLinterpreter,VarName))
		{
			int i=0,j=0;
			int nrow=0,ncol=0;
			int k=0;

			char **ReturnArrayString=NULL;

			TCL_ArrayDim(TCLinterpreter,VarName,&ncol,&nrow);
			ReturnArrayString = (char **) MALLOC(ncol*nrow*sizeof(char **));
			k=0;
			for (j=1;j<ncol+1;j++)	for (i=1;i<nrow+1;i++)
			{
				char *RetStr=NULL;
				char *AsciiFromUTF8=NULL;

				RetStr=TCL_ArrayGetVar(TCLinterpreter,VarName,i,j);
				AsciiFromUTF8=UTF8toANSI(TCLinterpreter,RetStr);
		
				ReturnArrayString[k++]=AsciiFromUTF8;
			}

			CreateVarFromPtr(Rhs+1, "S", &nrow, &ncol, ReturnArrayString);

			LhsVar(1)=Rhs+1;
			C2F(putlhsvar)();

			for (i=0;i<ncol;i++) for (j=0;j<nrow;j++) { FREE(ReturnArrayString[i+ncol*j]);ReturnArrayString[i+ncol*j]=NULL; }
			FREE(ReturnArrayString);
		}
		else
		{
			char *RetStr=NULL;

			RetStr= (char*)Tcl_GetVar(TCLinterpreter, VarName, TCL_GLOBAL_ONLY);
			if ( RetStr )
			{
				char *AsciiFromUTF8=NULL;
				char *output=NULL ;

				AsciiFromUTF8=UTF8toANSI(TCLinterpreter,RetStr);

				output=(char*)MALLOC((strlen(AsciiFromUTF8)+1)*sizeof(char));
				sprintf(output,"%s",AsciiFromUTF8);
				CreateVarFromPtr( 1, "c",(m1=strlen(output), &m1),&n1,&output);

				LhsVar(1) = 1;
				C2F(putlhsvar)();

				if (output) {FREE(output);output=NULL;}
				if (AsciiFromUTF8){FREE(AsciiFromUTF8);AsciiFromUTF8=NULL;}
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
		 Scierror(999,TCL_ERROR14,fname);
		 return 0;
	}
	
	return 0;
}
/*-----------------------------------------------------------------------------------*/
int TCL_ArrayExist(Tcl_Interp *TCLinterpreter,char *VarName)
{
	int bExist=FALSE;

	if (strcmp(VarName,"TclScilabTmpVar"))
	{
		char MyTclCommand[2048];
		char *StrArrayExist=NULL;

		sprintf(MyTclCommand, "set TclScilabTmpVar [array exists %s];",VarName); 

		if ( Tcl_Eval(TCLinterpreter,MyTclCommand) == TCL_ERROR  )
		{
			Scierror(999,"Tcl Error %s\r\n",TCLinterpreter->result);
			return 0;
		}

		StrArrayExist = (char *) Tcl_GetVar(TCLinterpreter, "TclScilabTmpVar",TCL_GLOBAL_ONLY);

		if (StrArrayExist)
		{
			bExist=(int)atoi(StrArrayExist);
			Tcl_UnsetVar(TCLinterpreter, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
		}
	}
	
	return bExist;
}
/*-----------------------------------------------------------------------------------*/
int TCL_ArraySize(Tcl_Interp *TCLinterpreter,char *VarName)
{
	int ArraySize=0;

	if (strcmp(VarName,"TclScilabTmpVar"))
	{
		char MyTclCommand[2048];
		char *StrArraySize=NULL;

		sprintf(MyTclCommand, "set TclScilabTmpVar [array size %s];",VarName); 

		if ( Tcl_Eval(TCLinterpreter,MyTclCommand) == TCL_ERROR  )
		{
			Scierror(999,"Tcl Error %s\r\n",TCLinterpreter->result);
			return 0;
		}

		StrArraySize = (char *) Tcl_GetVar(TCLinterpreter, "TclScilabTmpVar",TCL_GLOBAL_ONLY);

		if (StrArraySize)
		{
			ArraySize=(int)atoi(StrArraySize);
			Tcl_UnsetVar(TCLinterpreter, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
		}
	}
	return ArraySize;
}
/*-----------------------------------------------------------------------------------*/
int TCL_ArrayDim(Tcl_Interp *TCLinterpreter,char *VarName,int *m,int *n)
{
	int Bok=FALSE;
	*m=0;
	*n=0;
	if (strcmp(VarName,"TclScilabTmpVar"))
	{
		char MyTclCommand[2048];
		char *StrArrayDims=NULL;
		
		sprintf(MyTclCommand, "set TclScilabTmpVar [lsort -dictionary [array names %s *,*]];",VarName); 

		if ( Tcl_Eval(TCLinterpreter,MyTclCommand) == TCL_ERROR  )
		{
			Scierror(999,"Tcl Error %s\r\n",TCLinterpreter->result);
			return 0;
		}

		StrArrayDims = (char *) Tcl_GetVar(TCLinterpreter, "TclScilabTmpVar",TCL_GLOBAL_ONLY);

		if (StrArrayDims)
		{
			char *token=NULL;
			char StrDimensions[256];
			int DimX;
			int DimY;
			token = strrchr( StrArrayDims,' ');
			if (token)
			{
				int loop=0;
				if (token[0]==' ') token=&token[1];
				for(loop=0;loop<(int)strlen(token);loop++) if (token[loop] == ',') token[loop]=' ';
				sprintf(StrDimensions,"%s",token);
				sscanf(StrDimensions,"%d %d",&DimX,&DimY);
				*n=DimX;
				*m=DimY;
			}
			else /* just a element */
			{
				int loop=0;
				for(loop=0;loop<(int)strlen(StrArrayDims);loop++) if ( StrArrayDims[loop] == ',')  StrArrayDims[loop]=' ';
				sprintf(StrDimensions,"%s", StrArrayDims);
				sscanf(StrDimensions,"%d %d",&DimX,&DimY);
				*n=DimX;
				*m=DimY;
			}
			Tcl_UnsetVar(TCLinterpreter, "TclScilabTmpVar", TCL_GLOBAL_ONLY);
			Bok=TRUE;
		}
	}
	return Bok;
}
/*-----------------------------------------------------------------------------------*/
char *TCL_ArrayGetVar(Tcl_Interp *TCLinterpreter,char *VarName,int i,int j)
{
	char *RetStr=NULL;
	char * StrValue=NULL;
	char ArrayName[2048];
	sprintf(ArrayName,"%s(%d,%d)",VarName,i,j);
	RetStr= (char*)Tcl_GetVar(TCLinterpreter, ArrayName, TCL_GLOBAL_ONLY);

	if (RetStr)
	{
		StrValue=MALLOC((strlen(RetStr)+1)*sizeof(char));
		sprintf(StrValue,"%s",RetStr);
	}
	else
	{
		StrValue=MALLOC((strlen("#NOT DEF.#")+1)*sizeof(char));
		sprintf(StrValue,"%s","#NOT DEF.#");
	}

	return StrValue;
}
/*-----------------------------------------------------------------------------------*/
