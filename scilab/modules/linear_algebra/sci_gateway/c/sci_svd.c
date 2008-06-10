
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include <stdio.h>
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intdgesvd1)(char *fname, unsigned long fname_len);
extern int C2F(intzgesvd1)(char *fname, unsigned long fname_len);
extern int C2F(intdgesvd2)(char *fname, unsigned long fname_len);
extern int C2F(intzgesvd2)(char *fname, unsigned long fname_len);
extern int C2F(intdoldsvd)(double *tol, char *fname, unsigned long fname_len);
extern int C2F(intzoldsvd)(double *tol, char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(intsvd)(char *fname,unsigned long fname_len)
{
	int *header1;int *header2;
	int Cmplx;int ret;double tol;

	if (GetType(1)!=sci_matrix) 
	{
		OverLoad(1);
		return 0;
	}
	header1 = (int *) GetData(1);
	Cmplx=header1[3];

	switch (Rhs) 
	{
		case 1:   /* s=svd(A)   or   [U,s,V]=svd(A)   */
		switch (Lhs) 
		{
			case 1: case 2: case 3:
				if (Cmplx==0) 
				{
					ret = C2F(intdgesvd1)("svd",3L);
					return 0; 
				}
				if (Cmplx==1) 
				{
					ret = C2F(intzgesvd1)("svd",3L);
					return 0; 
				}
			break;

			case 4:
				if (Cmplx==0) 
				{
					ret = C2F(intdoldsvd)((tol=0,&tol),"svd",3L);
					return 0;
				}
				if (Cmplx==1) 
				{
					ret = C2F(intzoldsvd)((tol=0,&tol),"svd",3L);
					return 0;
				}
			break;
	 }
	 case 2 :   /* svd(A, something)   */
		header2 = (int *) GetData(2);
		switch (header2[0]) 
		{
			case SCI_DOUBLE :
			if (Lhs == 4) 
			{
				/*  old svd, tolerance is passed: [U,S,V,rk]=svd(A,tol)  */
				/*   ret = C2F(intsvdold)("svd",2L);  */
				if (Cmplx==0) 
				{
					tol = ((double *) header2)[2];
					ret = C2F(intdoldsvd)(&tol,"svd",3L);
					return 0;
				}
				if (Cmplx==1) 
				{
					tol = ((double *) header2)[2];
					ret = C2F(intzoldsvd)(&tol,"svd",3L);
					return 0;
				}
			}
			else 
			{
				/* old Economy size:  [U,S,V]=svd(A,0)  */
				if (Cmplx==0) 
				{
					ret = C2F(intdgesvd2)("svd",3L);
					return 0;
				}
				if (Cmplx==1) 
				{
					ret = C2F(intzgesvd2)("svd",3L);
					return 0;
				}
			}
			break;

			case STRING  :
			/* Economy size:  [U,S,V]=svd(A,"e")  */
			if (Cmplx==0) 
			{
				ret = C2F(intdgesvd2)("svd",3L);
				return 0;
			}
			if (Cmplx==1) 
			{
				ret = C2F(intzgesvd2)("svd",3L);
				return 0;
			}
			break;
	  }
	  break;

	  default :   /*  rhs > 2 */
		Scierror(999,("%s: Wrong number of input argument(s).\n"),fname);
	  break;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
