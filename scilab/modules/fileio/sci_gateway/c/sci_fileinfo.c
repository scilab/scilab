/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "gw_io.h"
#include "PATH_MAX.h"
#include "stack-c.h"
#include "fileinfo.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static double *InversionMatrixDouble(int W,int L,double *Matrix)
{
	double *buffer = NULL;
	if (Matrix)
	{
		buffer = (double *)MALLOC( (W*L)*sizeof(double) );
		if (buffer)
		{
			int i=0;
			int j=0;

			for (i=0; i<W; i++) for (j=0; j<L; j++) 
			{
				buffer[ i*L+j ] = Matrix[ j*W+i ];
			}
		}
	}
	return buffer;
}
/*--------------------------------------------------------------------------*/
int sci_fileinfo(char *fname,unsigned long fname_len)
{
	if (VarType(1) == sci_strings)
	{
		char **InputString_Parameter = NULL;
		int m = 0, n = 0;
		int mn = 0;

		CheckRhs(1,1);
		CheckLhs(1,2);

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE, &m, &n, &InputString_Parameter); 
		mn = m * n;

		if (mn == 0)
		{
			Scierror(999,"%s : Memory allocation error.\n",fname);
			return 0;
		}

		if (mn == 1)
		{
			int result = 0;
			double *infos = fileinfo(InputString_Parameter[0],&result);

			if (Lhs == 2)
			{
				int m_out = 1;
				int n_out = 1;
				int l_out = 0;
				CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE,&m_out,&n_out,&l_out);
				*stk(l_out) = (double) result;
				LhsVar(2) = Rhs + 2;
			}

			if (infos)
			{
				int m_out = 1;
				int n_out = FILEINFO_ARRAY_SIZE;
				CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &infos);
				LhsVar(1) = Rhs + 1;
				FREE(infos);
				infos = NULL;
			}
			else
			{
				int m_out = 0;
				int n_out = 0;
				int l_out = 0;

				/* returns [] */

                CreateVar(Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE,&m_out,&n_out,&l_out);
				LhsVar(1) = Rhs + 1;
			}

			freeArrayOfString(InputString_Parameter, mn);

			C2F(putlhsvar)();
		}
		else
		{
			if ( ((m == 1) && (n != 1)) || ((m != 1) && (n == 1)) )
			{
				int *results = (int*)MALLOC(sizeof(int) *mn);
				
				double *infos = filesinfo(InputString_Parameter, mn, results);

				

				if (infos)
				{
					int m_out = 0;
					int n_out = 0;
					double *infs = NULL;

					m_out = FILEINFO_ARRAY_SIZE;
					n_out = m*n;

					infs = InversionMatrixDouble(FILEINFO_ARRAY_SIZE,m*n,infos);

					n_out = FILEINFO_ARRAY_SIZE;
					m_out = m*n;

					CreateVarFromPtr(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE, &m_out, &n_out, &infs);
					LhsVar(1) = Rhs + 1;
				}
				else
				{
					int m_out = 0;
					int n_out = 0;
					int l_out = 0;

					/* returns [] */

					CreateVar(Rhs + 1,MATRIX_OF_DOUBLE_DATATYPE,&m_out,&n_out,&l_out);
					LhsVar(1) = Rhs + 1;

				}

				if (Lhs == 2)
				{
					int i = 0;
					int m_out = m;
					int n_out = n;
					int l_out = 0;

					CreateVar(Rhs + 2, MATRIX_OF_DOUBLE_DATATYPE,&m_out,&n_out,&l_out);
					for (i = 0; i < mn; i++)
					{
						*stk(l_out + i) = (double) results[i];
					}
					LhsVar(2) = Rhs + 2;
				}

				freeArrayOfString(InputString_Parameter, mn);

				C2F(putlhsvar)();
			}
			else
			{
				freeArrayOfString(InputString_Parameter, mn);
				Scierror(999, _("%s: Wrong size for input argument #%d: A vector expected.\n"), fname, 1);
			}
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"), fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/

