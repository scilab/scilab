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

#include "types_substraction.hxx"
#include "core_math.h"

extern "C"
{
	#include "matrix_substraction.h"
}

int SubstractDoubleToDouble(Double* _pDouble1, Double* _pDouble2, Double** _pDoubleOut)
{
	bool bComplex1		= _pDouble1->isComplex();
	bool bComplex2		= _pDouble2->isComplex();
	bool bScalar1			= _pDouble1->rows_get() == 1 && _pDouble1->cols_get() == 1;
	bool bScalar2			= _pDouble2->rows_get() == 1 && _pDouble2->cols_get() == 1;

	double *pReal			= NULL;
	double *pImg			= NULL;

	if(bScalar1)
	{//add pL with each element of pR
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut) = new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);
			iSubstractRealMatrixToRealScalar(
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					_pDouble1->real_get()[0],
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut) = new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iSubstractComplexMatrixToRealScalar(
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					_pDouble1->real_get()[0],
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut) = new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iSubstractRealMatrixToComplexScalar(
					_pDouble2->real_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					_pDouble1->real_get()[0], _pDouble1->img_get()[0],
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut) = new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
			iSubstractComplexMatrixToComplexScalar(
					_pDouble2->real_get(), _pDouble2->img_get(), _pDouble2->rows_get(), _pDouble2->cols_get(),
					_pDouble1->real_get()[0], _pDouble1->img_get()[0],
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else if(bScalar2)
	{//add pL with each element of pR
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal);
			iSubstractRealScalarToRealMatrix(
					_pDouble2->real_get()[0],
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iSubstractComplexScalarToRealMatrix(
					_pDouble2->real_get()[0], _pDouble2->img_get()[0],
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iSubstractRealScalarToComplexMatrix(
					_pDouble2->real_get()[0],
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iSubstractComplexScalarToComplexMatrix(
					_pDouble2->real_get()[0], _pDouble2->img_get()[0],
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else if(_pDouble1->rows_get() == _pDouble2->rows_get() && _pDouble1->cols_get() == _pDouble2->cols_get())//same dimension
	{//add pL and pR element wise
		if(bComplex1 == false && bComplex2 == false)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal);
			iSubstractRealMatrixToRealMatrix(
					_pDouble2->real_get(),
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get());
		}
		else if(bComplex1 == false && bComplex2 == true)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iSubstractComplexMatrixToRealMatrix(
					_pDouble2->real_get(), _pDouble2->img_get(),
					_pDouble1->real_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == false)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iSubstractRealMatrixToComplexMatrix(
					_pDouble2->real_get(),
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
		else if(bComplex1 == true && bComplex2 == true)
		{
			(*_pDoubleOut) = new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal, &pImg);
			iSubstractComplexMatrixToComplexMatrix(
					_pDouble2->real_get(), _pDouble2->img_get(),
					_pDouble1->real_get(), _pDouble1->img_get(), _pDouble1->rows_get(), _pDouble1->cols_get(),
					(*_pDoubleOut)->real_get(), (*_pDoubleOut)->img_get());
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

int SubstractPolyToDouble(Double *_pDouble, MatrixPoly *_pPoly, MatrixPoly** _pPolyOut)
{
	double *pInDblR			= _pDouble->real_get();
	double *pInDblI			= _pDouble->img_get();

	if(_pDouble->size_get() == 1)
	{
		(*_pPolyOut) = new MatrixPoly();
		//Create new Poly
		**_pPolyOut = *_pPoly;

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}
		
		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0]			= pInDblR[0] - pInPolyR[0];

			for(int j = 1 ; j < pInPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = -pOutPolyR[j];
			}
		}

		if((*_pPolyOut)->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0]		= (pInDblI == NULL ? 0 : pInDblI[0]) - (pInPolyI == NULL ? 0 : pInPolyI[0]);

				for(int j = 1 ; j < pInPoly->rank_get() ; j++)
				{
					pOutPolyI[j] = -pOutPolyI[j];
				}
			}
		}
	}
	else if (_pDouble->rows_get() == _pPoly->rows_get() && _pDouble->cols_get() == _pPoly->cols_get())
	{
		(*_pPolyOut) = new MatrixPoly();
		//Create new Poly
		**_pPolyOut = *_pPoly;

		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0] = pInDblR[i] - pInPolyR[0];

			for(int j = 1 ; j < pInPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = -pOutPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) - (pOutPolyI != NULL ? pOutPolyI[0] : 0);

				for(int j = 1 ; j < pInPoly->rank_get() ; j++)
				{
					pOutPolyI[j] = -pOutPolyI[j];
				}
			}
		}
	}
	else if(_pPoly->size_get() == 1)
	{//cas balaise
		int *piRank = new int[_pDouble->size_get()];
		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			piRank[i] = _pPoly->poly_get(0)->rank_get();
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pDouble->cols_get(), piRank);

		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(0);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0] = pInDblR[i] - pInPolyR[0];

			for(int j = 1 ; j < pOutPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = -pInPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(0);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) - (pInPolyI != NULL ? pInPolyI[0] : 0);

				for(int j = 1 ; j < pOutPoly->rank_get() ; j++)
				{
					pOutPolyI[j] = -pInPolyI[j];
				}
			}
		}
	}
	else
	{
		return 1;
	}

	return 0;	
}

