/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 

#include "gw_elementary_functions.h"
#include "stack-c.h"
#include "basic_functions.h"
#include "../../core/src/c/parse.h"
#include "Scierror.h"
#include "api_scilab.h"

/*--------------------------------------------------------------------------*/
int C2F(sci_atan) (char *fname,unsigned long fname_len)
{
	int i;
	int iRet								= 0;
	int iRows1							= 0;
	int iCols1							= 0;
	int iRows2 							= 0;
	int iCols2 							= 0;
	
	int* piAddr1					= NULL;
	int* piAddr2					= NULL;

	double *pdblReal1			= NULL;
	double *pdblImg1			= NULL;
	double *pdblReal2			= NULL;
	double *pdblImg2			= NULL;
	double *pdblRealRet		= NULL;
	double *pdblImgRet		= NULL;

	CheckRhs(1,2);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr1);
	if(getVarType(piAddr1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(Rhs == 1)
	{
		if(isVarComplex(piAddr1))
		{// case complex
			iRet = getComplexMatrixOfDouble(piAddr1, &iRows1, &iCols1, &pdblReal1, &pdblImg2);
			if(iRet)
			{
				return 1;
			}
			
			iRet = allocComplexMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pdblRealRet, &pdblImgRet);
			if(iRet)
			{
				return 1;
			}

			for(i = 0 ; i < iRows1 * iCols1 ; i++)
			{
				if(pdblReal1[i] == 0 && dabss(pdblImg1[i]) == 1)
				{
					if(C2F(errgst).ieee==0)
						Error(32);
					else if(C2F(errgst).ieee==1)
						Msgs(64,0);
				}
				else
				{
					watan(pdblReal1[i], pdblImg1[i], &pdblRealRet[i], &pdblImgRet[i]);
				}
			}
		}
		else
		{// case real
			iRet = getMatrixOfDouble(piAddr1, &iRows1, &iCols1, &pdblReal1);
			if(iRet)
			{
				return 1;
			}

			allocMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pdblRealRet);
			for(i = 0 ; i < iRows1 * iCols1 ; i++)
			{
				pdblRealRet[i] = datans(pdblReal1[i]);
			}
		}
	}
	else
	{//Rhs == 2
		iRet = getVarAddressFromPosition(2, &piAddr2);
		if(iRet)
		{
			return 1;
		}

		if(isVarComplex(piAddr1) == FALSE && isVarComplex(piAddr2) == FALSE)
		{//Only works with real matrix
			iRet = getMatrixOfDouble(piAddr1, &iRows1, &iCols1, &pdblReal1);
			if(iRet)
			{
				return 1;
			}

			iRet = getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal2);
			if(iRet)
			{
				return 1;
			}

			if(iRows1 * iCols1 == iRows2 * iCols2)
			{
				allocMatrixOfDouble(Rhs + 1, iRows1, iCols1, &pdblRealRet);
				for(i = 0 ; i < iRows1 * iCols1 ; i++)
				{
					pdblRealRet[i] = datan2s(pdblReal1[i], pdblReal2[i]);
				}
			}
			else
				Error(60);
		}
		else
			Error(43);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
