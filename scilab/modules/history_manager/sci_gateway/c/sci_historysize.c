/*------------------------------------------------------------------------*/
/* Copyright INRIA 2007                                                   */
/* Authors : Allan Cornet						                                      */
/* desc : interface for historysize primitive                             */
/*------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_historysize) _PARAMS((char *fname,unsigned long fname_len))
{
	int *paramoutINT=NULL;
	int n = 1;
	Rhs = Max(Rhs,0);
	CheckRhs(0,0);
	CheckLhs(0,1);

	paramoutINT=(int*)MALLOC(sizeof(int));

	*paramoutINT = getSizeScilabHistory();

	CreateVarFromPtr(Rhs+1,MATRIX_OF_INTEGER_DATATYPE, &n, &n, &paramoutINT);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	if (paramoutINT) {FREE(paramoutINT);paramoutINT = NULL;}
	return 0 ;
}
/*--------------------------------------------------------------------------*/