int SubstractDoubleToPoly(MatrixPoly *_pPoly,Double *_pDouble, MatrixPoly **_pPolyOut)
{
	double *pInDblR			= _pDouble->real_get();
	double *pInDblI			= _pDouble->img_get();

	if(_pDouble->size_get() == 1)
	{
		(*_pPolyOut) = new MatrixPoly();
		//Create new Poly
		**_pPolyOut = *_pPoly;
		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0]			= pInPolyR[0] - pInDblR[0];
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{ 
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);
				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0]		=  (pInPolyI == NULL ? 0 : pInPolyI[0]) - (pInDblI == NULL ? 0 : pInDblI[0]);
			}
		}
	}
	else if (_pDouble->rows_get() == _pPoly->rows_get() && _pDouble->cols_get() == _pPoly->cols_get())
	{
		(*_pPolyOut) = new MatrixPoly();
		//Create new Poly
		**_pPolyOut = *_pPoly;
		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pInPolyI	= pInPoly->coef_get()->img_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();
			double *pOutPolyI	= pOutPoly->coef_get()->img_get();

			pOutPolyR[0] = pInPolyR[0] - pInDblR[i];

			for(int j = 1 ; j < pInPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = pOutPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0] = (pInPolyI != NULL ? pInPolyI[0] : 0) - (pInDblI != NULL ? pInDblI[i] : 0);

				for(int j = 1 ; j < pInPoly->rank_get() ; j++)
				{
					pOutPolyI[j] = pOutPolyI[j];
				}
			}
		}
	}
	else if(_pPoly->size_get() == 1)
	{
		int *piRank = new int[_pDouble->size_get()];
		for(int i = 0 ; i < _pDouble->size_get() ; i++)
		{
			piRank[i] = _pPoly->poly_get(0)->rank_get();
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pDouble->cols_get(), piRank);

		for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(0);
			Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pInPolyI	= pInPoly->coef_get()->img_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();
			double *pOutPolyI	= pOutPoly->coef_get()->img_get();

			pOutPolyR[0] = pInPolyR[0] - pInDblR[i];

			for(int j = 1 ; j < pOutPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = pInPolyR[j];
			}
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
			for(int i = 0 ; i < (*_pPolyOut)->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(0);
				Poly *pOutPoly		= (*_pPolyOut)->poly_get(i);

				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0] = (pInPolyI != NULL ? pInPolyI[0] : 0) - (pInDblI != NULL ? pInDblI[i] : 0);

				for(int j = 1 ; j < pOutPoly->rank_get() ; j++)
				{
					pOutPolyI[j] = pInPolyI[j];
				}
			}
		}
	}
	else
	{
		return 1;
	}
	return 0;
}

