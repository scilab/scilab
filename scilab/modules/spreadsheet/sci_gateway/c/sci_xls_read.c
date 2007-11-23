/*--------------------------------------------------------------------------*/
/* Authors Pierrick Mode, Serge Steer INRIA 2005, Copyright INRIA */
/* Allan CORNET INRIA 2007 reorganize code */
/*--------------------------------------------------------------------------*/ 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "gw_spreadsheet.h"
#include "xls.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_xls_read) _PARAMS((char *fname,unsigned long fname_len))
{
	int m1 = 0,n1 = 0,l1 = 0,zero = 0,ierr = 0;
	double *data = NULL;
	int *ind = NULL;
	int M = 0,N = 0,MN = 0;
	int pos = 0,fd = 0;

	CheckLhs(2,2);
	CheckRhs(2,2);

	if ( (VarType(1) != sci_matrix) || (VarType(2) != sci_matrix) )
	{
		Scierror(999,"%s :Invalid parameter(s).\n",fname);
		return 0;
	}

	/*  checking variable fd */
	GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
	fd=(int)*stk(l1);
	/*  checking variable Pos */
	GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
	pos=(int)*stk(l1);

	xls_read(&fd,&pos,&data, &ind, &N, &M,  &ierr);

	switch (ierr)
	{
	case 1 :
		Scierror(999,_("%s :Not enough memory to allocate results\n"),fname);
		return 0;
		break;
	case 2 :
		Scierror(999,_("%s :Failed to read expected data, may be invalid xls file\n"),fname);
		return 0;
		break;
	case 3 :
		Scierror(999,_("%s :End of File\n"),fname);
		return 0;
		break;
	default :
		/* no error */
		break;
	}

	MN = M*N;

	if (MN == 0)
	{
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &zero,&zero, &l1);
		CreateVar(Rhs+2,MATRIX_OF_DOUBLE_DATATYPE, &zero,&zero, &l1);
	}
	else
	{
		CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &N,&M, &data);
		CreateVarFromPtr(Rhs+2,MATRIX_OF_INTEGER_DATATYPE, &N,&M, &ind);
		FREE(data);
		data = NULL;
		FREE(ind);
		ind = NULL;
	}
	LhsVar(1)= Rhs+1;
	LhsVar(2)= Rhs+2;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
