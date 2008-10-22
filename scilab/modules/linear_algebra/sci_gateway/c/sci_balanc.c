
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
extern int C2F(complexify)(int *num);
extern int C2F(intdgebal)(char *fname, unsigned long fname_len);
extern int C2F(intzgebal)(char *fname, unsigned long fname_len);
extern int C2F(intdggbal)(char *fname, unsigned long fname_len);
extern int C2F(intzggbal)(char *fname, unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(intbalanc)(char *fname,unsigned long fname_len)
{
	int *header1, *header2;
	int CmplxA, CmplxB;
	int ret;int X;

	switch (Rhs) 
	{
		case 1:   /* balanc(A)   */
		if (GetType(1)!=sci_matrix) 
		{
		  OverLoad(1);
		  return 0;
		}
		header1 = (int *) GetData(1);
		CmplxA=header1[3];
		switch (CmplxA) 
		{
			case REAL:
				ret = C2F(intdgebal)("balanc",6L);
			break;

			case COMPLEX:
				ret = C2F(intzgebal)("balanc",6L);
			break;

			default:
				Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
						fname,1);
			break;
		} /* end switch  (CmplxA) */
		break; /* end case 1 */

		case 2: /* balanc(A,B) */
		if (GetType(1)!=sci_matrix) 
		{
		  OverLoad(1);
		  return 0;
		}
		if (GetType(2)!=sci_matrix) 
		{
		  OverLoad(2);
		  return 0;
		}
		header1 = (int *) GetData(1);
		header2 = (int *) GetData(2);
		CmplxA=header1[3];
		CmplxB=header2[3];
		switch (CmplxA) 
		{
			case REAL:
			switch (CmplxB) 
			{
				case REAL :
				/* A real, Breal */
				ret = C2F(intdggbal)("balanc",6L);
				break;

				case COMPLEX :
				/* A real, B complex : complexify A */
				C2F(complexify)((X=1,&X));
				ret = C2F(intzggbal)("balanc",6L);
				break;

				default:
					Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
					fname,1);

				break;
			}
			break;

			case COMPLEX :
			switch (CmplxB) 
			{
				case REAL :
				/* A complex, B real : complexify B */
				C2F(complexify)((X=2,&X));
				ret = C2F(intzggbal)("balanc",6L);
				break;

				case COMPLEX :
				/* A complex, B complex */
				ret = C2F(intzggbal)("balanc",6L);
				return 0;
				break;

				default:
					Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
					fname,2);
				break;
			}
			break;

			default :
				Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
				fname,1);
			break;
		} /*end  switch (CmplxA) */
		break;/* end case 2 */
	}/* end switch (Rhs) */
	return 0;
}
/*--------------------------------------------------------------------------*/
