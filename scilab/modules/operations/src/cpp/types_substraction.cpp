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

Double* SubstractDoubleToDouble(Double* _pDouble1, Double* _pDouble2)
{
	Double *pResult = NULL;
	if(_pDouble1->size_get() == 1)
	{//add pL with each element of pR
		double *pReal			= NULL;
		double *pImg			= NULL;

		double *pdblRealR	= _pDouble2->real_get();
		double *pdblImgR	= _pDouble2->img_get();
		double pdblRealL	= _pDouble1->real_get() == NULL ? 0 : _pDouble1->real_get()[0];
		double pdblImgL		= _pDouble1->img_get() == NULL ? 0 : _pDouble1->img_get()[0];

		pResult						= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);

		for(int i = 0 ; i < _pDouble2->size_get() ; i++)
		{
			pReal[i]	= pdblRealL - (pdblRealR == NULL ? 0 : pdblRealR[i]);
		}

		if(_pDouble1->isComplex() || _pDouble2->isComplex())
		{
			pResult->complex_set(true);
			pImg				= pResult->img_get();
			for(int i = 0 ; i < _pDouble2->size_get() ; i++)
			{
				pImg[i]		= pdblImgL	- (pdblImgR == NULL ? 0 : pdblImgR[i]);
			}
		}
	}
	else if(_pDouble2->size_get() == 1)
	{//add pL with each element of pR
		double *pReal			= NULL;
		double *pImg			= NULL;

		double *pdblRealL	= _pDouble1->real_get();
		double *pdblImgL	= _pDouble1->img_get();
		double pdblRealR	= _pDouble2->real_get() == NULL ? 0 : _pDouble2->real_get()[0];
		double pdblImgR		= _pDouble2->img_get() == NULL ? 0 : _pDouble2->img_get()[0];

		pResult						= new Double(_pDouble1->rows_get(), _pDouble1->cols_get(), &pReal);

		for(int i = 0 ; i < _pDouble1->size_get() ; i++)
		{
			pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i])	- pdblRealR;
		}

		if(_pDouble1->isComplex() || _pDouble2->isComplex())
		{
			pResult->complex_set(true);
			pImg = pResult->img_get();
			for(int i = 0 ; i < _pDouble1->size_get() ; i++)
			{
				pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])		- pdblImgR;
			}
		}
	}
	else if(_pDouble1->rows_get() == _pDouble2->rows_get() && _pDouble1->cols_get() == _pDouble2->cols_get())//same dimension
	{//add pL and pR element wise
		double *pReal			= NULL;
		double *pImg			= NULL;

		pResult						= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal, &pImg);
		double *pdblRealR	= _pDouble2->real_get();
		double *pdblImgR	= _pDouble2->img_get();
		double *pdblRealL	= _pDouble1->real_get();
		double *pdblImgL	= _pDouble1->img_get();

		for(int i = 0 ; i < _pDouble2->size_get() ; i++)
		{
			pReal[i]	= (pdblRealL == NULL ? 0 : pdblRealL[i])	- (pdblRealR == NULL ? 0 : pdblRealR[i]);
		}

		if(_pDouble1->isComplex() || _pDouble2->isComplex())
		{
			pResult->complex_set(true);
			pImg = pResult->img_get();

			for(int i = 0 ; i < _pDouble2->size_get() ; i++)
			{
				pImg[i]		= (pdblImgL == NULL ? 0 : pdblImgL[i])		- (pdblImgR == NULL ? 0 : pdblImgR[i]);
			}

		}
	}
	return pResult;
}

