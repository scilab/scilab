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
#include "types_addition.hxx"

extern "C"
{
	#include "matrix_multiplication.h"
	#include "matrix_addition.h"
	#include "operation_f.h"
}

int MultiplyDoubleByDouble(Double* _pDouble1, Double* _pDouble2, Double**	 _pDoubleOut)
{
	bool bComplex1		= _pDouble1->isComplex();
	bool bComplex2		= _pDouble2->isComplex();
	bool bScalar1			= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
	bool bScalar2			= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;

	int iRowResult 	= 0;
	int iColResult	= 0;

	if(bScalar1)
	{
		iRowResult = _pDouble2->rows_get();
		iColResult = _pDouble2->cols_get();
	}
	else if(bScalar2)
	{
		iRowResult = _pDouble1->rows_get();
		iColResult = _pDouble1->cols_get();
	}
	else if(_pDouble1->cols_get() == _pDouble2->rows_get())
	{
		iRowResult = _pDouble1->rows_get();
		iColResult = _pDouble2->cols_get();
	}
	else
	{
		return 1;
	}

	//Output variables
	bool bComplexOut	= bComplex1 || bComplex2;
	(*_pDoubleOut) = new Double(iRowResult, iColResult, bComplexOut);

	double *pReal			= (*_pDoubleOut)->real_get();
	double *pImg			= (*_pDoubleOut)->img_get();

	if(bScalar1)
	{//cst*b
		if(bComplex1 == false && bComplex2 == false)
		{
			iMultiRealScalarByRealMatrix(_pDouble1->real_get(0,0), _pDouble2->real_get(), iRowResult, iColResult, pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iMultiRealScalarByComplexMatrix(_pDouble1->real_get(0,0), _pDouble2->real_get(), _pDouble2->img_get(), iRowResult, iColResult, pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiComplexScalarByRealMatrix(_pDouble1->real_get(0,0), _pDouble1->img_get(0,0), _pDouble2->real_get(), iRowResult, iColResult, pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexScalarByComplexMatrix(_pDouble1->real_get(0,0), _pDouble1->img_get(0,0), _pDouble2->real_get(), _pDouble2->img_get(), iRowResult, iColResult, pReal, pImg);
		}
	}
	else if(bScalar2)
	{//a * cst
		if(bComplex1 == false && bComplex2 == false)
		{//Real Matrix by Real Scalar
			iMultiRealScalarByRealMatrix(_pDouble2->real_get(0,0), _pDouble1->real_get(), iRowResult, iColResult, pReal);
		}
		else if(bComplex1 == false && bComplex2 == true)
		{//Real Matrix by Scalar Complex
			iMultiComplexScalarByRealMatrix(_pDouble2->real_get(0,0), _pDouble2->img_get(0,0), _pDouble1->real_get(), iRowResult, iColResult, pReal, pImg);
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiRealScalarByComplexMatrix(_pDouble2->real_get(0,0), _pDouble1->real_get(), _pDouble1->img_get(), iRowResult, iColResult, pReal, pImg);
		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexScalarByComplexMatrix(_pDouble2->real_get(0,0), _pDouble2->img_get(0,0), _pDouble1->real_get(), _pDouble1->img_get(), iRowResult, iColResult, pReal, pImg);
		}
	}
	else if(_pDouble1->cols_get() == _pDouble2->rows_get())
	{//a * b
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
	else
	{
		return 1;
	}
	return 0;
}

int MultiplyDoubleByPoly(Double* _pDouble, MatrixPoly* _pPoly, MatrixPoly** _pPolyOut)
{
	bool bComplex1 	= _pDouble->isComplex();
	bool bComplex2 	= _pPoly->isComplex();
	bool bScalar1		= _pDouble->rows_get() == 1 && _pDouble->cols_get() == 1;
	bool bScalar2		= _pPoly->rows_get() == 1 && _pPoly->cols_get() == 1;

	int iRowResult 	= 0;
	int iColResult	= 0;
	int *piRank			= NULL;

	if(bScalar1)
	{
		iRowResult = _pPoly->rows_get();
		iColResult = _pPoly->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly->poly_get(i)->rank_get();
		}
	}
	else if (bScalar2)
	{
		iRowResult = _pDouble->rows_get();
		iColResult = _pDouble->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly->poly_get(0)->rank_get();
		}
	}
	else if(_pDouble->cols_get() == _pPoly->rows_get())
	{
		iRowResult = _pDouble->rows_get();
		iColResult = _pPoly->cols_get();
		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly->rank_max_get();
		}
	}
	else
	{
		return 1;
	}

	(*_pPolyOut) = new MatrixPoly(_pPoly->var_get(), iRowResult, iColResult, piRank);
	delete[] piRank;
	if(bComplex1 || bComplex2)
	{
		(*_pPolyOut)->complex_set(true);
	}

	if(bScalar1)
	{//cst * p
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
		double *pDoubleR	= _pDouble->real_get();
		double *pDoubleI	= _pDouble->img_get();

		Poly *pPolyIn			= _pPoly->poly_get(0);
		double* pRealIn		= pPolyIn->coef_get()->real_get();
		double* pImgIn		= pPolyIn->coef_get()->img_get();

		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			Poly *pPolyOut		= (*_pPolyOut)->poly_get(i);
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
	}
	else if(_pDouble->cols_get() == _pPoly->rows_get())
	{
		Double *pCoef	= _pPoly->coef_get();
		Double *pTemp	= new Double(_pDouble->rows_get(), pCoef->cols_get(), bComplex1 || bComplex2);

		if(bComplex1 == false && bComplex2 == false)
		{
			iMultiRealMatrixByRealMatrix(
							_pDouble->real_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get());

		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			iMultiRealMatrixByComplexMatrix(
							_pDouble->real_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->img_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			iMultiComplexMatrixByRealMatrix(
							_pDouble->real_get(), _pDouble->img_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

		}
		else //if(bComplex1 == true && bComplex2 == true)
		{
			iMultiComplexMatrixByComplexMatrix(
							_pDouble->real_get(), _pDouble->img_get(), _pDouble->rows_get(), _pDouble->cols_get(),
							pCoef->real_get(), pCoef->img_get(), pCoef->rows_get(), pCoef->cols_get(),
							pTemp->real_get(), pTemp->img_get());

		}
		(*_pPolyOut)->coef_set(pTemp);
		delete pTemp;
	}
	else
	{
		return 1;
	}

	return 0;
}

int MultiplyPolyByDouble(MatrixPoly* _pPoly, Double* _pDouble, MatrixPoly **_pPolyOut)
{
	bool bComplex1 	= _pPoly->isComplex();
	bool bComplex2 	= _pDouble->isComplex();
	bool bScalar1		= _pPoly->rows_get() == 1 && _pPoly->cols_get() == 1;
	bool bScalar2		= _pDouble->rows_get() == 1 && _pDouble->cols_get() == 1;

	int iRowResult 	= 0;
	int iColResult	= 0;
	int *piRank			= NULL;

	if(bScalar1)
	{
		iRowResult = _pDouble->rows_get();
		iColResult = _pDouble->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly->poly_get(0)->rank_get();
		}
	}
	else if (bScalar2)
	{
		iRowResult = _pPoly->rows_get();
		iColResult = _pPoly->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly->poly_get(i)->rank_get();
		}
	}
	else if(_pPoly->cols_get() == _pDouble->rows_get())
	{
		iRowResult = _pPoly->rows_get();
		iColResult = _pDouble->cols_get();
		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly->rank_max_get();
		}
	}
	else
	{
		return 1;
	}

	(*_pPolyOut) = new MatrixPoly(_pPoly->var_get(), iRowResult, iColResult, piRank);
	delete[] piRank;
	if(bComplex1 || bComplex2)
	{
		(*_pPolyOut)->complex_set(true);
	}
	//Output variables
	//MatrixPoly *pResult	= NULL;
	bool bComplexOut		= bComplex1 || bComplex2;

	if(bScalar1)
	{//p[1] * A
		double *pDoubleR	= _pDouble->real_get();
		double *pDoubleI	= _pDouble->img_get();

		Poly *pPolyIn			= _pPoly->poly_get(0);
		double* pRealIn		= pPolyIn->coef_get()->real_get();
		double* pImgIn		= pPolyIn->coef_get()->img_get();

		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			Poly *pPolyOut		= (*_pPolyOut)->poly_get(i);
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
	}
	else if(bScalar2)
	{//p * cst
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

		//for each line of _pPoly
		for(int iRow1 = 0 ; iRow1 < _pPoly->rows_get() ; iRow1++)
		{//for each col of _pDouble
			for(int iCol2 = 0 ; iCol2 < _pDouble->cols_get() ; iCol2++)
			{//for each rows of _pDouble / cols of _pPoly
				for(int iRow2 = 0 ; iRow2 < _pDouble->rows_get() ; iRow2++)
				{
					Double *pPolyCoef = _pPoly->poly_get(iRow1, iRow2)->coef_get();

					Double *pDouble = NULL;
					if(bComplex2 == false)
					{
						pDouble	= new Double(_pDouble->real_get(iRow2, iCol2));
					}
					else
					{
						pDouble	= new Double(_pDouble->real_get(iRow2, iCol2), _pDouble->img_get(iRow2, iCol2));
					}

					Double *TimeDouble = NULL; //(pPolyCoef->rows_get(), pPolyCoef->cols_get(), bComplexOut);
					MultiplyDoubleByDouble(pPolyCoef, pDouble, &TimeDouble);
					Double *pAddDouble = NULL;
					AddDoubleToDouble(TimeDouble, (*_pPolyOut)->poly_get(iRow1, iCol2)->coef_get(), &pAddDouble);
					(*_pPolyOut)->poly_set(iRow1, iCol2, pAddDouble);

					delete pAddDouble;
					delete pDouble;
				}//for(int iRow2 = 0 ; iRow2 < _pDouble->rows_get() ; iRow2++)
			}//for(int iCol2 = 0 ; iCol2 < _pDouble->cols_get() ; iCol2++)
		}//for(int iRow1 = 0 ; iRow1 < _pPoly->rows_get() ; iRow1++)
	}
	else //Wrong dimensions.
	{
		return 1;
	}
	return 0;
}

