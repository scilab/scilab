
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Cong WU , Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "BOOL.h"
/*----------------------------------------------------------------------------*/
extern int C2F(icopy)(int *nbelements, int *arrayInput, int *incBetweenElementsInputs,
					  int *arrayOutput, int *incBetweenElementsOutputs);
/* fortran function defined in SCI/modules/elementary_functions/src/fortran */
/*----------------------------------------------------------------------------*/
int C2F(sci_strsplit)(char *fname,unsigned long fname_len)
{
	/* Check Input & Output parameters */
	CheckRhs(2,2);
	CheckLhs(1,1);

	if (VarType(1) == sci_matrix) /* strsplit([]) */
	{
		int m = 0, n = 0, l = 0;
		
		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
		if ( (m == 0) && (n == 0) )
		{
			/* returns [] */
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings or empty matrix expected.\n"),fname,1);
		}
		return 0;
	}
	else if ( (VarType(1) == sci_strings) && (VarType(2) == sci_matrix) )
	{
		/* split works directly on scilab code and not on strings :( */
		/* interface written with stack1 */
		/* faster but very difficult to understand and debug :( */
		/* benchmark : Windows Vista C2D 6600 2.4 Ghz */
		/* 
		text     = mgetl(SCI+"/modules/string/tests/benchmarks/ascii_text.txt");
		text_cat = strcat(text);
		nb_run = 10000;
		timer();
		for i = 1:nb_run
		a = strsplit(text_cat,[1000 2000 3000 4000 5000 6000 7000]);
		end
		timing = timer();
		returned_time = timing * 1000000 / nb_run;
		*/

		/* Scilab 4.1.2 : 140 s */
		/* Scilab strsplit stack1 (current version) : 142 s */
		/* Scilab strsplit stack3 : 12200 s */
		/* slower because with stack3, scilab converts scilab code to strings and strings to code */

		#define  get_value_param2(pos) (*stk(l2 + pos))
		#define  POSITION_FIRST_CHAR 1

		int i = 0;

		int l1 = 0, il1 = 0; /* variables paramater 1 */
		int lengthInputString = 0;
		BOOL bParam1IsaRef = FALSE;

		int l2 = 0, il2 = 0; /* variables paramater 2 */
		int m2n2 = 0; /* (m2 * n2) size of param2 */
		BOOL bParam2IsaRef = FALSE;

		int ilr = 0; /* variable paramater output */
		int  id1 = 0;
		
		int one = 1;

		int lw = C2F(vstk).lstk[Top];

		/* check if parameter two is a reference */
		il2 = iadr( C2F(vstk).lstk[Top - 1] );
		bParam2IsaRef = (*istk(il2) < 0);
		if (bParam2IsaRef) il2 = iadr( *istk(il2 + 1) );

		/* get number elements in param2 */
		m2n2 = *istk(il2 + 1) * *istk(il2 + 2);

		l2 = sadr(il2 + 4);

		/* Check second parameter (a matrix) is sorted (increasing) */
		if (get_value_param2(0) < POSITION_FIRST_CHAR) 
		{
			Err = 2;
			Error(116);
			return 0;
		}

		for (i = 1; i <= m2n2 - 1; ++i) 
		{
			if ( get_value_param2(i) < get_value_param2(i-1) ) 
			{
				Err = 2;
				Error(99);
				return 0;
			}
		}

		/* returns @ Top */
		--Top;

		/* check if parameter one is a reference */
		il1 = iadr( C2F(vstk).lstk[Top - 1] );
		bParam1IsaRef = (*istk(il1) < 0);

		if (bParam1IsaRef) il1 = iadr( *istk(il1+1) );

		id1 = il1 + 4;
		l1 = id1 + 2;
		lengthInputString = *istk(id1+1) - 1;

		/* check that last value if >=  than length of input string */
		if ( get_value_param2(m2n2 - 1) >= (double) lengthInputString ) 
		{
			Err = 2;
			Error(116);
			return 0;
		}

		if (bParam1IsaRef && bParam2IsaRef) 
		{
			ilr = iadr ( C2F(vstk).lstk[Top - 1] );
		}
		else 
		{
			int j = 0;
			ilr = iadr (lw);
			j = ilr + 4 + m2n2 + 2 + lengthInputString;

			Err = sadr(j) - C2F(vstk).lstk[Bot - 1];
			if (Err > 0) 
			{
				/* stacksize exceeded */
				Error(17);
				return 0;
			}
		}

		/* create output strings matrix , see C2F(crematvar) stack1.c */
		*istk(ilr) = sci_strings;
		*istk(ilr + 1) = m2n2 + 1;
		*istk(ilr + 2) = 1;
		*istk(ilr + 3) = 0;
		*istk(ilr + 4) = 1;
		
		for (i = 1; i <= m2n2; ++i) 
		{
			*istk(ilr + 4 + i) = (int) (*stk(l2 + i - 1) + 1);
		}

		/* put values on stack */
		*istk(ilr + 5 + m2n2) = lengthInputString + 1;
		C2F(icopy)(&lengthInputString, istk(l1), &one, istk(ilr + nsiz + m2n2), &one);

		if (bParam1IsaRef && bParam2IsaRef) 
		{
			int j = ilr + nsiz + m2n2 + lengthInputString;
			C2F(vstk).lstk[Top] = sadr(j);
		}
		else 
		{
			int j = 0;
			int il = iadr( C2F(vstk).lstk[Top - 1]);

			j = m2n2 + nsiz + lengthInputString;
			C2F(icopy)(&j, istk(ilr), &one, istk(il), &one);

			j = il + nsiz + m2n2 + lengthInputString;
			C2F(vstk).lstk[Top] = sadr(j);
		}
	}
	else
	{
		if(VarType(1) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of integers expected.\n"),fname,2);
		}
	}
	return 0;
}
/*----------------------------------------------------------------------------*/
//int C2F(sci_strsplit)(char *fname,unsigned long fname_len)
//{
//	CheckRhs(2,2);
//	CheckLhs(1,1);
//
//	if ( (VarType(1)== sci_strings) && (VarType(2)== sci_matrix) )
//	{
//		int m1 = 0, n1 = 0, l1 = 0;
//
//		int m2 = 0, n2 = 0, l2 = 0;
//		int m2n2 = 0;
//
//		char *Input_String = NULL;
//		int *Input_Matrix = NULL;
//
//		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
//		Input_String = cstk(l1);
//
//		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
//		m2n2 = m2 * n2;
//		Input_Matrix = istk(l2);
//
//		if ( (m2 != 1) && (n2 != 1) && (m2n2 != 1) )
//		{
//			Scierror(999,_("%s : Wrong type for second input argument: vector expected.\n"),fname);
//			return 0;
//		}
//
//		if (m2n2 == 0)
//		{
//			int outIndex = 0;
//			CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
//			strcpy(cstk(outIndex), Input_String );
//			LhsVar(1) = Rhs+1;
//			C2F(putlhsvar)();
//		}
//		else
//		{
//			int i = 0;
//			int j = 0;
//			char **Output = NULL;
//			int m_out = 0, n_out = 0;
//
//			j = 0;
//			for (i = 0; i < m2n2 ; i++)
//			{
//				/* Check input matrix */
//				if ( (Input_Matrix[i] <= 0) || (Input_Matrix[i]>= m1) )
//				{
//					freeArrayOfString(Output,i);
//					Scierror(116,_("%s : 2th argument has incorrect value.\n"),fname);
//					return 0;
//				}
//
//				if ( i < (m2n2 - 1) )
//				{
//					if (Input_Matrix[i] >Input_Matrix[i+1])
//					{
//						freeArrayOfString(Output,i);
//						Scierror(99,"%s : elements of 2th must be in increasing order!",fname);
//						return 0;
//					}
//				}
//
//				if (i == 0)
//				{
//					Output = (char**)MALLOC(sizeof(char*)*(m2n2 + 1) );
//					if (Output == NULL)
//					{
//						Scierror(999,_("%s: No more memory.\n"),fname);
//						return 0;
//					}
//
//					Output[i] = (char*)MALLOC(sizeof(char)*(Input_Matrix[i]+1));
//					if (Output[i] == NULL)
//					{
//						freeArrayOfString(Output,i);
//						Scierror(999,_("%s: No more memory.\n"),fname);
//						return 0;
//					}
//					strncpy(Output[i],&Input_String[j],Input_Matrix[i]);
//				}
//				else
//				{
//					Output[i] = (char*)MALLOC(sizeof(char)*((Input_Matrix[i]-Input_Matrix[i-1])+1));
//					if (Output[i] == NULL)
//					{
//						freeArrayOfString(Output,i);
//						Scierror(999,_("%s: No more memory.\n"),fname);
//						return 0;
//					}
//					strncpy(Output[i],&Input_String[j],Input_Matrix[i]-Input_Matrix[i-1]);
//				}
//
//				j = Input_Matrix[i];
//			}
//			
//			Output[m2n2] = (char*)MALLOC(sizeof(char)*(Input_Matrix[m2n2]+1));
//			if (Output[m2n2] == NULL)
//			{
//				freeArrayOfString(Output,m2n2);
//				Scierror(999,_("%s: No more memory.\n"),fname);
//				return 0;
//			}
//			strncpy(Output[m2n2],&Input_String[j],Input_Matrix[m2n2]-Input_Matrix[m2n2-1]);
//			
//			m_out = m2n2 + 1;
//			n_out = 1;
//
//			CreateVarFromPtr(Rhs + 1,MATRIX_OF_STRING_DATATYPE, &m_out, &n_out, Output );
//			LhsVar(1) = Rhs + 1 ;
//			C2F(putlhsvar)();
//			freeArrayOfString(Output,m_out);
//		}
//	}
//	else if (VarType(1) == sci_matrix)
//	{
//		int m = 0;
//		int n = 0;
//		int l = 0;
//
//		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
//		if ( (m == 0) && (n == 0) )
//		{
//			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
//			LhsVar(1) = Rhs+1 ;
//			C2F(putlhsvar)();
//		}
//		else
//		{
//			Scierror(999,_("%s : Wrong type for first input argument: string expected.\n"),fname);
//		}
//		return 0;
//	}
//	else
//	{
//		Scierror(999,_("%s : Wrong type for input argument(s).\n"),fname);
//	}
//	return 0;
//}
///*--------------------------------------------------------------------------*/
