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
#include "elem_func.hxx"

extern "C"
{
	#include "cos.h"
	#include "gw_elementary_functions.h"
	#include "stack-c.h"
}

using namespace types;

/*--------------------------------------------------------------------------*/
extern int C2F(intcos) (int *id);
/*--------------------------------------------------------------------------*/


int C2F(sci_cos) (char *fname,unsigned long fname_len)
{
	static int id[6];
	int iRows = 0;
	int iCols = 0;
	int iRealData = 0;
	int iImgData = 0;
	int iIndex;

	CheckRhs(1,1);
	CheckLhs(1,1);

	if(GetType(1) != sci_matrix)
	{
		OverLoad(1);
		return 0;
	}

	if(iIsComplex(1))
	{
		double *pdblRealData = 0;
		double *pdblImgData = 0;
		double *pReturnRealData = NULL;
		double *pReturnImgData = NULL;
		int iComplex = 1;

		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &iRealData, &iImgData);
		pdblRealData	= stk(iRealData);
		pdblImgData		= stk(iImgData);

		iAllocComplexMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData, &pReturnImgData);
		//pReturnRealData = (double*)malloc(iRows * iCols * sizeof(double));
		//pReturnImgData	= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; iIndex < iCols * iRows ; iIndex++)
			zcoss(pdblRealData[iIndex], pdblImgData[iIndex], &pReturnRealData[iIndex], &pReturnImgData[iIndex]);

		//CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex, &iRows, &iCols, &pReturnRealData, &pReturnImgData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
		//free(pReturnImgData);
	}
	else
	{
		double *pdblRealData = 0;
		double *pReturnRealData = NULL;

		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &iRealData);
		pdblRealData		= stk(iRealData);

		iAllocMatrixOfDouble(Rhs + 1, iRows, iCols, &pReturnRealData);
		//pReturnRealData		= (double*)malloc(iRows * iCols * sizeof(double));

		for(iIndex = 0 ; iIndex < iCols * iRows ; iIndex++)
			pReturnRealData[iIndex] = dcoss(pdblRealData[iIndex]);

		//CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iRows, &iCols, &pReturnRealData);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		//free(pReturnRealData);
	}
	return 0;
}

//new sci_cos interface ofr scilab 6
Function::ReturnValue sci_cos(types::typed_list &in, int* _piRetCount, types::typed_list &out)
{
	double *pDataInR	= NULL;
	double *pDataInI	= NULL;
	double *pDataOutR = NULL;
	double *pDataOutI = NULL;
	Double *pRetVal		= NULL;

	if(in.size() != 1)
	{
		return Function::WrongParamNumber;
	}

	if(in[0]->getType() != types::InternalType::RealDouble)
	{
		return Function::WrongParamType;
	}


	Double *pIn				= in[0]->getAsDouble();
	if(pIn->isComplex())
	{
		pDataInR	=	pIn->real_get();
		pDataInI	=	pIn->img_get();

		pRetVal = new Double(pIn->rows_get(), pIn->cols_get(), &pDataOutR, &pDataOutI);

		for(int i = 0 ; i < pIn->size_get() ; i++)
		{
			zcoss(pDataInR[i], pDataInI[i], &pDataOutR[i], &pDataOutI[i]);
		}
	}
	else
	{
		pDataInR	=	pIn->real_get();
		pRetVal = new Double(pIn->rows_get(), pIn->cols_get(), &pDataOutR);
		for(int i = 0 ; i < pIn->size_get() ; i++)
		{
			pDataOutR[i] = dcoss(pDataInR[i]);
		}
	}

	out.push_back(pRetVal);
	*_piRetCount = 1;
	return Function::AllGood;
}

/*--------------------------------------------------------------------------*/
