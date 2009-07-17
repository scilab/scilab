/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdlib.h>
#include "gw_core.h"
#include "stack-c.h"
#include "localization.h"
#include "Scierror.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
/* @TO DO : extend 'error' primitive
/*--------------------------------------------------------------------------*/
#define defaultErrorCode 10000
#define defaultErrorPosition 0
#define defaultErrorMessage " "
#define bufferErrorMessage C2F(cha1).buf
/*--------------------------------------------------------------------------*/
int C2F(sci_error)(char *fname,unsigned long fname_len)
{
	 int errorCode = defaultErrorCode;
	 int errorPosition = defaultErrorPosition;

	 CheckRhs(1,2);
	 CheckLhs(1,1);

	 if (Rhs == 1) 
	 {
		switch ( GetType(1) )
		{
			/* error('My message') */
			case sci_strings : 
			{
				int m = 0; int n = 0;
				char **InputString_Parameter = NULL;
				
				GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m,&n,&InputString_Parameter);
				/* check scalar */
				if ( (m == 1) && (n == 1) )
				{
					errorCode = defaultErrorCode;
					strcpy(bufferErrorMessage,InputString_Parameter[0]);

					freeArrayOfString(InputString_Parameter,m*n);

					C2F(iop).err = errorPosition;
					SciError(errorCode);
				}
				else
				{
					freeArrayOfString(InputString_Parameter,m*n);
					Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
					return 0;
				}
			}
			break;

			/* error(code error) */
			case sci_matrix :
			{
				int m = 0, n = 0, l = 0;
				/* check double compatibility scilab 4.x */
				GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m, &n, &l);

				/* check scalar */
				if ( (m == 1) && (n == 1) ) 
				{
					/* store error */
					errorCode = (int)(*stk(l));
                                        if (errorCode <= 0)
                                          {
                                            Scierror(999,_("%s: Wrong value for input argument #%d: Value greater than 0 expected.\n"),fname,1);
                                            return 0;
                                          }
					strcpy(bufferErrorMessage,defaultErrorMessage);
					C2F(iop).err = errorPosition;
					SciError(errorCode);
				}
				else 
				{
					Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
					return 0;
				}
			}
			break;

			default :
			{
				Scierror(999,_("%s: Wrong type for input argument #%d.\n"),fname,1);
				return 0;
			}
			break;
		}
	 }
	 else /* Rhs == 2 */
	 {
		/* error('message',code error) */
		if ( ( GetType(1) == sci_strings ) && ( GetType(2) == sci_matrix ) )
		{
			int m1 = 0; int n1 = 0;
			char **InputString_Parameter1 = NULL;

			int m2 = 0, n2 = 0, l2 = 0;

			GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&InputString_Parameter1);

			/* check scalar */
			if ( (m1 == 1) && (n1 == 1) )
			{
				GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
				/* check scalar */
				if ( (m2 == 1) && (n2 == 1) )
				{
					/* store error */
					errorCode = (int)(*stk(l2));
					strcpy(bufferErrorMessage,InputString_Parameter1[0]);
					freeArrayOfString(InputString_Parameter1,m1*n1);
					C2F(iop).err = errorPosition;
					SciError(errorCode);
				}
				else
				{
					freeArrayOfString(InputString_Parameter1,m1*n1);
					Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
				}
			}
			else
			{
				freeArrayOfString(InputString_Parameter1,m1*n1);
				Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,1);
			}
			return 0;
		}

		/* error(code,pos) */
		if ( ( GetType(1) == sci_matrix ) && ( GetType(2) == sci_matrix ) )
		{
			int m1 = 0, n1 = 0, l1 = 0;
			int m2 = 0, n2 = 0, l2 = 0;

			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
			/* check scalar */
			if ( (m1 == 1) && (n1 == 1) )
			{
				GetRhsVar(2,MATRIX_OF_DOUBLE_DATATYPE, &m2, &n2, &l2);
				/* check scalar */
				if ( (m2 == 1) && (n2 == 1) )
				{
					/* store error */
					errorCode = (int)(*stk(l1));
					errorPosition = (int)(*stk(l2));

					strcpy(bufferErrorMessage,defaultErrorMessage);
					C2F(iop).err = errorPosition;
					SciError(errorCode);
				}
				else
				{
					Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,2);
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
			}
			return 0;
		}

		/* error(code error,'message') */
		if ( ( GetType(1) == sci_matrix ) && ( GetType(2) == sci_strings ) )
		{
			int m1 = 0, n1 = 0, l1 = 0;

			int m2 = 0; int n2 = 0;
			char **InputString_Parameter2 = NULL;

			GetRhsVar(1,MATRIX_OF_DOUBLE_DATATYPE, &m1, &n1, &l1);
			/* check scalar */
			if ( (m1 == 1) && (n1 == 1) )
			{
				GetRhsVar(2,MATRIX_OF_STRING_DATATYPE,&m2,&n2,&InputString_Parameter2);
				/* check scalar */
				if ( (m2 == 1) && (n2 == 1) )
				{
					/* store error */
					errorCode = (int)(*stk(l1));
					Scierror(errorCode,InputString_Parameter2[0]);
					freeArrayOfString(InputString_Parameter2,m2*n2);
				}
				else
				{
					freeArrayOfString(InputString_Parameter2,m2*n2);
					Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
				}
			}
			else
			{
				freeArrayOfString(InputString_Parameter2,m2*n2);
				Scierror(999,_("%s: Wrong size for input argument #%d: A scalar expected.\n"),fname,1);
			}

			return 0;
		}

		Scierror(999,_("%s: Wrong type for input argument.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
