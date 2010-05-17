
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

/* desc : zero length string                                              */
/*        With no input argument returns a zero length character string   */
/*        With a matrix for input argument returns a zero length character*/
/*              strings matrix of the same size                           */
/*        With two int arguments returns a mxn zero length character  */
/*                strings matrix                                          */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "CreateEmptystr.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
static int sci_emptystr_no_rhs(void);
static int sci_emptystr_one_rhs(char *fname);
static int sci_emptystr_two_rhs(char *fname);
/*--------------------------------------------------------------------------*/
int sci_emptystr(char *fname,unsigned long fname_len)
{
	Rhs = Max(0, Rhs);
	
	CheckRhs(0,2);
	CheckLhs(1,1);
	
	if (Rhs < 1)
	{
		sci_emptystr_no_rhs();
	}
	else if (Rhs == 1)
	{
		sci_emptystr_one_rhs(fname);
	}
	else if (Rhs == 2)
	{
		sci_emptystr_two_rhs(fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/       
static int sci_emptystr_no_rhs(void)
{
	/* With no input argument returns a zero length character string */
	int m1 = 0, n1 = 0, l1 = 0;
	
	CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/       
static int sci_emptystr_one_rhs(char *fname)
{
	int m1 = 0,n1 = 0; /* m1 is the number of row ; n1 is the number of col*/

	/*With a matrix for input argument returns a zero length character strings matrix of the same size */
	int Type = VarType(1);

	if (Type == sci_matrix) 
	{
		char **Input_StringMatrix_One = NULL;
		
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&Input_StringMatrix_One);
		if ((m1 == 0) && (n1 == 0)) /* emptystr([]) */
		{
			int l = 0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
			return 0;
		}
	}
	else
	{
		int RHSPOS = 1;
		int l1 = 0;
		int il = 0;
		int lw = RHSPOS + Top - Rhs;
		
		l1 = *Lstk(lw);
		il = iadr(l1);
		
		if (*istk(il ) < 0) il = iadr(*istk(il + 1));
		
		/* get dimensions */
		m1 = getNumberOfLines(il); /* row */
		n1 = getNumberOfColumns(il); /* col */
	}

	/* m1 is the number of row ; n1 is the number of col*/
	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1, &n1, NULL);   
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	
	return 0;
}
/*--------------------------------------------------------------------------*/
static int sci_emptystr_two_rhs(char *fname)
{
	/*value_param_pos_1 is the number of row ; value_param_pos_2 is the number of col*/
	
	int Type_One = GetType(1);
	int Type_Two = GetType(2);

	if ((Type_One == sci_matrix) && (Type_Two == sci_matrix))
	{
		double value_param_pos_1 = 0;
		double value_param_pos_2 = 0;
		int matrixdimension = 0;

		int m1 = 0, n1 = 0, l1 = 0;
		int m2 = 0, n2 = 0, l2 = 0;
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);

		value_param_pos_1 = *stk(l1);
		value_param_pos_2 = *stk(l2);

		matrixdimension = (int)(value_param_pos_1 * value_param_pos_2);

		if (matrixdimension > 0)
		{
			int m = (int)value_param_pos_1;
			int n = (int)value_param_pos_2;
			CreateVarFromPtr(Rhs + 1,MATRIX_OF_STRING_DATATYPE, &m, &n, NULL);
		}
		else
		{
			/* returns [] */
			int l = 0;
			int m = 0;
			int n = 0;
			CreateVar(Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE,&m, &n,&l);
		}
		LhsVar(1) = Rhs + 1;

		C2F(putlhsvar)();
	}
	else
	{
		if (Type_One != sci_matrix)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"),fname,1);
		}
		else /* Type_Two */
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"),fname,2);
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