int MultiplyPolyByPoly(MatrixPoly* _pPoly1, MatrixPoly* _pPoly2, MatrixPoly** _pPolyOut)
{
	bool bComplex1 	= _pPoly1->isComplex();
	bool bComplex2 	= _pPoly2->isComplex();
	bool bScalar1		= _pPoly1->rows_get() == 1 && _pPoly1->cols_get() == 1;
	bool bScalar2		= _pPoly2->rows_get() == 1 && _pPoly2->cols_get() == 1;

	int iRowResult 	= 0;
	int iColResult	= 0;
	int *piRank			= NULL;

	if(_pPoly1->size_get() == 1 && _pPoly2->size_get() == 1)
	{
		iRowResult = 1;
		iColResult = 1;

		piRank = new int[1];
		piRank[0] = _pPoly1->poly_get(0)->rank_get() + _pPoly2->poly_get(0)->rank_get() - 1;
	}
	else if(_pPoly1->size_get() == 1)
	{
		iRowResult = _pPoly2->rows_get();
		iColResult = _pPoly2->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly1->poly_get(0)->rank_get() + _pPoly2->poly_get(i)->rank_get() - 1;
		}
	}
	else if (_pPoly2->size_get() == 1)
	{
		iRowResult = _pPoly1->rows_get();
		iColResult = _pPoly1->cols_get();

		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly2->poly_get(0)->rank_get() * _pPoly1->poly_get(i)->rank_get();
		}
	}
	else if(_pPoly1->cols_get() == _pPoly2->rows_get())
	{
		iRowResult = _pPoly1->rows_get();
		iColResult = _pPoly2->cols_get();
		piRank = new int[iRowResult * iColResult];
		for(int i = 0 ; i < iRowResult * iColResult ; i++)
		{
			piRank[i] = _pPoly1->rank_max_get() * _pPoly2->rank_max_get();
		}
	}
	else
	{
		return 1;
	}

	(*_pPolyOut) = new MatrixPoly(_pPoly1->var_get(), iRowResult, iColResult, piRank);

	delete[] piRank;

	if(bComplex1 || bComplex2)
	{
		(*_pPolyOut)->complex_set(true);
	}

	if(bScalar1 && bScalar2)
	{//poly1(0) * poly2(0)
		if(bComplex1 == false && bComplex2 == false)
		{
			Poly *pPoly1		= _pPoly1->poly_get(0);
			Poly *pPoly2		= _pPoly2->poly_get(0);
			Poly *pPolyOut	= (*_pPolyOut)->poly_get(0);

			pPolyOut->coef_get()->zero_set();

			iMultiRealPolyByRealPoly(
					pPoly1->coef_get()->real_get(), pPoly1->rank_get(),
					pPoly2->coef_get()->real_get(), pPoly2->rank_get(),
					pPolyOut->coef_get()->real_get(), pPolyOut->rank_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			Poly *pPoly1		= _pPoly1->poly_get(0);
			Poly *pPoly2		= _pPoly2->poly_get(0);
			Poly *pPolyOut	= (*_pPolyOut)->poly_get(0);

			pPolyOut->coef_get()->zero_set();

			iMultiRealPolyByComplexPoly(
					pPoly1->coef_get()->real_get(), pPoly1->rank_get(),
					pPoly2->coef_get()->real_get(), pPoly2->coef_get()->img_get(), pPoly2->rank_get(),
					pPolyOut->coef_get()->real_get(), pPolyOut->coef_get()->img_get(), pPolyOut->rank_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			Poly *pPoly1		= _pPoly1->poly_get(0);
			Poly *pPoly2		= _pPoly2->poly_get(0);
			Poly *pPolyOut	= (*_pPolyOut)->poly_get(0);

			pPolyOut->coef_get()->zero_set();

			iMultiComplexPolyByRealPoly(
					pPoly1->coef_get()->real_get(), pPoly1->coef_get()->img_get(), pPoly1->rank_get(),
					pPoly2->coef_get()->real_get(), pPoly2->rank_get(),
					pPolyOut->coef_get()->real_get(), pPolyOut->coef_get()->img_get(), pPolyOut->rank_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			Poly *pPoly1			= _pPoly1->poly_get(0);
			Poly *pPoly2			= _pPoly2->poly_get(0);
			Poly *pPolyOut		= (*_pPolyOut)->poly_get(0);
			Double *pCoef1		= pPoly1->coef_get();
			Double *pCoef2		= pPoly2->coef_get();
			Double *pCoefOut	= pPolyOut->coef_get();

			pCoefOut->zero_set();
			iMultiComplexPolyByComplexPoly(
					pCoef1->real_get(), pCoef1->img_get(), pPoly1->rank_get(),
					pCoef2->real_get(), pCoef2->img_get(), pPoly2->rank_get(),
					pCoefOut->real_get(), pCoefOut->img_get(), pPolyOut->rank_get());
		}
	}
	else if(bScalar1)
	{//poly1(0) * poly2(n)
		Poly *pPoly1		= _pPoly1->poly_get(0);
		if(bComplex1 == false && bComplex2 == false)
		{
			for(int iPoly = 0 ; iPoly < _pPoly2->size_get() ; iPoly++)
			{
				Poly *pPoly2		= _pPoly2->poly_get(iPoly);
				Poly *pPolyOut	= (*_pPolyOut)->poly_get(iPoly);

				pPolyOut->coef_get()->zero_set();

				iMultiRealPolyByRealPoly(
						pPoly1->coef_get()->real_get(), pPoly1->rank_get(),
						pPoly2->coef_get()->real_get(), pPoly2->rank_get(),
						pPolyOut->coef_get()->real_get(), pPolyOut->rank_get());
			}
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			for(int iPoly = 0 ; iPoly < _pPoly2->size_get() ; iPoly++)
			{
				Poly *pPoly2		= _pPoly2->poly_get(iPoly);
				Poly *pPolyOut	= (*_pPolyOut)->poly_get(iPoly);

				pPolyOut->coef_get()->zero_set();

				iMultiRealPolyByComplexPoly(
						pPoly1->coef_get()->real_get(), pPoly1->rank_get(),
						pPoly2->coef_get()->real_get(), pPoly2->coef_get()->img_get(), pPoly2->rank_get(),
						pPolyOut->coef_get()->real_get(), pPolyOut->coef_get()->img_get(), pPolyOut->rank_get());
			}
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			for(int iPoly = 0 ; iPoly < _pPoly2->size_get() ; iPoly++)
			{
				Poly *pPoly2		= _pPoly2->poly_get(iPoly);
				Poly *pPolyOut	= (*_pPolyOut)->poly_get(iPoly);

				pPolyOut->coef_get()->zero_set();

				iMultiComplexPolyByRealPoly(
						pPoly1->coef_get()->real_get(), pPoly1->coef_get()->img_get(), pPoly1->rank_get(),
						pPoly2->coef_get()->real_get(), pPoly2->rank_get(),
						pPolyOut->coef_get()->real_get(), pPolyOut->coef_get()->img_get(), pPolyOut->rank_get());
			}
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			Double *pCoef1		= pPoly1->coef_get();
			for(int iPoly = 0 ; iPoly < _pPoly2->size_get() ; iPoly++)
			{
				Poly *pPoly2			= _pPoly2->poly_get(iPoly);
				Poly *pPolyOut		= (*_pPolyOut)->poly_get(iPoly);
				Double *pCoef2		= pPoly2->coef_get();
				Double *pCoefOut	= pPolyOut->coef_get();


				pCoefOut->zero_set();

				iMultiComplexPolyByComplexPoly(
						pCoef1->real_get(), pCoef1->img_get(), pPoly1->rank_get(),
						pCoef2->real_get(), pCoef2->img_get(), pPoly2->rank_get(),
						pCoefOut->real_get(), pCoefOut->img_get(), pPolyOut->rank_get());
			}
		}
	}
	else if(bScalar2)
	{//poly1(n) * poly2(0)
		Poly *pPoly2		= _pPoly2->poly_get(0);
		if(bComplex1 == false && bComplex2 == false)
		{
			for(int iPoly = 0 ; iPoly < _pPoly1->size_get() ; iPoly++)
			{
				Poly *pPoly1		= _pPoly1->poly_get(iPoly);
				Poly *pPolyOut	= (*_pPolyOut)->poly_get(iPoly);

				pPolyOut->coef_get()->zero_set();

				iMultiRealPolyByRealPoly(
						pPoly1->coef_get()->real_get(), pPoly1->rank_get(),
						pPoly2->coef_get()->real_get(), pPoly2->rank_get(),
						pPolyOut->coef_get()->real_get(), pPolyOut->rank_get());
			}
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			for(int iPoly = 0 ; iPoly < _pPoly1->size_get() ; iPoly++)
			{
				Poly *pPoly1		= _pPoly1->poly_get(iPoly);
				Poly *pPolyOut	= (*_pPolyOut)->poly_get(iPoly);

				pPolyOut->coef_get()->zero_set();

				iMultiRealPolyByComplexPoly(
						pPoly1->coef_get()->real_get(), pPoly1->rank_get(),
						pPoly2->coef_get()->real_get(), pPoly2->coef_get()->img_get(), pPoly2->rank_get(),
						pPolyOut->coef_get()->real_get(), pPolyOut->coef_get()->img_get(), pPolyOut->rank_get());
			}
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			for(int iPoly = 0 ; iPoly < _pPoly1->size_get() ; iPoly++)
			{
				Poly *pPoly1		= _pPoly1->poly_get(iPoly);
				Poly *pPolyOut	= (*_pPolyOut)->poly_get(iPoly);

				pPolyOut->coef_get()->zero_set();

				iMultiComplexPolyByRealPoly(
						pPoly1->coef_get()->real_get(), pPoly1->coef_get()->img_get(), pPoly1->rank_get(),
						pPoly2->coef_get()->real_get(), pPoly2->rank_get(),
						pPolyOut->coef_get()->real_get(), pPolyOut->coef_get()->img_get(), pPolyOut->rank_get());
			}
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			Double *pCoef2		= pPoly2->coef_get();
			for(int iPoly = 0 ; iPoly < _pPoly1->size_get() ; iPoly++)
			{
				Poly *pPoly1			= _pPoly1->poly_get(iPoly);
				Poly *pPolyOut		= (*_pPolyOut)->poly_get(iPoly);
				Double *pCoef1		= pPoly1->coef_get();
				Double *pCoefOut	= pPolyOut->coef_get();


				pCoefOut->zero_set();

				iMultiComplexPolyByComplexPoly(
						pCoef1->real_get(), pCoef1->img_get(), pPoly1->rank_get(),
						pCoef2->real_get(), pCoef2->img_get(), pPoly2->rank_get(),
						pCoefOut->real_get(), pCoefOut->img_get(), pPolyOut->rank_get());
			}
		}
	}
	else
	{// matrix by matrix
		if(bComplex1 == false && bComplex2 == false)
		{
			double *pReal	= NULL;
			Poly *pTemp		= new Poly(&pReal, (*_pPolyOut)->rank_max_get());

			for(int iRow = 0 ; iRow < _pPoly1->rows_get() ; iRow++)
			{
				for(int iCol = 0 ; iCol < _pPoly2->cols_get() ; iCol++)
				{
					Poly *pResult = (*_pPolyOut)->poly_get(iRow, iCol);
					pResult->coef_get()->zero_set();

					for(int iCommon = 0 ; iCommon < _pPoly1->cols_get() ; iCommon++)
					{
						Poly *pL			= _pPoly1->poly_get(iRow, iCommon);
						Poly *pR			= _pPoly2->poly_get(iCommon, iCol);

						pTemp->coef_get()->zero_set();

						iMultiRealPolyByRealPoly(
								pL->coef_get()->real_get(), pL->rank_get(),
								pR->coef_get()->real_get(), pR->rank_get(),
								pTemp->coef_get()->real_get(), pL->rank_get() + pR->rank_get() - 1);
				
						iAddRealPolyToRealPoly(
								pResult->coef_get()->real_get(), pResult->rank_get(),
								pTemp->coef_get()->real_get(), pResult->rank_get(),
								pResult->coef_get()->real_get(), pResult->rank_get());
					}
				}
			}
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			double *pReal	= NULL;
			double *pImg	= NULL;
			Poly *pTemp		= new Poly(&pReal, &pImg, (*_pPolyOut)->rank_max_get());

			for(int iRow = 0 ; iRow < _pPoly1->rows_get() ; iRow++)
			{
				for(int iCol = 0 ; iCol < _pPoly2->cols_get() ; iCol++)
				{
					Poly *pResult = (*_pPolyOut)->poly_get(iRow, iCol);
					pResult->coef_get()->zero_set();

					for(int iCommon = 0 ; iCommon < _pPoly1->cols_get() ; iCommon++)
					{
						Poly *pL			= _pPoly1->poly_get(iRow, iCommon);
						Poly *pR			= _pPoly2->poly_get(iCommon, iCol);

						pTemp->coef_get()->zero_set();

						iMultiRealPolyByComplexPoly(
								pL->coef_get()->real_get(), pL->rank_get(),
								pR->coef_get()->real_get(), pR->coef_get()->img_get(), pR->rank_get(),
								pTemp->coef_get()->real_get(), pTemp->coef_get()->img_get(), pL->rank_get() + pR->rank_get() - 1);
				
						iAddComplexPolyToComplexPoly(
								pResult->coef_get()->real_get(), pResult->coef_get()->img_get(), pResult->rank_get(),
								pTemp->coef_get()->real_get(), pTemp->coef_get()->img_get(), pResult->rank_get(),
								pResult->coef_get()->real_get(), pResult->coef_get()->img_get(), pResult->rank_get());
					}
				}
			}
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			double *pReal	= NULL;
			double *pImg	= NULL;
			Poly *pTemp		= new Poly(&pReal, &pImg, (*_pPolyOut)->rank_max_get());

			for(int iRow = 0 ; iRow < _pPoly1->rows_get() ; iRow++)
			{
				for(int iCol = 0 ; iCol < _pPoly2->cols_get() ; iCol++)
				{
					Poly *pResult = (*_pPolyOut)->poly_get(iRow, iCol);
					pResult->coef_get()->zero_set();

					for(int iCommon = 0 ; iCommon < _pPoly1->cols_get() ; iCommon++)
					{
						Poly *pL			= _pPoly1->poly_get(iRow, iCommon);
						Poly *pR			= _pPoly2->poly_get(iCommon, iCol);

						pTemp->coef_get()->zero_set();

						iMultiRealPolyByComplexPoly(
								pR->coef_get()->real_get(), pR->rank_get(),
								pL->coef_get()->real_get(), pL->coef_get()->img_get(), pL->rank_get(),
								pTemp->coef_get()->real_get(), pTemp->coef_get()->img_get(), pL->rank_get() + pR->rank_get() - 1);
				
						iAddComplexPolyToComplexPoly(
								pResult->coef_get()->real_get(), pResult->coef_get()->img_get(), pResult->rank_get(),
								pTemp->coef_get()->real_get(), pTemp->coef_get()->img_get(), pResult->rank_get(),
								pResult->coef_get()->real_get(), pResult->coef_get()->img_get(), pResult->rank_get());
					}
				}
			}
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			double *pReal	= NULL;
			double *pImg	= NULL;
			Poly *pTemp		= new Poly(&pReal, &pImg, (*_pPolyOut)->rank_max_get());

			for(int iRow = 0 ; iRow < _pPoly1->rows_get() ; iRow++)
			{
				for(int iCol = 0 ; iCol < _pPoly2->cols_get() ; iCol++)
				{
					Poly *pResult = (*_pPolyOut)->poly_get(iRow, iCol);
					pResult->coef_get()->zero_set();

					for(int iCommon = 0 ; iCommon < _pPoly1->cols_get() ; iCommon++)
					{
						Poly *pL			= _pPoly1->poly_get(iRow, iCommon);
						Poly *pR			= _pPoly2->poly_get(iCommon, iCol);

						pTemp->coef_get()->zero_set();

						iMultiComplexPolyByComplexPoly(
								pL->coef_get()->real_get(), pL->coef_get()->img_get(), pL->rank_get(),
								pR->coef_get()->real_get(), pR->coef_get()->img_get(), pR->rank_get(),
								pTemp->coef_get()->real_get(), pTemp->coef_get()->img_get(), pL->rank_get() + pR->rank_get() - 1);
				
						iAddComplexPolyToComplexPoly(
								pResult->coef_get()->real_get(), pResult->coef_get()->img_get(), pResult->rank_get(),
								pTemp->coef_get()->real_get(), pTemp->coef_get()->img_get(), pResult->rank_get(),
								pResult->coef_get()->real_get(), pResult->coef_get()->img_get(), pResult->rank_get());
					}
				}
			}
		}
	}
	return 0; //No Error;
}
