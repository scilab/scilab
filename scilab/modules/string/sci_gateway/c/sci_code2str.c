
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* desc : str=code2str(c)
          Returns character string associated with Scilab int codes.
          str is such that c(i) is the Scilab int code of part(str,i))*/
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "localization.h"
#include "code2str.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int sci_code2str(char *fname,unsigned long fname_len)
{
	char **Output_Matrix = NULL;
	int Row_Num = 0,Col_Num = 0;
	int *Input_Matrix = NULL; /* Input matrix */

	int numRow = 1;
	int numCol = 0;
	int outIndex = 0 ;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if (VarType(1) == sci_matrix)
	{
		int Stack_position = 0;
		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&Row_Num,&Col_Num,&Stack_position);
		Input_Matrix=istk(Stack_position); /* Input*/
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Real matrix expected.\n"),fname,1);
		return 0;
	}

	numCol   = Row_Num*Col_Num ;

	/* Allocation output matrix */
	Output_Matrix = (char**)MALLOC(sizeof(char*)); 

	if (Output_Matrix == NULL)
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	}

	if (numCol != 0) 
	{
		Output_Matrix[0]=(char*)MALLOC(sizeof(char*)*(numCol));
	}
	else Output_Matrix[0]=(char*)MALLOC(sizeof(char*));

	if (Output_Matrix[0] == NULL)
	{
		FREE(Output_Matrix);
		Output_Matrix = NULL;
		Scierror(999,_("%s: No more memory.\n"),fname);
		return 0;
	}

	/* code2str algorithm */

	code2str(Output_Matrix,Input_Matrix,Row_Num*Col_Num);

	/* put on scilab stack */
	numRow   = 1 ; /*Output number row */
	outIndex = 0 ;
	CreateVar(Rhs+1,STRING_DATATYPE,&numRow,&numCol,&outIndex);
	strncpy(cstk(outIndex), &Output_Matrix[0][0] ,numCol ) ;
	LhsVar(1) = Rhs+1 ;
	C2F(putlhsvar)();

	/* free pointers */
	freeArrayOfString(Output_Matrix, 1);

	return 0;
}
/*--------------------------------------------------------------------------*/ 
