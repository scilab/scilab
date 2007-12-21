/*------------------------------------------------------------------------*/
/* File: sci_strsplit.c                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Cong Wu , A.C                                                */
/*------------------------------------------------------------------------*/
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "gw_string.h"
#include "machine.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "strsplit.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*----------------------------------------------------------------------------*/
int C2F(sci_strsplit) _PARAMS((char *fname,unsigned long fname_len))
{
	CheckRhs(2,2);
	CheckLhs(1,1);

	if ( (VarType(1)== sci_strings) && (VarType(2)== sci_matrix) )
	{
		int m1 = 0, n1 = 0, l1 = 0;

		int m2 = 0, n2 = 0, l2 = 0;
		int m2n2 = 0;

		char *Input_String = NULL;
		int *Input_Matrix = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		Input_String = cstk(l1);

		GetRhsVar(2,MATRIX_OF_INTEGER_DATATYPE,&m2,&n2,&l2);
		m2n2 = m2 * n2;
		Input_Matrix = istk(l2);

		if ( (m2 != 1) && (n2 != 1) && (m2n2 != 1) )
		{
			Scierror(999,_("%s : Wrong type for second input argument: vector expected.\n"),fname);
			return 0;
		}

		if (m2n2 == 0)
		{
			int outIndex = 0;
			CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&outIndex);
			strcpy(cstk(outIndex), Input_String );
			LhsVar(1) = Rhs+1;
			C2F(putlhsvar)();
		}
		else
		{
			int i = 0;
			int j = 0;
			char **Output = NULL;
			int m_out = 0, n_out = 0;

			j = 0;
			for (i = 0; i < m2n2 ; i++)
			{
				/* Check input matrix */
				if ( (Input_Matrix[i] <= 0) || (Input_Matrix[i]>= m1) )
				{
					freeArrayOfString(Output,i);
					Scierror(116,_("%s : 2th argument has incorrect value.\n"),fname);
					return 0;
				}

				if ( i < (m2n2 - 1) )
				{
					if (Input_Matrix[i] >Input_Matrix[i+1])
					{
						freeArrayOfString(Output,i);
						Scierror(99,"%s : elements of 2th must be in increasing order!",fname);
						return 0;
					}
				}

				if (i == 0)
				{
					Output = (char**)MALLOC(sizeof(char*)*(m2n2 + 1) );
					if (Output == NULL)
					{
						Scierror(999,_("%s : Memory allocation error.\n"),fname);
						return 0;
					}

					Output[i] = (char*)MALLOC(sizeof(char)*(Input_Matrix[i]+1));
					if (Output[i] == NULL)
					{
						freeArrayOfString(Output,i);
						Scierror(999,_("%s : Memory allocation error.\n"),fname);
						return 0;
					}
					strncpy(Output[i],&Input_String[j],Input_Matrix[i]);
				}
				else
				{
					Output[i] = (char*)MALLOC(sizeof(char)*((Input_Matrix[i]-Input_Matrix[i-1])+1));
					if (Output[i] == NULL)
					{
						freeArrayOfString(Output,i);
						Scierror(999,_("%s : Memory allocation error.\n"),fname);
						return 0;
					}
					strncpy(Output[i],&Input_String[j],Input_Matrix[i]-Input_Matrix[i-1]);
				}

				j = Input_Matrix[i];
			}
			
			Output[m2n2] = (char*)MALLOC(sizeof(char)*(Input_Matrix[m2n2]+1));
			if (Output[m2n2] == NULL)
			{
				freeArrayOfString(Output,m2n2);
				Scierror(999,_("%s : Memory allocation error.\n"),fname);
				return 0;
			}
			strncpy(Output[m2n2],&Input_String[j],Input_Matrix[m2n2]-Input_Matrix[m2n2-1]);
			
			m_out = m2n2 + 1;
			n_out = 1;

			CreateVarFromPtr(Rhs + 1,MATRIX_OF_STRING_DATATYPE, &m_out, &n_out, Output );
			LhsVar(1) = Rhs + 1 ;
			C2F(putlhsvar)();
			freeArrayOfString(Output,m_out);
		}
	}
	else if (VarType(1) == sci_matrix)
	{
		int m = 0;
		int n = 0;
		int l = 0;

		GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
		if ( (m == 0) && (n == 0) )
		{
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&m,&n,&l);
			LhsVar(1) = Rhs+1 ;
			C2F(putlhsvar)();
		}
		else
		{
			Scierror(999,_("%s : Wrong type for first input argument: string expected.\n"),fname);
		}
		return 0;
	}
	else
	{
		Scierror(999,_("%s : Wrong type for input argument(s).\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
