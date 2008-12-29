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

#include "types_multiplication.hxx"

extern "C"
{
	#include "matrix_multiplication.h"
}

Double* MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2)
{
	bool bComplex1		= _pDouble1->isComplex();
	bool bComplex2		= _pDouble2->isComplex();
	bool bScalar1			= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
	bool bScalar2			= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;


	//Output variables
	Double *pResult		= NULL;
	int iRowsOut			= 0;
	int iColsOut			= 0;
	bool bComplexOut	= bComplex1 || bComplex2;
	double *pReal			= NULL;
	double *pImg			= NULL;

	if(bScalar1)
	{//cst*b
		iRowsOut				= _pDouble2->rows_get();
		iColsOut				= _pDouble2->cols_get();
		if(bComplexOut)
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal, &pImg);
		}
		else
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal);
		}

		if(bComplex1 == false && bComplex2 == false)
		{
			iMultiRealScalarByRealMatrix(_pDouble1->real_get(0,0), _pDouble2->real_get(), iRowsOut, iColsOut, pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iMultiRealScalarByComplexMatrix(_pDouble1->real_get(0,0), _pDouble2->real_get(), _pDouble2->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiComplexScalarByRealMatrix(_pDouble1->real_get(0,0), _pDouble1->img_get(0,0), _pDouble2->real_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexScalarByComplexMatrix(_pDouble1->real_get(0,0), _pDouble1->img_get(0,0), _pDouble2->real_get(), _pDouble2->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
	}
	else if(bScalar2)
	{//a * cst
		iRowsOut				= _pDouble1->rows_get();
		iColsOut				= _pDouble1->cols_get();

		if(bComplexOut)
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal, &pImg);
		}
		else
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal);
		}

		if(bComplex1 == false && bComplex2 == false)
		{//Real Matrix by Real Scalar
			iMultiRealScalarByRealMatrix(_pDouble2->real_get(0,0), _pDouble1->real_get(), iRowsOut, iColsOut, pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{//Real Matrix by Scalar Complex
			iMultiComplexScalarByRealMatrix(_pDouble2->real_get(0,0), _pDouble2->img_get(0,0), _pDouble1->real_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiRealScalarByComplexMatrix(_pDouble2->real_get(0,0), _pDouble1->real_get(), _pDouble1->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexScalarByComplexMatrix(_pDouble2->real_get(0,0), _pDouble2->img_get(0,0), _pDouble1->real_get(), _pDouble1->img_get(), iRowsOut, iColsOut, pReal, pImg);
		}
	}
	else
	{//a * b
		if(_pDouble1->cols_get() != _pDouble2->rows_get())
		{
			return NULL;
		}

		iRowsOut				= _pDouble1->rows_get();
		iColsOut				= _pDouble2->cols_get();

		if(bComplexOut)
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal, &pImg);
		}
		else
		{
			pResult 			= new Double(iRowsOut, iColsOut, &pReal);
		}

		if(bComplex1 == false && bComplex2 == false)
		{//Real Matrix by Real Matrix
			iMultiRealMatrixByRealMatrix(
									_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
									_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
									pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{//Real Matrix by Matrix Complex
			iMultiRealMatrixByComplexMatrix(
									_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
									_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
									pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{//Complex Matrix by Real Matrix
			iMultiComplexMatrixByRealMatrix(
									_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
									_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
									pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{//Complex Matrix by Complex Matrix
			iMultiComplexMatrixByComplexMatrix(
									_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
									_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
									pReal, pImg);
		}
	}

	return pResult;
}

/**
[1,2;3,4] * [%s,%s;%s,%s]
**/
MatrixPoly* MultiplyDoubleByPoly(Double* _pDouble, MatrixPoly* _pPoly)
{
	bool bComplex1 	= _pDouble->isComplex();
	bool bComplex2 	= _pPoly->isComplex();
	bool bScalar1		= _pDouble->rows_get() == 1 && _pDouble->cols_get() == 1;
	bool bScalar2		= _pPoly->rows_get() == 1 && _pPoly->cols_get() == 1;

	//Output variables
	MatrixPoly *pResult	= NULL;
	int iRowsOut				= 0;
	int iColsOut				= 0;
	bool bComplexOut		= bComplex1 || bComplex2;

	if(bScalar1)
	{//cst * p
		pResult 	= new MatrixPoly();
		//Create new PolyMatrix
		*pResult 	= *_pPoly;
		if(bComplex2 == false && bComplexOut == true)
		{
			pResult->complex_set(true);
		}

		for(int i = 0 ; i < _pPoly->size_get() ; i++)
		{
			Poly *pPolyIn			= _pPoly->poly_get(i);
			double* pRealIn		= pPolyIn->coef_get()->real_get();
			double* pImgIn		= pPolyIn->coef_get()->img_get();

			Poly *pPolyOut		= pResult->poly_get(i);
			double* pRealOut	= pPolyOut->coef_get()->real_get();
			double* pImgOut		= pPolyOut->coef_get()->img_get();

			if(bComplex1 == false && bComplex2 == false)
			{
				iMultiRealScalarByRealMatrix(
								_pDouble->real_get(0,0),
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut);
			}
			else if(bComplex1 == false && bComplex2 == true)
			{
				iMultiRealScalarByComplexMatrix(
								_pDouble->real_get(0,0),
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == false)
			{
				iMultiComplexScalarByRealMatrix(
								_pDouble->real_get(0,0), _pDouble->img_get(0,0),
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == true)
			{
				iMultiComplexScalarByComplexMatrix(
								_pDouble->real_get(0,0), _pDouble->img_get(0,0),
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
		}
	}
	else if(bScalar2)
	{
		int *piRank = new int[_pDouble->size_get()];
		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			piRank[i] = _pPoly->poly_get(0)->rank_get();
		}

		pResult = new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pDouble->cols_get(), piRank);
		if(_pDouble->isComplex() || _pPoly->isComplex())
		{
			pResult->complex_set(true);
		}

		double *pDoubleR	= _pDouble->real_get();
		double *pDoubleI	= _pDouble->img_get();

		Poly *pPolyIn			= _pPoly->poly_get(0);
		double* pRealIn		= pPolyIn->coef_get()->real_get();
		double* pImgIn		= pPolyIn->coef_get()->img_get();



		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			Poly *pPolyOut		= pResult->poly_get(i);
			double* pRealOut	= pPolyOut->coef_get()->real_get();
			double* pImgOut		= pPolyOut->coef_get()->img_get();

			if(bComplex1 == false && bComplex2 == false)
			{
				iMultiRealScalarByRealMatrix(
								pDoubleR[i],
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut);
			}
			else if(bComplex1 == false && bComplex2 == true)
			{
				iMultiRealScalarByComplexMatrix(
								pDoubleR[i],
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == false)
			{
				iMultiComplexScalarByRealMatrix(
								pDoubleR[i], pDoubleI[i],
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == true)
			{
				iMultiComplexScalarByComplexMatrix(
								pDoubleR[i], pDoubleI[i],
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
		}
		delete piRank;
	}
	else if(_pDouble->cols_get() == _pPoly->rows_get())
	{
		int iMaxRank 	= _pPoly->rank_max_get();
		int *piRank		= new int[_pDouble->rows_get() * _pPoly->cols_get()];

		for(int i = 0 ; i < _pDouble->rows_get() * _pPoly->cols_get() ; i++)
		{
			piRank[i] = iMaxRank;
		}

		Double *pCoef	= _pPoly->coef_get();
		pResult 			= new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pPoly->cols_get(), piRank);
		Double *pTemp	= new Double(_pDouble->rows_get(), pCoef->cols_get(), false);

		if(bComplex1 == true || bComplex2 == true)
		{
			pResult->complex_set(true);
			pTemp->complex_set(true);
		}


		if(bComplex1 == false && bComplex2 == false)
		{
			iMultiRealMatrixByRealMatrix(
							_pDouble->real_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get());

			pResult->coef_set(pTemp);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iMultiRealMatrixByComplexMatrix(
							_pDouble->real_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->img_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

			pResult->coef_set(pTemp);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiComplexMatrixByRealMatrix(
							_pDouble->real_get(), _pDouble->img_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

			pResult->coef_set(pTemp);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexMatrixByComplexMatrix(
							_pDouble->real_get(), _pDouble->img_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->img_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

			pResult->coef_set(pTemp);
		}
		delete piRank;
		delete pTemp;
	}

	return pResult;
}

MatrixPoly* MultiplyPolyByDouble(MatrixPoly* _pPoly, Double* _pDouble)
{
	bool bComplex1 	= _pPoly->isComplex();
	bool bComplex2 	= _pDouble->isComplex();
	bool bScalar1		= _pPoly->rows_get() == 1 && _pPoly->cols_get() == 1;
	bool bScalar2		= _pDouble->rows_get() == 1 && _pDouble->cols_get() == 1;

	//Output variables
	MatrixPoly *pResult	= NULL;
	int iRowsOut				= 0;
	int iColsOut				= 0;
	bool bComplexOut		= bComplex1 || bComplex2;

	if(bScalar1)
	{//p[1] * A
		std::cout << "1" << std::endl;
		int *piRank = new int[_pDouble->size_get()];
		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			piRank[i] = _pPoly->poly_get(0)->rank_get();
		}

		std::cout << "2" << std::endl;
		pResult = new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pDouble->cols_get(), piRank);
		if(_pDouble->isComplex() || _pPoly->isComplex())
		{
			pResult->complex_set(true);
		}

		std::cout << "3" << std::endl;
		double *pDoubleR	= _pDouble->real_get();
		double *pDoubleI	= _pDouble->img_get();

		Poly *pPolyIn			= _pPoly->poly_get(0);
		double* pRealIn		= pPolyIn->coef_get()->real_get();
		double* pImgIn		= pPolyIn->coef_get()->img_get();

		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			std::cout << "4" << std::endl;
			Poly *pPolyOut		= pResult->poly_get(i);
			double* pRealOut	= pPolyOut->coef_get()->real_get();
			double* pImgOut		= pPolyOut->coef_get()->img_get();

			if(bComplex1 == false && bComplex2 == false)
			{
				iMultiRealScalarByRealMatrix(
								pDoubleR[i],
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut);
			}
			else if(bComplex1 == false && bComplex2 == true)
			{
				iMultiComplexScalarByRealMatrix(
								pDoubleR[i], pDoubleI[i],
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == false)
			{
				iMultiRealScalarByComplexMatrix(
								pDoubleR[i],
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == true)
			{
				iMultiComplexScalarByComplexMatrix(
								pDoubleR[i], pDoubleI[i],
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
		}
		delete piRank;
	}
	else if(bScalar2)
	{//p * cst
		pResult 	= new MatrixPoly();
		//Create new PolyMatrix
		*pResult 	= *_pPoly;
		if(bComplex2 == false && bComplexOut == true)
		{
			pResult->complex_set(true);
		}

		for(int i = 0 ; i < _pPoly->size_get() ; i++)
		{
			Poly *pPolyIn			= _pPoly->poly_get(i);
			double* pRealIn		= pPolyIn->coef_get()->real_get();
			double* pImgIn		= pPolyIn->coef_get()->img_get();

			Poly *pPolyOut		= pResult->poly_get(i);
			double* pRealOut	= pPolyOut->coef_get()->real_get();
			double* pImgOut		= pPolyOut->coef_get()->img_get();

			if(bComplex1 == false && bComplex2 == false)
			{
				iMultiRealScalarByRealMatrix(
								_pDouble->real_get(0,0),
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut);
			}
			else if(bComplex1 == false && bComplex2 == true)
			{
				iMultiComplexScalarByRealMatrix(
								_pDouble->real_get(0,0), _pDouble->img_get(0,0),
								pRealIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == false)
			{
				iMultiRealScalarByComplexMatrix(
								_pDouble->real_get(0,0),
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
			else if(bComplex1 == true && bComplex2 == true)
			{
				iMultiComplexScalarByComplexMatrix(
								_pDouble->real_get(0,0), _pDouble->img_get(0,0),
								pRealIn, pImgIn, 1, pPolyIn->rank_get(),
								pRealOut, pImgOut);
			}
		}
	}
	else if(_pPoly->cols_get() == _pDouble->rows_get())
	{

		//Distribution a la mano par appels a des sous-fonctions ( iMulti...ScalarBy...Scalar ) plus iAdd...To... )
		int iMaxRank 	= _pPoly->rank_max_get();
		int *piRank		= new int[_pPoly->rows_get() * _pDouble->cols_get()];

		for(int i = 0 ; i < _pPoly->rows_get() * _pDouble->cols_get() ; i++)
		{
			piRank[i] = iMaxRank;
		}

		Double *pCoef	= _pPoly->coef_get();
		pResult 			= new MatrixPoly(_pPoly->var_get(), _pPoly->rows_get(), _pDouble->cols_get(), piRank);
		Double *pTemp	= new Double(pCoef->rows_get(), _pDouble->cols_get(), false);

		if(bComplex1 == true || bComplex2 == true)
		{
			pResult->complex_set(true);
			pTemp->complex_set(true);
		}


		if(bComplex1 == false && bComplex2 == false)
		{
			iMultiRealMatrixByRealMatrix(
							pCoef->real_get(), pCoef->rows_get(), pCoef->cols_get(),
							_pDouble->real_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pTemp->real_get());

			pResult->coef_set(pTemp);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iMultiRealMatrixByComplexMatrix(
							_pDouble->real_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->img_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

			pResult->coef_set(pTemp);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiComplexMatrixByRealMatrix(
							_pDouble->real_get(), _pDouble->img_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

			pResult->coef_set(pTemp);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexMatrixByComplexMatrix(
							_pDouble->real_get(), _pDouble->img_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->img_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

			pResult->coef_set(pTemp);
		}
		delete piRank;
		delete pTemp;
	}

	return pResult;
}
