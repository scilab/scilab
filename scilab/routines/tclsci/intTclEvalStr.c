/*-----------------------------------------------------------------------------------*/
/* INRIA 2005 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/
#include "intTclEvalStr.h"
/*-----------------------------------------------------------------------------------*/
int C2F(intTclEvalStr) _PARAMS((char *fname))
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

		GetRhsVar(1,"S",&m1,&n1,&Str);

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

        ReturnArrayString = (char **) MALLOC(m1*n1*sizeof(char **));

		for (i = 0; i<m1*n1 ;i++)
		{
			char *RetStr=NULL;
			char *AsciiFromUTF8=NULL;

			RET=Tcl_Eval(TCLinterpreter,Str[i]);

			if (RET==TCL_ERROR)
			{
				if(C2F(iop).err>0)
				{
                    Scierror(999,TCL_ERROR10,fname,i+1);
				}
				else
				{
					Scierror(999,TCL_ERROR11,fname,TCLinterpreter->result,i+1);
				}
				FreeRhsSVar(Str);
				return 0;
            } else {
                /* return result of the successful evaluation of the script */
                /* return a matrix of string results */
                RetStr = (char*)Tcl_GetStringResult(TCLinterpreter);
                AsciiFromUTF8=UTF8toANSI(TCLinterpreter,RetStr);
                ReturnArrayString[k++]=AsciiFromUTF8;
            }
		}
		CreateVarFromPtr(Rhs+1, "S", &m1, &n1, ReturnArrayString);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();

		for (i=0;i<n1;i++) for (j=0;j<m1;j++) { FREE(ReturnArrayString[i+n1*j]);ReturnArrayString[i+n1*j]=NULL; }
		FREE(ReturnArrayString);
	}
	else
	{
		Scierror(999,TCL_ERROR12);
		return 0;
	}
	
  	return 0;
}
/*-----------------------------------------------------------------------------------*/
