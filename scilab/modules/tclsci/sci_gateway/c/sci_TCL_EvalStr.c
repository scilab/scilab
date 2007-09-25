/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "error_scilab.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_TCL_EvalStr) _PARAMS((char *fname,unsigned long l))
{ 
	CheckRhs(1,2);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char **Str=NULL;
		int m1,n1,i,j,RET;
		int m2,n2,l2;
        Tcl_Interp *TCLinterpreter=NULL;
        char **ReturnArrayString=NULL;
        int k=0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str);

		if (TCLinterp == NULL)
		{
			error_scilab(999,"%s : Error main TCL interpreter not initialized.",fname);
			return 0;
		}

		if (Rhs==2)
		{
			/* two arguments given - get a pointer on the slave interpreter */
			if (GetType(2) == sci_strings)
			{
				GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
				TCLinterpreter=Tcl_GetSlave(TCLinterp,cstk(l2));
				if (TCLinterpreter==NULL)
				{
					error_scilab(999,"%s: No such slave interpreter.",fname);
					return 0;
				}
			}
			else
			{
				error_scilab(999,"%s : Argument type must be character string.",fname);
				 return 0;
			}
		}
		else
		{
			/* only one argument given - use the main interpreter */
			TCLinterpreter=TCLinterp;
		}

        ReturnArrayString = (char **) MALLOC(m1*n1*sizeof(char **));

		for (i = 0; i<m1*n1 ;i++)
		{
			char *RetStr=NULL;
			char *AsciiFromUTF8=NULL;

			RET=Tcl_Eval(TCLinterpreter,Str[i]);

			if (RET==TCL_ERROR)
			{
                const char *trace = Tcl_GetVar(TCLinterpreter, "errorInfo", TCL_GLOBAL_ONLY);
				if(C2F(iop).err>0)
				{
					error_scilab(999,"%s, ScilabEval error at line %i\n	%s.",fname,i+1,(char *)trace);
				}
				else
				{
					error_scilab(999,"%s, %s at line %i\n	%s",fname,TCLinterpreter->result,i+1,(char *)trace);
				}
				if (Str) for (i = 0; i<m1*n1 ;i++)
				{
					if (Str[i]) 
					{
						FREE(Str[i]);
						Str[i]=NULL;
					}
				}
				if (Str) {FREE(Str); Str=NULL;}
				return 0;
            } else {
                /* return result of the successful evaluation of the script */
                /* return a matrix of string results */
                RetStr = (char*)Tcl_GetStringResult(TCLinterpreter);
                AsciiFromUTF8=UTF8toANSI(TCLinterpreter,RetStr);
                ReturnArrayString[k++]=AsciiFromUTF8;
            }
		}
		CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, ReturnArrayString);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();

		for (i=0;i<n1;i++) for (j=0;j<m1;j++) { FREE(ReturnArrayString[i+n1*j]);ReturnArrayString[i+n1*j]=NULL; }
		FREE(ReturnArrayString);
	}
	else
	{
		error_scilab(999,"Argument type incorrect: Must be a string or vector of strings.");
		return 0;
	}
	
  	return 0;
}
/*-----------------------------------------------------------------------------------*/
