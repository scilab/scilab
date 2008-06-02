/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006/2007 - INRIA - Alan LAYEC
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "sci_fftw_flags.h"
#include "fftw_utilities.h"
#include "MALLOC.h"
#include "gw_fftw.h"
#include "localization.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern unsigned cur_fftw_flags;
/*--------------------------------------------------------------------------*/
/* fftw_flags function.
*
* Scilab Calling sequence :
*   -->[a,b]=fftw_flags();
* or
*   -->[a,b]=fftw_flags(S);
*
*  a is an int scalar. (the int value of
*                            the flag parameter of fftw)
*  b is a string matrix.(the string values of
*                             the flag parameter of fftw)
*  S is a string matrix or an int or a double scalar
*  given the value(s) of the fftw flag parameter.
*
*  This function gives and set the flag parameter of fftw
*  when creating a new plan.
*  This should be done before calling fftw function.
*  (default is FFTW_ESTIMATE)
*/
int sci_fftw_flags(char *fname,unsigned long fname_len)
{
	/* declaration of variables to store scilab parameters address */
	static int l1 = 0, m1 = 0, n1 = 0;
	SciIntMat M1;
	char **Str1 = NULL;

	static int l2 = 0, m2 = 0, n2 = 0;

	char **Str3 = NULL;
	int len = 0;

	/* please update me ! */
	static int nb_flag = 22;
	static char *Str[]= {/* documented flags */
                       "FFTW_MEASURE",
                       "FFTW_DESTROY_INPUT",
                       "FFTW_UNALIGNED",
                       "FFTW_CONSERVE_MEMORY",
                       "FFTW_EXHAUSTIVE",
                       "FFTW_PRESERVE_INPUT",
                       "FFTW_PATIENT",
                       "FFTW_ESTIMATE",

                       /* undocumented beyond-guru flags */
                       "FFTW_ESTIMATE_PATIENT",
                       "FFTW_BELIEVE_PCOST",
                       "FFTW_NO_DFT_R2HC",
                       "FFTW_NO_NONTHREADED",
                       "FFTW_NO_BUFFERING",
                       "FFTW_NO_INDIRECT_OP",
                       "FFTW_ALLOW_LARGE_GENERIC",
                       "FFTW_NO_RANK_SPLITS",
                       "FFTW_NO_VRANK_SPLITS",
                       "FFTW_NO_VRECURSE",
                       "FFTW_NO_SIMD",
                       "FFTW_NO_SLOW",
                       "FFTW_NO_FIXED_RADIX_LARGE_N",
                       "FFTW_ALLOW_PRUNING"};

	static unsigned flagt[]= {/* documented flags */
                            FFTW_MEASURE,
                            FFTW_DESTROY_INPUT,
                            FFTW_UNALIGNED,
                            FFTW_CONSERVE_MEMORY,
                            FFTW_EXHAUSTIVE,
                            FFTW_PRESERVE_INPUT,
                            FFTW_PATIENT,
                            FFTW_ESTIMATE,

                            /* undocumented beyond-guru flags */
                            FFTW_ESTIMATE_PATIENT,
                            FFTW_BELIEVE_PCOST,
                            FFTW_NO_DFT_R2HC,
                            FFTW_NO_NONTHREADED,
                            FFTW_NO_BUFFERING,
                            FFTW_NO_INDIRECT_OP,
                            FFTW_ALLOW_LARGE_GENERIC,
                            FFTW_NO_RANK_SPLITS,
                            FFTW_NO_VRANK_SPLITS,
                            FFTW_NO_VRECURSE,
                            FFTW_NO_SIMD,
                            FFTW_NO_SLOW,
                            FFTW_NO_FIXED_RADIX_LARGE_N,
                            FFTW_ALLOW_PRUNING};
	unsigned flagv = 0;

	int i = 0,j = 0;

	CheckRhs(0,1);

	if (Rhs==0) 
	{
		// nothing
	}
	else 
	{
		switch(VarType(1)){
			case sci_ints:

			/* int */
			GetRhsVar(1,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE, &m1, &n1, &M1);
			CheckDims(1,m1,n1,1,1);
			cur_fftw_flags = ((int *)M1.D)[0];
			break;
			case sci_matrix:
				/* double */
				GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
				CheckDims(1,m1,n1,1,1);
				cur_fftw_flags = (int)*stk(l1);
				break;
			case sci_strings:
				/* string */
				GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&Str1);
				for (j=0;j<m1*n1;j++) 
					{
						for (i=0;i<nb_flag;i++) 
							{
								if (strcmp(Str1[j],Str[i])==0) break;
							}

						if (i == nb_flag) 
							{
								freeArrayOfString(Str1,m1*n1);
								Scierror(999,_("%s: Wrong values for input argument #%d: FFTW flag expected.\n"),fname,1); 
								return(0);
							}
						else 
							{
								if (i>0) flagv = ( flagv | (1U << (i-1)) );
							}
					}
				cur_fftw_flags = flagv;
				freeArrayOfString(Str1,m1*n1);
				break;
			default:
				Scierror(53,_("%s: Wrong type for input argument #%d.\n"),fname,1);
				return(0);
		}
	}

	/* return value of Sci_Plan.flags in position 2 */
	m2 = 1;
	n2 = m2;
	l2 = I_INT32;
	CreateVar(Rhs+2,MATRIX_OF_VARIABLE_SIZE_INTEGER_DATATYPE,&m2,&n2,&l2);
	*istk(l2)=(int) cur_fftw_flags;

	/*Test for only FFTW_MEASURE*/
	if (cur_fftw_flags == 0) 
	{
		j = 1;
		if ((Str3 = (char **)MALLOC(sizeof(char *))) == NULL) 
		{
			Scierror(999,_("%s: No more memory.\n"),fname);
			return(0);
		}

		len = (int)strlen(Str[0]);
		if ((Str3[0] = (char *)MALLOC(sizeof(char)*(len+1))) == NULL) 
		{
			Scierror(999,_("%s: No more memory.\n"),fname);
			return(0);
		}
		strcpy(Str3[0],Str[0]);
	}
	else 
	{
		j = 0;
		for (i = 1;i < nb_flag; i++) 
		{
			if((cur_fftw_flags&flagt[i])==flagt[i]) 
			{
				j++;
				if (Str3) Str3 = (char **)REALLOC(Str3,sizeof(char *)*j);
				else Str3 = (char **)MALLOC(sizeof(char *)*j);

				if ( Str3 == NULL) 
				{
					Scierror(999,_("%s: No more memory.\n"),fname);
					return(0);
				}
				len = (int)strlen(Str[i]);
				if ((Str3[j-1] = (char *)MALLOC(sizeof(char)*(len+1))) == NULL) 
				{
					freeArrayOfString(Str3,j);
					Scierror(999,_("%s: No more memory.\n"),fname);
					return(0);
				}
				strcpy(Str3[j-1],Str[i]);
			}
		}
	}

	n1=1;
	CreateVarFromPtr( Rhs+3,MATRIX_OF_STRING_DATATYPE, &j, &n1, Str3);
	LhsVar(1)=Rhs+2;
	LhsVar(2)=Rhs+3;
	PutLhsVar();

	freeArrayOfString(Str3,j);
	return(0);
}
/*--------------------------------------------------------------------------*/ 