MatrixPoly* SubstractPolyToDouble(Double *_pDouble, MatrixPoly *_pPoly)
{
	MatrixPoly* pResult = NULL;
	double *pInDblR			= _pDouble->real_get();
	double *pInDblI			= _pDouble->img_get();

	if(_pDouble->size_get() == 1)
	{
		pResult = new MatrixPoly();
		//Create new Poly
		*pResult = *_pPoly;

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{
			pResult->complex_set(true);
		}
		
		for(int i = 0 ; i < pResult->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= pResult->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0]			= pInDblR[0] - pInPolyR[0];

			for(int j = 1 ; j < pInPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = -pOutPolyR[j];
			}
		}

		if(pResult->isComplex())
		{
			pResult->complex_set(true);
			for(int i = 0 ; i < pResult->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= pResult->poly_get(i);
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
		pResult = new MatrixPoly();
		//Create new Poly
		*pResult = *_pPoly;

		for(int i = 0 ; i < pResult->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= pResult->poly_get(i);

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
			pResult->complex_set(true);
			for(int i = 0 ; i < pResult->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= pResult->poly_get(i);

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

		pResult = new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pDouble->cols_get(), piRank);

		for(int i = 0 ; i < pResult->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(0);
			Poly *pOutPoly		= pResult->poly_get(i);

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
			pResult->complex_set(true);
			for(int i = 0 ; i < pResult->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(0);
				Poly *pOutPoly		= pResult->poly_get(i);

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

	return pResult;	
}

MatrixPoly* SubstractDoubleToPoly(MatrixPoly *_pPoly,Double *_pDouble)
{
	MatrixPoly *pResult = NULL;
	double *pInDblR			= _pDouble->real_get();
	double *pInDblI			= _pDouble->img_get();

	if(_pDouble->size_get() == 1)
	{
		pResult = new MatrixPoly();
		//Create new Poly
		*pResult = *_pPoly;
		for(int i = 0 ; i < pResult->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= pResult->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0]			= pInPolyR[0] - pInDblR[0];
		}

		if(_pPoly->isComplex() || _pDouble->isComplex())
		{ 
			pResult->complex_set(true);
			for(int i = 0 ; i < pResult->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= pResult->poly_get(i);
				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();

				pOutPolyI[0]		=  (pInPolyI == NULL ? 0 : pInPolyI[0]) - (pInDblI == NULL ? 0 : pInDblI[0]);
			}
		}
	}
	else if (_pDouble->rows_get() == _pPoly->rows_get() && _pDouble->cols_get() == _pPoly->cols_get())
	{
		pResult = new MatrixPoly();
		//Create new Poly
		*pResult = *_pPoly;
		for(int i = 0 ; i < pResult->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(i);
			Poly *pOutPoly		= pResult->poly_get(i);

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
			pResult->complex_set(true);
			for(int i = 0 ; i < pResult->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(i);
				Poly *pOutPoly		= pResult->poly_get(i);

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

		pResult = new MatrixPoly(_pPoly->var_get(), _pDouble->rows_get(), _pDouble->cols_get(), piRank);

		for(int i = 0 ; i < pResult->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(0);
			Poly *pOutPoly		= pResult->poly_get(i);

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
			pResult->complex_set(true);
			for(int i = 0 ; i < pResult->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(0);
				Poly *pOutPoly		= pResult->poly_get(i);

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
	return pResult;
}

MatrixPoly* SubstractPolyToPoly(MatrixPoly *_pPoly1, MatrixPoly *_pPoly2)
{
	MatrixPoly *pResult = NULL;
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

		pResult = new MatrixPoly(_pPoly2->var_get(), _pPoly1->rows_get(), _pPoly1->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			pResult->complex_set(true);
		}

		//Result P1(i) + P2(i)
		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			Double *pCoef1	= _pPoly1->poly_get(i)->coef_get();
			double *p1R			= pCoef1->real_get();

			Double *pCoef2	= _pPoly2->poly_get(i)->coef_get();
			double *p2R			= pCoef2->real_get();

			Double *pCoefR	= pResult->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			if(pResult->isComplex())
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

		delete pRank;
		delete pRank1;
		delete pRank2;
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

		pResult = new MatrixPoly(_pPoly2->var_get(), _pPoly2->rows_get(), _pPoly2->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			pResult->complex_set(true);
		}

		//Result P1(0) + P2(i)
		Double *pCoef1	= _pPoly1->poly_get(0)->coef_get();
		double *p1R			= pCoef1->real_get();

		for(int i = 0 ; i < _pPoly2->size_get() ; i++)
		{
			Double *pCoef2	= _pPoly2->poly_get(i)->coef_get();
			double *p2R			= pCoef2->real_get();

			Double *pCoefR	= pResult->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			if(pResult->isComplex())
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

		delete pRank;
		delete pRank1;
		delete pRank2;
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

		pResult = new MatrixPoly(_pPoly1->var_get(), _pPoly1->rows_get(), _pPoly1->cols_get(), pRank);
		if(_pPoly1->isComplex() || _pPoly2->isComplex())
		{
			pResult->complex_set(true);
		}

		//Result P1(i) + P2(0)
		Double *pCoef2	= _pPoly2->poly_get(0)->coef_get();
		double *p2R			= pCoef2->real_get();

		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			Double *pCoef1	= _pPoly1->poly_get(i)->coef_get();
			double *p1R			= pCoef1->real_get();

			Double *pCoefR	= pResult->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] - p2R[j];
			}

			if(pResult->isComplex())
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

		delete pRank;
		delete pRank1;
		delete pRank2;
	}

	if(pResult != NULL)
	{
		pResult->update_rank();
	}

	//if pResult == NULL -> incompatible dimensions
	return pResult;
}

