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
#include "stdlib.h"

/*--------------------------------------------------------------------------*/
extern int C2F(intacos) _PARAMS((int *id));

extern double dacoss(double _dblVal);
extern void wacos(double _dblReal, double _dblImg, double *_pdblReal, double *_pdblImg);

/*--------------------------------------------------------------------------*/
int C2F(sci_acos) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex;
	
	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		//Voir comment faire l'appel a une fonction 'client dans le cas ou les variables ne sont pas des matrices de doubles.
		return 0;
	}

	if(iIsComplex(1))
	{// case complex
		double *pdblRealData	= 0;
		double *pdblImgData		= 0;
		double *pReturnRealData	= NULL;
		double *pReturnImgData	= NULL;
		int		iComplex		= 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);

		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
		pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; iIndex < iRows * iCols ; iIndex++)
			wacos(pdblRealData[iIndex], pdblImgData[iIndex], &pReturnRealData[iIndex], &pReturnImgData[iIndex]);

		CreateCVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = 2;
		PutLhsVar();
		free(pReturnRealData);
		free(pReturnImgData);
	}
	else
	{// case real
		double *pdblRealData	= 0;
		double *pReturnRealData = NULL;
		double *pReturnImgData	= NULL;
		int		itr				= 0;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);
		pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));

		//check if all variables are between [-1,1]
		for(iIndex = 0 ; (iIndex < iRows * iCols) && (itr == 0) ; iIndex++)
			if(fabs(pdblRealData[iIndex]) > 1)
				itr = 1;


		if(itr == 0)
		{//all values are in [-1,1]
			for(iIndex = 0 ; (iIndex < iRows * iCols) && (itr == 0) ; iIndex++)
				pReturnRealData[iIndex] = dacoss(pdblRealData[iIndex]);

			CreateVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
			LhsVar(1) = 2;
			PutLhsVar();
			free(pReturnRealData);
		}
		else
		{// Values outside [-1,1]
			//return complex double matrix.
			int iComplex = 1;
			pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

			for(iIndex = 0 ; (iIndex < iRows * iCols) ; iIndex++)
				wacos(pdblRealData[iIndex], 0, &pReturnRealData[iIndex], &pReturnImgData[iIndex]);

			CreateCVarFromPtr(2, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
			LhsVar(1) = 2;
			PutLhsVar();
			free(pReturnRealData);
			free(pReturnImgData);
		}
	}
#else
	C2F(intacos)(id);
#endif
	return 0;
}
/*--------------------------------------------------------------------------*/