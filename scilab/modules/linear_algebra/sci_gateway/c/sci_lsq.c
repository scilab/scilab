
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
extern int C2F(intdgelsy)(char *fname, unsigned long fname_len);
extern int C2F(intzgelsy)(char *fname, unsigned long fname_len);

/*--------------------------------------------------------------------------*/
int C2F(intlsq)(char *fname,unsigned long fname_len)
{
	int *header1;int *header2;
	int CmplxA;int Cmplxb;int ret;int I2;

	/*   lsq(A,b)  */
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
	header1 = (int *) GetData(1);    header2 = (int *) GetData(2);
	CmplxA=header1[3];   Cmplxb=header2[3];
	switch (CmplxA) 
	{
		case REAL:
			switch (Cmplxb) 
			{
				case REAL :
				/* A real, b real */
				ret = C2F(intdgelsy)("lsq",3L);
				break;

				case COMPLEX :
				/* A real, b complex */
				C2F(complexify)((I2=1,&I2));
				ret = C2F(intzgelsy)("lsq",3L);
				break;

				default:
				break;
			}
			return 0;

		case COMPLEX :
			switch (Cmplxb) 
			{
				case REAL :
				/* A complex, b real */
				C2F(complexify)((I2=2,&I2));
				ret = C2F(intzgelsy)("lsq",3L);
				break;

				case COMPLEX :
				/* A complex, b complex */
				ret = C2F(intzgelsy)("lsq",3L);
				break;

				default:
					Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
					fname,2);
				break;
			}
			return 0;
		break;

		default :
			Scierror(999,_("%s: Wrong type for input argument #%d: Real or Complex matrix expected.\n"),
			fname,1);
			return 0;
		break;
	}
}
/*--------------------------------------------------------------------------*/
