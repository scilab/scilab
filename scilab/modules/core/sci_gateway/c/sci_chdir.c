/*-----------------------------------------------------------------------------------*/
/* INRIA 2006 */
/* Allan CORNET */
/*-----------------------------------------------------------------------------------*/ 
#include <stdio.h>
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "scicurdir.h"
#include "cluni0.h"
/*-----------------------------------------------------------------------------------*/
#ifndef MAX_PATH_STR
#define MAX_PATH_STR 1024
#endif
/*-----------------------------------------------------------------------------------*/
int C2F(sci_chdir) _PARAMS((char *fname,unsigned long fname_len))
{
	Rhs = Max(0, Rhs);
	CheckRhs(0,1);
	CheckLhs(1,1);

	if ( (Rhs == 0) || (GetType(1) == sci_strings) )
	{
		int ierr = 0;
		static int l1,n1,m1;
		char shortpath[MAX_PATH_STR];
		char path[MAX_PATH_STR];
		int out_n = 0;

		if (Rhs == 0)
		{
			strcpy(shortpath,"home/");
		}
		else
		{
			GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
			strcpy(shortpath,cstk(l1));
		}

		C2F(cluni0)(shortpath,path,&out_n,(long)strlen(shortpath),MAX_PATH_STR);
		C2F(scichdir)(path,&ierr);

		n1=1;
		CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);

		if (ierr == 0) *istk(l1)=(int)(TRUE);
		else *istk(l1)=(int)(FALSE);

		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,"Invalid parameter.\r\n");
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/
