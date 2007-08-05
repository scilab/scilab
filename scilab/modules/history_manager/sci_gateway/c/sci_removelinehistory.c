/*-----------------------------------------------------------------------------------*/
/* Copyright INRIA 2007                                                              */
/* Author : Allan Cornet                                                             */
/* desc : interface for removelinehistory primitive                                  */
/*-----------------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "stack-c.h"
#include "Scierror.h"
#include "HistoryManager.h"
/*-----------------------------------------------------------------------------------*/
int C2F(sci_removelinehistory) _PARAMS((char *fname,unsigned long fname_len))
{
	Rhs = Max(Rhs,0);
	CheckRhs(1,1);
	CheckLhs(0,1);

	if ( GetType(1) == sci_matrix )
	{
		int N = -1;
		int l1,m1=1,n1=1;
		GetRhsVar(1,"i",&m1,&n1,&l1);
		N=*istk(l1);

		if ( (N < 0) || (N > getSizeScilabHistory()) )
		{
			Scierror(999,"Invalid parameter.\n");
			return 0;
		}
		else
		{
			deleteNthLineScilabHistory(N);
		}
	}
	else
	{
		Scierror(999,"Invalid parameter.\n");
		return 0;
	}


	LhsVar(1) = 0;
	C2F(putlhsvar)();
	return 0;
}
/*-----------------------------------------------------------------------------------*/