int SubstractPolyToPoly(MatrixPoly *_pPoly1, MatrixPoly *_pPoly2, MatrixPoly **_pPolyOut)
{
	//3 cases : 
	//size(p1) == size(P2)
	if(_pPoly1->rows_get() == _pPoly2->rows_get() && _pPoly1->cols_get() == _pPoly2->cols_get())
	{
		int *pRank	= new int[_pPoly1->size_get()];
		int *pRank1	= new int[_pPoly1->size_get()];
		int *pRank2	= new int[_pPoly2->size_get()];

		_pPoly1->rank_get(pRank1);
		_pPoly2->rank_get(pRank2);
		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			pRank[i] = Max(pRank1[i], pRank2[i]);
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly2->var_get(), _pPoly1->rows_get(), _pPoly1->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}

		//Result P1(i) + P2(i)
		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			Double *pCoef1	= _pPoly1->poly_get(i)->coef_get();
			double *p1R			= pCoef1->real_get();

			Double *pCoef2	= _pPoly2->poly_get(i)->coef_get();
			double *p2R			= pCoef2->real_get();

			Double *pCoefR	= (*_pPolyOut)->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();

			for(int j = 0 ; j < Min(pRank1[i], pRank2[i]) ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			double *pTemp = NULL;
			int iCoef			= 1;
			if(pRank1[i] > pRank2[i])
			{
				pTemp = p1R;
				iCoef = 1;
			}
			else
			{
				pTemp = p2R;
				iCoef = -1;
			}
			for(int j = Min(pRank1[i], pRank2[i]) ; j < Max(pRank1[i], pRank2[i]) ; j++)
			{
				pRR[j] = pTemp[j] * iCoef;
			}

			if((*_pPolyOut)->isComplex())
			{
				double *p1I			= pCoef1->img_get();
				double *p2I			= pCoef2->img_get();
				double *pRI			= pCoefR->img_get();

				for(int j = 0 ; j < Min(pRank1[i], pRank2[i]) ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
				}

				double *pTemp = NULL;
				int iCoef			= 1;
				if(pRank1[i] > pRank2[i])
				{
					pTemp = p1I;
					iCoef = 1;
				}
				else
				{
					pTemp = p2I;
					iCoef = -1;
				}
				for(int j = Min(pRank1[i], pRank2[i]) ; j < Max(pRank1[i], pRank2[i]) ; j++)
				{
					pRR[j] = pTemp[j] * iCoef;
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else if(_pPoly1->size_get() == 1)
	{//size(p1) == 1
		int *pRank	= new int[_pPoly2->size_get()];
		int *pRank1	= new int[_pPoly2->size_get()];
		int *pRank2	= new int[_pPoly2->size_get()];
		memset(pRank1, 0x00, _pPoly2->size_get() * sizeof(int));

		_pPoly1->rank_get(pRank1);
		_pPoly2->rank_get(pRank2);
		for(int i = 0 ; i < _pPoly2->size_get() ; i++)
		{
			pRank[i] = Max(pRank1[0], pRank2[i]);
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly2->var_get(), _pPoly2->rows_get(), _pPoly2->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}

		//Result P1(0) + P2(i)
		Double *pCoef1	= _pPoly1->poly_get(0)->coef_get();
		double *p1R			= pCoef1->real_get();

		for(int i = 0 ; i < _pPoly2->size_get() ; i++)
		{
			Double *pCoef2	= _pPoly2->poly_get(i)->coef_get();
			double *p2R			= pCoef2->real_get();

			Double *pCoefR	= (*_pPolyOut)->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				double *p1I			= pCoef1->img_get();
				double *p2I			= pCoef2->img_get();
				double *pRI			= pCoefR->img_get();

				for(int j = 0 ; j < pRank[i] ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else if(_pPoly2->size_get() == 1)
	{//size(p2) == 1
		int *pRank	= new int[_pPoly1->size_get()];
		int *pRank1	= new int[_pPoly1->size_get()];
		int *pRank2	= new int[_pPoly1->size_get()];
		memset(pRank2, 0x00, _pPoly1->size_get() * sizeof(int));

		_pPoly1->rank_get(pRank1);
		_pPoly2->rank_get(pRank2);
		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			pRank[i] = Max(pRank1[i], pRank2[0]);
		}

		(*_pPolyOut) = new MatrixPoly(_pPoly1->var_get(), _pPoly1->rows_get(), _pPoly1->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			(*_pPolyOut)->complex_set(true);
		}

		//Result P1(i) + P2(0)
		Double *pCoef2	= _pPoly2->poly_get(0)->coef_get();
		double *p2R			= pCoef2->real_get();

		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			Double *pCoef1	= _pPoly1->poly_get(i)->coef_get();
			double *p1R			= pCoef1->real_get();

			Double *pCoefR	= (*_pPolyOut)->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			if((*_pPolyOut)->isComplex())
			{
				double *p2I			= pCoef2->img_get();
				double *p1I			= pCoef1->img_get();
				double *pRI			= pCoefR->img_get();
				for(int j = 0 ; j < pRank[i] ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) - (p2I == NULL ? 0 : p2I[j]);
				}
			}
		}

		delete[] pRank;
		delete[] pRank1;
		delete[] pRank2;
	}
	else
	{
		return 1;
	}

	if((*_pPolyOut) != NULL)
	{
		(*_pPolyOut)->update_rank();
	}

	return 0;
}

