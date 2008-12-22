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

#include "types_addition.hxx"
#include "core_math.h"

Double * AddDoubleToDouble(Double *_pDouble1, Double *_pDouble2)
{
	Double *pResult			= NULL;
	if(_pDouble1->size_get() == 1)
	{//add pL with each element of pR
		double *pReal			= NULL;
		double *pImg			= NULL;

		double *pdblRealR	= _pDouble2->real_get();
		double pdblRealL	= _pDouble1->real_get()[0];

		pResult						= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);

		for(int i = 0 ; i < _pDouble2->size_get() ; i++)
		{
			pReal[i]				= pdblRealL	+ pdblRealR[i];
		}


		if(_pDouble1->isComplex() || _pDouble2->isComplex())
		{
			pResult->complex_set(true);

			double *pdblImgR	= _pDouble2->img_get();
			double pdblImgL		= _pDouble1->img_get() == NULL ? 0 : _pDouble1->img_get()[0];
			pImg							= pResult->img_get();

			for(int i = 0 ; i < _pDouble2->size_get() ; i++)
			{
				pImg[i]					= pdblImgL	+ (pdblImgR == NULL ? 0 : pdblImgR[i]);
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
			pReal[i]				= (pdblRealL == NULL ? 0 : pdblRealL[i])	+ pdblRealR;
		}

		if(_pDouble1->isComplex() || _pDouble2->isComplex())
		{
			pResult->complex_set(true);
			pImg						= pResult->img_get();		

			for(int i = 0 ; i < _pDouble1->size_get() ; i++)
			{
				pImg[i]				= (pdblImgL == NULL ? 0 : pdblImgL[i])		+ pdblImgR;
			}
		}
	}
	else if(_pDouble1->rows_get() == _pDouble2->rows_get() && _pDouble1->cols_get() == _pDouble2->cols_get())//same dimension
	{//add pL and pR element wise
		double *pReal			= NULL;
		double *pImg			= NULL;

		pResult					= new Double(_pDouble2->rows_get(), _pDouble2->cols_get(), &pReal);

		double *pdblRealR	= _pDouble2->real_get();
		double *pdblRealL	= _pDouble1->real_get();

		for(int i = 0 ; i < _pDouble2->size_get() ; i++)
		{
			pReal[i]				= pdblRealL[i] + pdblRealR[i];
		}

		if(_pDouble1->isComplex() || _pDouble2->isComplex())
		{
			pResult->complex_set(true);
			pImg							= pResult->img_get();
			double *pdblImgR	= _pDouble2->img_get();
			double *pdblImgL	= _pDouble1->img_get();
			for(int i = 0 ; i < _pDouble2->size_get() ; i++)
			{
				pImg[i]					= (pdblImgL == NULL ? 0 : pdblImgL[i])	 + (pdblImgR == NULL ? 0 : pdblImgR[i]);
			}
		}
	}
	return pResult;
}

MatrixPoly* AddDoubleToPoly(MatrixPoly *_pPoly, Double *_pDouble)
{
	MatrixPoly* pResult = NULL;
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

			pOutPolyR[0]			= pInDblR[0] + pInPolyR[0];
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

				pOutPolyI[0]			= (pInDblI == NULL ? 0 : pInDblI[0]) + (pInPolyI == NULL ? 0 : pInPolyI[0]);
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

			pOutPolyR[0]			= pInDblR[i] + pInPolyR[0];
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

				pOutPolyI[0]			= (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);
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
		if(_pDouble->isComplex() || _pPoly->isComplex())
		{
			pResult->complex_set(true);
		}

		for(int i = 0 ; i < pResult->size_get() ; i++)
		{
			Poly *pInPoly			= _pPoly->poly_get(0);
			Poly *pOutPoly		= pResult->poly_get(i);
			double *pInPolyR	= pInPoly->coef_get()->real_get();
			double *pOutPolyR	= pOutPoly->coef_get()->real_get();

			pOutPolyR[0] = pInDblR[i] + pInPolyR[0];

			for(int j = 1 ; j < pInPoly->rank_get() ; j++)
			{
				pOutPolyR[j] = pInPolyR[j];
			}
		}
		
		if(pResult->isComplex())
		{
			for(int i = 0 ; i < pResult->size_get() ; i++)
			{
				Poly *pInPoly			= _pPoly->poly_get(0);
				Poly *pOutPoly		= pResult->poly_get(i);
				double *pInPolyI	= pInPoly->coef_get()->img_get();
				double *pOutPolyI	= pOutPoly->coef_get()->img_get();
	
				pOutPolyI[0] = (pInDblI != NULL ? pInDblI[i] : 0) + (pInPolyI != NULL ? pInPolyI[0] : 0);

				for(int j = 1 ; j < pInPoly->rank_get() ; j++)
				{
					pOutPolyI[j] = (pInPolyI != NULL ? pInPolyI[j] : 0);
				}
			}
		}
	}
	return pResult;	
}

