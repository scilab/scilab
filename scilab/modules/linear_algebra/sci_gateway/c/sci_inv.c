/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Bernard HUGUENEY
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/
#include "api_common.h"
#include "api_double.h"

#include <string.h>
#include <stdio.h> /* sprintf */
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "localization.h"

#include "stack3.h" /* */
#include "sciprint.h"
#include "Scierror.h"
#include "invert_matrix.h"

#include "msgs.h"

int sci_inv(char *fname, int* _piKey)
{
	int* arg;
	int ret = 0;
	int type;
	/*   inv(A)  */
	if(Rhs >= 1)
	{
		getVarAddressFromPosition(_piKey, 1, &arg);
		getVarType(_piKey, arg, &type);
		if(type !=sci_matrix)
		{
			OverLoad(1);
			return 0;
		}

		/* from now on, we have an sci_matrix, so we can use iIsComplex */
		CheckRhs(1,1); /* one and only one arg */
		CheckLhs(1,1); /* one and only one returned value */

		{
			int iRows, iCols;
			double* pData;
			double* pDataReal;
			double* pDataImg;
			int complexArg=isVarComplex(_piKey, arg);
			if(complexArg)
			{
				getComplexMatrixOfDouble(_piKey, arg, &iRows, &iCols, &pDataReal, &pDataImg);

				/* c -> z */
				pData=(double*)oGetDoubleComplexFromPointer( pDataReal, pDataImg, iRows * iCols);
			}
			else
			{
				getMatrixOfDouble(_piKey, arg, &iRows, &iCols, &pData);
			}

			if(iRows != iCols)
			{
				Scierror(20, _("%s: Wrong type for input argument #%d: Square matrix expected.\n"), fname, 1);
				ret=20;
			}
			else
			{
				if(iCols != 0)
				{
					if( iCols == -1)
					{
						*pData = 1./(*pData);
					}
					else
					{
						double dblRcond;
						ret = iInvertMatrixM(iRows, iCols, pData, complexArg, &dblRcond);
						if(complexArg)
						{
							/* z -> c */
							vGetPointerFromDoubleComplex((doublecomplex*)pData, iCols * iRows, pDataReal, pDataImg);
							vFreeDoubleComplexFromPointer((doublecomplex*)pData);
						}

						if(ret ==-1)
						{
							sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
							Msgs(5,1);
						}
					}
				}
			}

			if(ret >0)
			{
				Error(ret) ;
			}
			else
			{
				LhsVar(1) = 1;
				/* TODO rajouter le PutLhsVar(); quand il sera enlevé du gw_  */
			}
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
