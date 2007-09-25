/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "stack-def.h"
#include "MALLOC.h"
#include "hashtable_core.h"
#include "error_scilab.h"
#include "Funtab.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(cvnamel)(int *id,char *str,int *jobptr,int *str_len); 
/*-----------------------------------------------------------------------------------*/
int C2F(sci_clearfun) _PARAMS((char *fname,unsigned long fname_len))
{
	static int l1,n1,m1;
	int *Status=NULL;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if (GetType(1) == sci_strings)
	{
		char *VarName=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		VarName=cstk(l1);

		Status=(int*)MALLOC(sizeof(int));

		if (ExistFunction(VarName))
		{
			int id[nsiz];
			int zero=0;
			int fptr=0;
			int job=4;

			C2F(cvname)(id,VarName,&zero,(unsigned long)strlen(VarName));
			C2F(funtab)(id,&fptr,&job,"NULL_NAME",0);
			*Status=TRUE;
		}
		else
		{
			*Status=FALSE;
		}
		m1=1;n1=1;
		CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
		LhsVar(1)=Rhs+1;

		C2F(putlhsvar)();
		if (Status) {FREE(Status);Status=NULL;}
	}
	else
	{
		error_scilab(999,"must be a name of function in scilab.");
		return 0;
	}

	return 0;
}
/*-----------------------------------------------------------------------------------*/

