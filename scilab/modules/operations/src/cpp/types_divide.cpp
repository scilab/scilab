/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
*
*  This file must be used under the terms of the CeCILL.
*  This source file is licensed as described in the file COPYING, which
*  you should have received as part of this distribution.  The terms
*  are also available at
*  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "types_divide.hxx"
#include "types_finite.hxx"
extern "C"
{
	#include "matrix_right_division.h"
	#include "matrix_right_division.h"
}

int DivideDoubleByDouble(Double *_pDouble1, Double *_pDouble2, Double **_pDoubleOut)
{
	bool bComplex1		= _pDouble1->isComplex();
	bool bComplex2		= _pDouble2->isComplex();
	bool bScalar1			= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
	bool bScalar2			= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;

	//check finite values of _pDouble1 and _pDouble2
	if(isDoubleFinite(_pDouble1) == false || isDoubleFinite(_pDouble2) == false)
	{
		return 2;
	}

	if(_pDouble1->size_get() == 0 || _pDouble2->size_get() == 0)
	{
		//return an empty matrix of double
		*_pDoubleOut	= new Double(0,0);
	}
	else if(bScalar2)
	{//[] / x
		int iErr				= 0;
		int iRowResult	= _pDouble1->rows_get();
		int iColResult	= _pDouble1->cols_get();
		int iInc1				= 1;
		int iInc2				= 0;
		int iSizeResult	= iRowResult * iColResult;

		*_pDoubleOut = new Double(iRowResult, iColResult, _pDouble1->isComplex() || _pDouble2->isComplex());

		if(bComplex1 == false && bComplex2 == false)
		{// Real1 \ Real2 -> Real2 / Real1
			iErr = iRightDivisionRealMatrixByRealMatrix(
				_pDouble1->real_get(), iInc1, 
				_pDouble2->real_get(), iInc2, 
				(*_pDoubleOut)->real_get(), 1, iSizeResult);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{// Real \ Complex -> Complex / Real
			iErr = iRightDivisionRealMatrixByComplexMatrix(
				_pDouble1->real_get(), iInc1, 
				_pDouble2->real_get(), _pDouble2->img_get(), iInc2, 
				(*_pDoubleOut)->real_get(),	(*_pDoubleOut)->img_get(), 1, iSizeResult);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{// Complex \ Real -> Real / Complex
			iErr = iRightDivisionComplexMatrixByRealMatrix(
				_pDouble1->real_get(), _pDouble1->img_get(), iInc1, 
				_pDouble2->real_get(), iInc2,
				(*_pDoubleOut)->real_get(),	(*_pDoubleOut)->img_get(), 1, iSizeResult);
		}
		else if(bComplex1 == true && bComplex2 == true)
		{// Complex \ Complex
			iErr = iRightDivisionComplexMatrixByComplexMatrix(
				_pDouble1->real_get(), _pDouble1->img_get(), iInc1,
				_pDouble2->real_get(), _pDouble2->img_get(), iInc2, 
				(*_pDoubleOut)->real_get(),	(*_pDoubleOut)->img_get(), 1, iSizeResult);
		}
	}
	else if(bScalar1)
	{
		double dblSavedR = 0;
		double dblSavedI = 0;
		Double *pdblTemp = NULL;

		int iRowResult = _pDouble2->cols_get();
		int iColResult = _pDouble2->rows_get();

		//in this case, we have to create a temporary square matrix 
		pdblTemp	= new Double(iRowResult, iRowResult, _pDouble1->isComplex());
		pdblTemp->zero_set();

		if(_pDouble1->isComplex())
		{
			dblSavedR = _pDouble1->real_get()[0];
			dblSavedI = _pDouble1->img_get()[0];
			for(int i = 0 ; i < iRowResult ; i++)
			{
				pdblTemp->val_set(i,i,dblSavedR, dblSavedI);
			}
		}
		else
		{
			dblSavedR = _pDouble1->real_get()[0];
			for(int i = 0 ; i < iRowResult ; i++)
			{
				pdblTemp->val_set(i,i,dblSavedR);
			}
		}

		*_pDoubleOut = new Double(iRowResult, iColResult, _pDouble1->isComplex() || _pDouble2->isComplex());

		if((*_pDoubleOut)->isComplex())
		{
			double dblRcond = 0;
			iRightDivisionOfComplexMatrix(
					pdblTemp->real_get(), pdblTemp->img_get(), pdblTemp->rows_get(), pdblTemp->cols_get(),
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get(), iRowResult, iColResult, &dblRcond);
		}
		else
		{
			double dblRcond = 0;
			iRightDivisionOfRealMatrix(
					pdblTemp->real_get(), pdblTemp->rows_get(), pdblTemp->cols_get(),
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), iRowResult, iColResult, &dblRcond);
		}
		delete pdblTemp;
	}
	else if(_pDouble1->cols_get() == _pDouble2->cols_get())
	{
		*_pDoubleOut = new Double(_pDouble1->rows_get(), _pDouble2->rows_get(), _pDouble1->isComplex() || _pDouble2->isComplex());
		if((*_pDoubleOut)->isComplex())
		{
			double dblRcond = 0;
			iRightDivisionOfComplexMatrix(
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get(), _pDouble1->rows_get(), _pDouble2->rows_get(), &dblRcond);
		}
		else
		{
			double dblRcond = 0;
			iRightDivisionOfRealMatrix(
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					(*_pDoubleOut)->real_get(), _pDouble1->rows_get(), _pDouble2->rows_get(), &dblRcond);
		}
	}
	else
	{
		//Bad dimensions
		return 1;
	}

	return 0;
}

int DividePolyByDouble(MatrixPoly* _pPoly, Double* _pDouble, MatrixPoly** _pPolyOut)
{
	bool bComplex1		= _pPoly->isComplex();
	bool bComplex2		= _pDouble->isComplex();
	bool bScalar1			= _pPoly->rows_get() == 1		&& _pPoly->cols_get() == 1;
	bool bScalar2			= _pDouble->rows_get() == 1 && _pDouble->cols_get() == 1;

	MatrixPoly *pTemp	= NULL; //use only is _pPoly is scalar and _pDouble not.

	int iRowResult 	= 0;
	int iColResult	= 0;
	int *piRank			= NULL;

/*	if(bScalar1 && bScalar2)
	{
		iRowResult = 1;
		iColResult = 1;

		piRank = new int[1];
		piRank[0]	= _pPoly->poly_get(0)->rank_get();
	}
	else */if(bScalar2)
	{
		iRowResult = _pPoly->rows_get();
		iColResult = _pPoly->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly->poly_get(i)->rank_get();
		}
	}
	else if(bScalar1)
	{
		//in this case, we have to create a temporary square polinomial matrix 
		iRowResult = _pDouble->cols_get();
		iColResult = _pDouble->rows_get();

		piRank = new int[iRowResult * iRowResult];
		for(int i = 0 ; i < iRowResult * iRowResult ; i++)
		{
			piRank[i] = _pPoly->rank_max_get();
		}

		pTemp = new MatrixPoly(_pPoly->var_get(), iRowResult, iRowResult, piRank);
		if(bComplex1 || bComplex2)
		{
			pTemp->complex_set(true);
		}

		Double *pdblData = _pPoly->poly_get(0)->coef_get();
		for(int i = 0 ; i < iRowResult ; i++)
		{
			pTemp->poly_set(i,i, pdblData);
		}
	}
/*	else if(pR->size_get() == 1)
	{
		iRowResult = pL->rows_get();
		iColResult = pL->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = pL->poly_get(i)->rank_get();
		}
	}
	else if(pL->rows_get() == pR->rows_get() && pL->cols_get() == pR->cols_get())
	{//Je ne sais pas encore comment ca marche ce machin la !!!
		iRowResult = pR->rows_get();
		iColResult = pR->cols_get();
	}
*/

	(*_pPolyOut) = new MatrixPoly(_pPoly->var_get(), iRowResult, iColResult, piRank);
	delete[] piRank;
	if(bComplex1 || bComplex2)
	{
		(*_pPolyOut)->complex_set(true);
	}

	if(bScalar2)
	{//[p] * cst
		for(int i = 0 ; i < _pPoly->size_get() ; i++)
		{
			Poly *pPolyIn			= _pPoly->poly_get(i);
			double* pRealIn		= pPolyIn->coef_get()->real_get();
			double* pImgIn		= pPolyIn->coef_get()->img_get();

			Poly *pPolyOut		= (*_pPolyOut)->poly_get(i);
			double* pRealOut	= pPolyOut->coef_get()->real_get();
			double* pImgOut		= pPolyOut->coef_get()->img_get();
			
			if(bComplex1 == false && bComplex2 == false)
			{
				iRightDivisionRealMatrixByRealMatrix(
						pRealIn, 1,
						_pDouble->real_get(),	0, 
						pRealOut, 1, pPolyOut->rank_get());
			}
			else if(bComplex1 == false && bComplex2 == true)
			{
				iRightDivisionRealMatrixByComplexMatrix(
						pRealIn, 1,
						_pDouble->real_get(),	_pDouble->img_get(), 0, 
						pRealOut,	pImgOut, 1, pPolyOut->rank_get());
			}
			else if(bComplex1 == true && bComplex2 == false)
			{
				iRightDivisionComplexMatrixByRealMatrix(
						pRealIn, pImgIn, 1,
						_pDouble->real_get(), 0, 
						pRealOut, pImgOut, 1, pPolyOut->rank_get());
			}
			else if(bComplex1 == true && bComplex2 == true)
			{
				iRightDivisionComplexMatrixByComplexMatrix(
						pRealIn, pImgIn, 1,
						_pDouble->real_get(),	_pDouble->img_get(), 0, 
						pRealOut, pImgOut, 1, pPolyOut->rank_get());
			}
		}
	}
	else if(bScalar1)
	{
		Double *pResultCoef = new Double(iRowResult, iColResult, (*_pPolyOut)->isComplex());
		double *pReal				= pResultCoef->real_get();
		double *pImg				= pResultCoef->img_get();

		for(int i = 0 ; i < pTemp->poly_get(0)->rank_get() ; i++)
		{
			Double *pCoef				= pTemp->extract_coef(i);
			Double *pResultCoef = new Double(iRowResult, iColResult, pCoef->isComplex());
			double *pReal				= pResultCoef->real_get();
			double *pImg				= pResultCoef->img_get();

			if(bComplex1 == false && bComplex2 == false)
			{
				double dblRcond = 0;
				iRightDivisionOfRealMatrix(
						pCoef->real_get(), iRowResult, iRowResult,
						_pDouble->real_get(), _pDouble->rows_get(), _pDouble->cols_get(), 
						pReal, iRowResult, iColResult, &dblRcond);
			}
			else
			{
				double dblRcond = 0;
				iRightDivisionOfComplexMatrix(
						pCoef->real_get(), pCoef->img_get(), iRowResult, iRowResult,
						_pDouble->real_get(), _pDouble->img_get(), _pDouble->rows_get(), _pDouble->cols_get(), 
						pReal, pImg, iRowResult, iColResult, &dblRcond);
			}

			(*_pPolyOut)->insert_coef(i, pResultCoef);
			delete pCoef;
		}
		delete pResultCoef;
	}
	return 0;
}

int DivideDoubleByPoly(Double* _pDouble, MatrixPoly* _pPoly, MatrixPoly** _pPolyOut)
{
	return 0;
}