MatrixPoly* AddPolyToPoly(MatrixPoly* _pPoly1, MatrixPoly* _pPoly2)
{
	MatrixPoly* pResult = NULL;
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
			double *p1I			= pCoef1->img_get();

			Double *pCoef2	= _pPoly2->poly_get(i)->coef_get();
			double *p2R			= pCoef2->real_get();
			double *p2I			= pCoef2->img_get();

			Double *pCoefR	= pResult->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();
			double *pRI			= pCoefR->img_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] + p2R[j];
			}

			if(pResult->isComplex())
			{
				for(int j = 0 ; j < pRank[i] ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
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
		double *p1I			= pCoef1->img_get();
		for(int i = 0 ; i < _pPoly2->size_get() ; i++)
		{
			Double *pCoef2	= _pPoly2->poly_get(i)->coef_get();
			double *p2R			= pCoef2->real_get();
			double *p2I			= pCoef2->img_get();

			Double *pCoefR	= pResult->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();
			double *pRI			= pCoefR->img_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] + p2R[j];
			}

			if(pResult->isComplex())
			{
				for(int j = 0 ; j < pRank[i] ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
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
		double *p2I			= pCoef2->img_get();

		for(int i = 0 ; i < _pPoly1->size_get() ; i++)
		{
			Double *pCoef1	= _pPoly1->poly_get(i)->coef_get();
			double *p1R			= pCoef1->real_get();
			double *p1I			= pCoef1->img_get();

			Double *pCoefR	= pResult->poly_get(i)->coef_get();
			double *pRR			= pCoefR->real_get();
			double *pRI			= pCoefR->img_get();

			for(int j = 0 ; j < pRank[i] ; j++)
			{
				pRR[j] = p1R[j] + p2R[j];
			}

			if(pResult->isComplex())
			{
				for(int j = 0 ; j < pRank[i] ; j++)
				{
					pRI[j] = (p1I == NULL ? 0 : p1I[j]) + (p2I == NULL ? 0 : p2I[j]);
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

String* AddStringToString(String *_pString1, String *_pString2)
{
	String *pResult = NULL;
	if(_pString1->size_get() == 1)
	{//concat pL with each element of pR
		pResult = new String(_pString2->rows_get(), _pString2->cols_get());
		int iCommonLen	=	strlen(_pString1->string_get(0,0));

		for(int i = 0 ; i < _pString2->rows_get() ; i++)
		{
			for(int j = 0 ; j < _pString2->cols_get() ; j++)
			{
				int iLen	= strlen(_pString2->string_get(i, j));
				char* psz = new char[iLen + iCommonLen + 1];
				memset(psz, 0x00, iLen + iCommonLen + 1);
				memcpy(psz, _pString1->string_get(0,0), iCommonLen * sizeof(char));
				memcpy(psz + iCommonLen, _pString2->string_get(i, j), iLen * sizeof(char));
				pResult->string_set(i, j, psz);
				delete psz;
			}
		}
	}
	else if(_pString2->size_get() == 1)
	{//concat each element of pL with pR
		pResult = new String(_pString1->rows_get(), _pString1->cols_get());
		int iCommonLen	=	strlen(_pString2->string_get(0,0));

		for(int i = 0 ; i < _pString1->rows_get() ; i++)
		{
			for(int j = 0 ; j < _pString1->cols_get() ; j++)
			{
				int iLen	= strlen(_pString1->string_get(i, j));
				char* psz = new char[iLen + iCommonLen + 1];
				memset(psz, 0x00, iLen + iCommonLen + 1);

				memcpy(psz, _pString1->string_get(i, j), iLen * sizeof(char));
				memcpy(psz + iLen, _pString2->string_get(0,0), iCommonLen * sizeof(char));

				pResult->string_set(i, j, psz);
				delete psz;
			}
		}
	}
	else if(_pString1->rows_get() == _pString2->rows_get() && _pString1->cols_get() == _pString2->cols_get())
	{//concat pL with pR element wise
		pResult = new String(_pString1->rows_get(), _pString1->cols_get());
		for(int i = 0 ; i < _pString1->rows_get() ; i++)
		{
			for(int j = 0 ; j < _pString1->cols_get() ; j++)
			{
				int iLenL	= strlen(_pString1->string_get(i, j));
				int iLenR	= strlen(_pString2->string_get(i, j));
				char* psz = new char[iLenL + iLenR + 1];
				memset(psz, 0x00, iLenL + iLenR + 1);

				memcpy(psz					, _pString1->string_get(i, j), iLenL * sizeof(char));
				memcpy(psz + iLenL	, _pString2->string_get(i, j), iLenR * sizeof(char));

				pResult->string_set(i, j, psz);
				delete psz;
			}
		}
	}
	return pResult;

}

