/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008-2008 - INRIA - Antoine ELIAS <antoine.elias@scilab.org>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "doublecomplex.h"
#include "matrix_right_division.h"
#include "matrix_left_division.h"

/*iRightDivisionComplexMatrixByComplexMatrix*/
int iRightDivisionComplexMatrixByComplexMatrix(
				double *_pdblReal1,		double *_pdblImg1,		int _iInc1,
				double *_pdblReal2,		double *_pdblImg2,		int _iInc2,
				double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize)
{
	int iErr = 0;
	int iErr2 = 0;
	int iErr3 = 0;
	int iIndex		= 0; //Main loop index
	int iIndex1		= 0; //Loop index on left operand
	int iIndex2		= 0; //Loop index on right operand
	int iIndexOut	= 0; //Lopp index on result matrix

	if(_iInc2 == 0)
	{
		if(dabss(_pdblReal2[iIndex2]) + dabss(_pdblImg2[iIndex2]) == 0)
			iErr2 = 1;
	}

	for(iIndex = 0 ; iIndex < _iSize ; iIndex++)
	{
		iErr3 = iRightDivisionComplexByComplex(_pdblReal1[iIndex1], _pdblImg1[iIndex1], _pdblReal2[iIndex2], _pdblImg2[iIndex2], &_pdblRealOut[iIndexOut], &_pdblImgOut[iIndexOut]);
		if(iErr3 != 0)
			iErr = iIndex + 1;
		iIndexOut	+= _iIncOut;
		iIndex1		+= _iInc1;
		iIndex2		+= _iInc2;
	}

	return iErr2 == 0 ? iErr : iErr2;
}

/*iRightDivisionComplexByComplex*/
int iRightDivisionComplexByComplex(
				double _dblReal1, double _dblImg1,
				double _dblReal2, double _dblImg2,
				double *_pdblRealOut, double *_pdblImgOut)
{
	int iErr = 0;
	if(_dblImg2 == 0)
	{
		if(_dblReal2 == 0)
		{//got NaN + i NaN
			iErr = 1;
			*_pdblRealOut	= _dblImg2 / _dblReal2;
			*_pdblImgOut	= *_pdblRealOut;
		}
		else
		{
			*_pdblRealOut	= _dblReal1 / _dblReal2;
			*_pdblImgOut	= _dblImg1 / _dblReal2;
		}
	}
	else if(_dblReal2 == 0)
	{
		*_pdblRealOut	= _dblImg1 / _dblImg2;
		*_pdblImgOut	= (-_dblReal1) / _dblImg2;
	}
	else
	{//Generic division algorithm

		if(dabss(_dblReal2) >= dabss(_dblImg2))
		{
			double dblRatio2	= _dblImg2 / _dblReal2;
			double dblSum		= _dblReal2 + dblRatio2 * _dblImg2;
			*_pdblRealOut		= (_dblReal1 + _dblImg1 * dblRatio2) / dblSum;
			*_pdblImgOut		= (_dblImg1 - _dblReal1 * dblRatio2) / dblSum;
		}
		else
		{
			double dblRatio2	= _dblReal2 / _dblImg2;
			double dblSum		= _dblImg2 +  dblRatio2 * _dblReal2;
			*_pdblRealOut		= (_dblReal1 * dblRatio2 + _dblImg1) / dblSum;
			*_pdblImgOut		= (_dblImg1 * dblRatio2 - _dblReal1) / dblSum;
		}
	}
	return iErr;
}

/*iRightDivisionRealMatrixByComplexMatrix*/
int iRightDivisionRealMatrixByComplexMatrix(
				double *_pdblReal1,								int _iInc1,
				double *_pdblReal2,		double *_pdblImg2,		int _iInc2,
				double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize)
{
	int iErr = 0;
	int iErr2 = 0;
	int iErr3 = 0;
	int iIndex		= 0; //Main loop index
	int iIndex1		= 0; //Loop index on left operand
	int iIndex2		= 0; //Loop index on right operand
	int iIndexOut	= 0; //Lopp index on result matrix

	if(_iInc2 == 0)
	{
		if(dabss(_pdblReal2[iIndex2]) + dabss(_pdblImg2[iIndex2]) == 0)
			iErr2 = 1;
	}

	for(iIndex = 0 ; iIndex < _iSize ; iIndex++)
	{
		iErr3 = iRightDivisionRealByComplex(_pdblReal1[iIndex1], _pdblReal2[iIndex2], _pdblImg2[iIndex2], &_pdblRealOut[iIndexOut], &_pdblImgOut[iIndexOut]);
		if(iErr3 != 0)
			iErr = iIndex + 1;
		iIndexOut	+= _iIncOut;
		iIndex1		+= _iInc1;
		iIndex2		+= _iInc2;
	}

	return iErr2 == 0 ? iErr : iErr2;
}

/*iRightDivisionRealByComplex*/
int iRightDivisionRealByComplex(
				double _dblReal1,
				double _dblReal2, double _dblImg2,
				double *_pdblRealOut, double *_pdblImgOut)
{
	int iErr = 0;
	if(_dblImg2 == 0)
	{
		*_pdblRealOut	= _dblReal1 / _dblReal2;
		*_pdblImgOut	= 0;
	}
	else if(_dblReal2 == 0)
	{
		*_pdblRealOut	= 0;
		*_pdblImgOut	= -_dblReal1 / _dblImg2;
	}
	else
	{
		double dblAbsSum = dabss(_dblReal2) + dabss(_dblImg2);

		if(dblAbsSum == 0)
		{
			iErr = 1;
			*_pdblRealOut	= _dblReal1 / dblAbsSum;
			*_pdblImgOut	= 0;
		}
		else
		{
			double dblReal1Sum	= _dblReal1 / dblAbsSum;
			double dblReal2Sum	= _dblReal2 / dblAbsSum;
			double dblImg2Sum	= _dblImg2 / dblAbsSum;
			double dblSum		= pow(dblReal2Sum, 2) + pow(dblImg2Sum,2);
			*_pdblRealOut		= (dblReal1Sum * dblReal2Sum) / dblSum;
			*_pdblImgOut		= (-dblReal1Sum * dblImg2Sum) / dblSum;
		}
	}
	return iErr;
}

/*iRightDivisionComplexMatrixByRealMatrix*/
int iRightDivisionComplexMatrixByRealMatrix(
				double *_pdblReal1,		double *_pdblImg1,		int _iInc1,
				double *_pdblReal2,								int _iInc2,
				double *_pdblRealOut,	double *_pdblImgOut,	int _iIncOut, int _iSize)
{
	int iErr = 0;
	int iErr2 = 0;
	int iIndex		= 0; //Main loop index
	int iIndex1		= 0; //Loop index on left operand
	int iIndex2		= 0; //Loop index on right operand
	int iIndexOut	= 0; //Lopp index on result matrix
	for(iIndex = 0 ; iIndex < _iSize ; iIndex++)
	{
		iErr2 = iRightDivisionComplexByReal(_pdblReal1[iIndex1], _pdblImg1[iIndex1], _pdblReal2[iIndex2], &_pdblRealOut[iIndexOut], &_pdblImgOut[iIndexOut]);
		if(iErr2 != 0)
			iErr = iIndex;
		iIndexOut	+= _iIncOut;
		iIndex1		+= _iInc1;
		iIndex2		+= _iInc2;
	}
	return iErr;
}

/*iRightDivisionComplexByReal*/
int iRightDivisionComplexByReal(
				double _dblReal1, double _dblImg1,
				double _dblReal2,
				double *_pdblRealOut, double *_pdblImgOut)
{
	int iErr = 0;
	if(_dblReal2 == 0)
		iErr = 1;

	*_pdblRealOut	= _dblReal1 / _dblReal2;
	*_pdblImgOut	= _dblImg1 / _dblReal2;

	return iErr;
}

/*iRightDivisionRealMatrixByRealMatrix*/
int iRightDivisionRealMatrixByRealMatrix(
				double *_pdblReal1, int _iInc1,
				double *_pdblReal2, int _iInc2,
				double *_pdblRealOut, int _iIncOut, int _iSize)
{
	int iIndex		= 0; //Main loop index
	int iIndex1		= 0; //Loop index on left operand
	int iIndex2		= 0; //Loop index on right operand
	int iIndexOut	= 0; //Lopp index on result matrix
	int iErr		= 0;

	for(iIndex = 0 ; iIndex < _iSize ; iIndex++)
	{
		if(_pdblReal2[iIndex2] == 0)
			iErr = iIndex + 1; //gné index a 0 en C donc iErr peut valoir 0 !!! Oo
		_pdblRealOut[iIndexOut] = _pdblReal1[iIndex1] / _pdblReal2[iIndex2];
		iIndexOut				+= _iIncOut;
		iIndex1					+= _iInc1;
		iIndex2					+= _iInc2;
	}
	return iErr;
}

int	iRightDivisionOfRealMatrix(
			double *_pdblReal1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	int _iRowsOut,	int _iColsOut, double* _pdblRcond)
{
	int iReturn	= 0;
	int iSize1	= _iRows1 * _iCols1;
	int iSize2	= _iRows2 * _iCols2;
	int iIndex	= 0;
	char cNorm	= 0;
	int iExit	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iInfo		= 0;
	int iMax		= 0;
	double dblRcond	= 0;

	double dblEps	= 0;
	double dblAnorm	= 0;

	double *pAf		= NULL;
	double *pAt		= NULL;
	double *pBt		= NULL;
	double *pDwork	= NULL;

	int *pRank	= NULL;
	int *pIpiv	= NULL;
	int *pJpvt	= NULL;
	int *pIwork	= NULL;

	iWorkMin	= Max(4 * _iCols2, Max(Min(_iRows2, _iCols2) + 3 * _iRows2 + 1, 2 * Min(_iRows2, _iCols2) + _iRows1));

	/* Array allocations*/
	pAf			= (double*)malloc(sizeof(double) * _iCols2 * _iRows2);
	pAt			= (double*)malloc(sizeof(double) * _iCols2 * _iRows2);
	pBt			= (double*)malloc(sizeof(double) * Max(_iRows2,_iCols2) * _iRows1);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) * _iCols2);
	pJpvt		= (int*)malloc(sizeof(int) * _iRows2);
	pIwork		= (int*)malloc(sizeof(int) * _iCols2);


	//C'est du grand nawak ca, on reserve toute la stack ! Oo

	cNorm		= '1';
	pDwork		= (double*)malloc(sizeof(double) * iWorkMin);
	dblEps		= F2C(dlamch)("eps",1L);
	dblAnorm	= C2F(dlange)(&cNorm, &_iRows2, &_iCols2, _pdblReal2, &_iRows2, pDwork);

	//tranpose A and B

	vTransposeRealMatrix(_pdblReal2, _iRows2, _iCols2, pAt);

	{
		int i,j,ij,ji;
		for(j = 0 ; j < _iRows1 ; j++)
		{
			for(i = 0 ; i < _iCols2 ; i++)
			{
				ij = i + j * Max(_iRows2, _iCols2);
				ji = j + i * _iRows1;
				pBt[ij]	= _pdblReal1[ji];
			}//for(j = 0 ; j < _iRows1 ; j++)
		}//for(i = 0 ; i < _iCols2 ; i++)
	}//bloc esthetique

	if(_iRows2 == _iCols2)
	{
		cNorm		= 'F';
		C2F(dlacpy)(&cNorm, &_iCols2, &_iCols2, pAt, &_iCols2, pAf, &_iCols2);
		C2F(dgetrf)(&_iCols2, &_iCols2, pAf, &_iCols2, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(dgecon)(&cNorm, &_iCols2, pAf, &_iCols2, &dblAnorm, &dblRcond, pDwork, pIwork, &iInfo);
			if(dblRcond > dsqrts(dblEps))
			{
				cNorm	= 'N';
				C2F(dgetrs)(&cNorm, &_iCols2, &_iRows1, pAf, &_iCols2, pIpiv, pBt, &_iCols2, &iInfo);
				vTransposeRealMatrix(pBt, _iCols2, _iRows1, _pdblRealOut);
				iExit = 1;
			}
		}

		if(iExit == 0)
		{
			//how to extract that ? Oo
			*_pdblRcond = dblRcond;
			iReturn = -1;
		}
	}

	if(iExit == 0)
	{
		dblRcond = dsqrts(dblEps);
		cNorm = 'F';
		iMax = Max(_iRows2, _iCols2);
		memset(pJpvt, 0x00, sizeof(int) * _iRows2);
		C2F(dgelsy1)(&_iCols2, &_iRows2, &_iRows1, pAt, &_iCols2, pBt, &iMax,
			pJpvt, &dblRcond, &pRank[0], pDwork, &iWorkMin, &iInfo);

		if(iInfo == 0)
		{
			if( _iRows2 != _iCols2 && pRank[0] < Min(_iRows2, _iCols2))
			{
				//how to extract that ? Oo
				iReturn = -2;
				*_pdblRcond = pRank[0];
			}

		//	TransposeRealMatrix(pBt, _iRows1, _iRows2, _pdblRealOut, Max(_iRows1,_iCols1), _iRows2);

			//Mega caca de la mort qui tue des ours a mains nues
			//mais je ne sais pas comment le rendre "beau" :(
			{
				int i,j,ij,ji;
				for(j = 0 ; j < _iRows2 ; j++)
				{
					for(i = 0 ; i < _iRows1 ; i++)
					{
						ij = i + j * _iRows1;
						ji = j + i * Max(_iRows2, _iCols2);
						_pdblRealOut[ij]	= pBt[ji];
					}//for(i = 0 ; i < _iRows2 ; i++)
				}//for(j = 0 ; j < _iRows1 ; j++)
			}//bloc esthetique
		}//if(iInfo == 0)
	}//if(bExit == 0)

	free(pAf);
	free(pAt);
	free(pBt);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pIwork);
	free(pDwork);
	return iReturn;
}


int	iRightDivisionOfComplexMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond)
{
	int iReturn	= 0;
	int iIndex1	= 0;
	int iIndex2	= 0;
	char cNorm	= 0;
	int iExit	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iInfo		= 0;
	int iMax		= 0;
	double dblRcond	= 0;

	double dblEps	= 0;
	double dblAnorm	= 0;

	doublecomplex *poVar1	= NULL;
	doublecomplex *poVar2	= NULL;
	doublecomplex *poOut	= NULL;
	doublecomplex *poAf		= NULL;
	doublecomplex *poAt		= NULL;
	doublecomplex *poBt		= NULL;
	doublecomplex *poDwork	= NULL;

	int *pRank	= NULL;
	int *pIpiv	= NULL;
	int *pJpvt	= NULL;
	double *pRwork	= NULL;

	iWorkMin	= Max(2*_iCols2, Min(_iRows2, _iCols2) + Max(2 * Min(_iRows2, _iCols2), Max(_iRows2 + 1, Min(_iRows2, _iCols2) + _iRows1)));

	/* Array allocations*/
	poVar1		= oGetDoubleComplexFromPointer(_pdblReal1,		_pdblImg1,		_iRows1 * _iCols1);
	poVar2		= oGetDoubleComplexFromPointer(_pdblReal2,		_pdblImg2,		_iRows2 * _iCols2);
	poOut		= oGetDoubleComplexFromPointer(_pdblRealOut,	_pdblImgOut,	_iRowsOut * _iColsOut);

	poAf		= (doublecomplex*)malloc(sizeof(doublecomplex) * _iRows2 * _iCols2);
	poAt		= (doublecomplex*)malloc(sizeof(doublecomplex) * _iRows2 * _iCols2);
	poBt		= (doublecomplex*)malloc(sizeof(doublecomplex) * Max(_iRows2, _iCols2) * _iRows1);
	poDwork		= (doublecomplex*)malloc(sizeof(doublecomplex) * iWorkMin);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) * _iCols2);
	pJpvt		= (int*)malloc(sizeof(int) * _iRows2);
	pRwork		= (double*)malloc(sizeof(double) * 2 * _iRows2);

	dblEps		= F2C(dlamch)("eps",1L);
	cNorm		= '1';
	dblAnorm	= C2F(zlange)(&cNorm, &_iRows2, &_iCols2, poVar2, &_iRows2, poDwork);

	//tranpose A and B

	vTransposeDoubleComplexMatrix(poVar2, _iRows2, _iCols2, poAt);

	{
		int i,j,ij,ji;
		for(j = 0 ; j < _iRows1 ; j++)
		{
			for(i = 0 ; i < _iCols2 ; i++)
			{
				ij = i + j * Max(_iRows2, _iCols2);
				ji = j + i * _iRows1;
				poBt[ij].r	= poVar1[ji].r;
				//Conjugate
				poBt[ij].i	= -poVar1[ji].i;
			}//for(j = 0 ; j < _iRows1 ; j++)
		}//for(i = 0 ; i < _iCols2 ; i++)
	}//bloc esthetique


	if(_iRows2 == _iCols2)
	{
		cNorm		= 'F';
		C2F(zlacpy)(&cNorm, &_iCols2, &_iCols2, poAt, &_iCols2, poAf, &_iCols2);
		C2F(zgetrf)(&_iCols2, &_iCols2, poAf, &_iCols2, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(zgecon)(&cNorm, &_iCols2, poAf, &_iCols2, &dblAnorm, &dblRcond, poDwork, pRwork, &iInfo);
			if(dblRcond > dsqrts(dblEps))
			{
				cNorm	= 'N';
				C2F(zgetrs)(&cNorm, &_iCols2, &_iRows1, poAf, &_iCols2, pIpiv, poBt, &_iCols2, &iInfo);
				vTransposeDoubleComplexMatrix(poBt, _iCols2, _iRows2, poOut);
				vGetPointerFromDoubleComplex(poOut, _iRowsOut * _iColsOut, _pdblRealOut, _pdblImgOut);
				iExit = 1;
			}
		}

		if(iExit == 0)
		{
			//how to extract that ? Oo
			*_pdblRcond = dblRcond;
			iReturn = -1;
		}
	}

	if(iExit == 0)
	{
		dblRcond = dsqrts(dblEps);
		cNorm = 'F';
		iMax = Max(_iRows2, _iCols2);
		memset(pJpvt, 0x00, sizeof(int) * _iRows2);
		C2F(zgelsy1)(&_iCols2, &_iRows2, &_iRows1, poAt, &_iCols2, poBt, &iMax,
			pJpvt, &dblRcond, pRank, poDwork, &iWorkMin, pRwork, &iInfo);

		if(iInfo == 0)
		{
			if( _iRows2 != _iCols2 && pRank[0] < Min(_iRows2, _iCols2))
			{
				//how to extract that ? Oo
				iReturn = -2;
				*_pdblRcond = pRank[0];
			}

		//	TransposeRealMatrix(pBt, _iRows1, _iRows2, _pdblRealOut, Max(_iRows1,_iCols1), _iRows2);

			//Mega caca de la mort qui tue des ours a mains nues
			//mais je ne sais pas comment le rendre "beau" :(
			{
				int i,j,ij,ji;
				for(j = 0 ; j < _iRows2 ; j++)
				{
					for(i = 0 ; i < _iRows1 ; i++)
					{
						ij = i + j * _iRows1;
						ji = j + i * Max(_iRows2, _iCols2);
						_pdblRealOut[ij]	= poBt[ji].r;
						//Conjugate
						_pdblImgOut[ij]		= -poBt[ji].i;
					}//for(i = 0 ; i < _iRows2 ; i++)
				}//for(j = 0 ; j < _iRows1 ; j++)
			}//bloc esthetique
		}//if(iInfo == 0)
	}//if(iExit == 0)


	vFreeDoubleComplexFromPointer(poVar1);
	vFreeDoubleComplexFromPointer(poVar2);
	vFreeDoubleComplexFromPointer(poOut);

	free(poAf);
	free(poAt);
	free(poBt);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pRwork);
	free(poDwork);
	return 0;
}

/*Matrix left division*/
int	iLeftDivisionOfRealMatrix(
			double *_pdblReal1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond)
{
	int iReturn	= 0;
	int iSize1	= _iRows1 * _iCols1;
	int iSize2	= _iRows2 * _iCols2;
	int iIndex	= 0;
	char cNorm	= 0;
	int iExit	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iInfo		= 0;
	int iMax		= 0;
	double dblRcond	= 0;

	double dblEps	= 0;
	double dblAnorm	= 0;

	double *pAf		= NULL;
	double *pXb		= NULL;
	double *pDwork	= NULL;

	int *pRank	= NULL;
	int *pIpiv	= NULL;
	int *pJpvt	= NULL;
	int *pIwork	= NULL;

	iWorkMin	= Max(4 * _iCols1, Max(Min(_iRows1, _iCols1) + 3 * _iRows1 + 1, 2 * Min(_iRows1, _iCols1) + _iCols2));

	/* Array allocations*/
	pAf			= (double*)malloc(sizeof(double) * _iRows1 * _iCols1);
	pXb			= (double*)malloc(sizeof(double) * Max(_iRows1,_iCols1) * _iCols1);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) * _iCols1);
	pJpvt		= (int*)malloc(sizeof(int) * _iCols1);
	pIwork		= (int*)malloc(sizeof(int) * _iCols1);


	//C'est du grand nawak ca, on reserve toute la stack ! Oo

	cNorm		= '1';
	pDwork		= (double*)malloc(sizeof(double) * iWorkMin);
	dblEps		= F2C(dlamch)("eps",1L);
	dblAnorm	= C2F(dlange)(&cNorm, &_iRows1, &_iCols1, _pdblReal1, &_iRows1, pDwork);

	if(_iRows1 == _iCols1)
	{
		cNorm		= 'F';
		C2F(dlacpy)(&cNorm, &_iCols1, &_iCols1, _pdblReal1, &_iCols1, pAf, &_iCols1);
		C2F(dgetrf)(&_iCols1, &_iCols1, pAf, &_iCols1, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(dgecon)(&cNorm, &_iCols1, pAf, &_iCols1, &dblAnorm, &dblRcond, pDwork, pIwork, &iInfo);
			if(dblRcond > dsqrts(dblEps))
			{
				cNorm	= 'N';
				C2F(dgetrs)(&cNorm, &_iCols1, &_iCols2, pAf, &_iCols1, pIpiv, _pdblReal2, &_iCols1, &iInfo);
				cNorm	= 'F';
				C2F(dlacpy)(&cNorm, &_iCols1, &_iCols2, _pdblReal2, &_iCols1, _pdblRealOut, &_iCols1);
				iExit = 1;
			}
		}
		if(iExit == 0)
		{
			//how to extract that ? Oo
			//how to extract that ? Oo
			*_pdblRcond = dblRcond;
			iReturn = -1;
		}
	}

	if(iExit == 0)
	{
		dblRcond = dsqrts(dblEps);
		cNorm = 'F';
		iMax = Max(_iRows1, _iCols1);
		C2F(dlacpy)(&cNorm, &_iRows1, &_iCols2, _pdblReal2, &_iRows1, pXb, &iMax);
		memset(pJpvt, 0x00, sizeof(int) * _iCols1);
		C2F(dgelsy1)(	&_iRows1, &_iCols1, &_iCols2, _pdblReal1, &_iRows1, pXb, &iMax,
						pJpvt, &dblRcond, &pRank[0], pDwork, &iWorkMin, &iInfo);

		if(iInfo == 0)
		{
			if( _iRows1 != _iCols1 && pRank[0] < Min(_iRows1, _iCols1))
			{
				//how to extract that ? Oo
				iReturn = -2;
				*_pdblRcond = pRank[0];
			}

			cNorm = 'F';
			C2F(dlacpy)(&cNorm, &_iCols1, &_iCols2, pXb, &iMax, _pdblRealOut, &_iCols1);
		}
	}

	free(pAf);
	free(pXb);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pIwork);
	free(pDwork);
	return 0;
}


/*Complex matrixes left division*/
int	iLeftDivisionOfComplexMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut, double *_pdblRcond)
{
	int iReturn = 0;
	int iSize1	= _iRows1 * _iCols1;
	int iSize2	= _iRows2 * _iCols2;
	int iIndex	= 0;
	char cNorm	= 0;
	int iExit	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iInfo		= 0;
	int iMax		= 0;
	double dblRcond	= 0;

	double dblEps	= 0;
	double dblAnorm	= 0;

	doublecomplex *pAf		= NULL;
	doublecomplex *pXb		= NULL;
	doublecomplex *pDwork	= NULL;
	doublecomplex *poVar1	= NULL;
	doublecomplex *poVar2	= NULL;
	doublecomplex *poOut	= NULL;

	double *pRwork			= NULL;

	int *pRank				= NULL;
	int *pIpiv				= NULL;
	int *pJpvt				= NULL;

	iWorkMin	= Max(2*_iCols1, Min(_iRows1, _iCols1) + Max(2 * Min(_iRows1, _iCols1), Max(_iCols1, Min(_iRows1, _iCols1) + _iCols2)));

	/* Array allocations*/
	poVar1		= oGetDoubleComplexFromPointer(_pdblReal1,	_pdblImg1,		_iRows1 * _iCols1);
	poVar2		= oGetDoubleComplexFromPointer(_pdblReal2,	_pdblImg2,		_iRows2 * _iCols2);
	poOut		= oGetDoubleComplexFromPointer(_pdblRealOut, _pdblImgOut,	_iRowsOut * _iColsOut);

	pAf			= (doublecomplex*)malloc(sizeof(doublecomplex) * _iRows1 * _iCols1);
	pXb			= (doublecomplex*)malloc(sizeof(doublecomplex) * Max(_iRows1,_iCols1) * _iColsOut);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) * _iCols1);
	pJpvt		= (int*)malloc(sizeof(int) * _iCols1);
	pRwork		= (double*)malloc(sizeof(double) * _iCols1*2);


	//C'est du grand nawak ca, on reserve toute la stack ! Oo

	cNorm		= '1';
	pDwork		= (doublecomplex*)malloc(sizeof(doublecomplex) * iWorkMin);
	dblEps		= F2C(dlamch)("eps",1L);
	dblAnorm	= C2F(zlange)(&cNorm, &_iRows1, &_iCols1, poVar1, &_iRows1, pDwork);

	if(_iRows1 == _iCols1)
	{
		cNorm		= 'F';
		C2F(zlacpy)(&cNorm, &_iCols1, &_iCols1,	poVar1, &_iCols1, pAf, &_iCols1);
		C2F(zlacpy)(&cNorm, &_iCols1, &_iCols2,	poVar2, &_iCols1, pXb, &_iCols1);
		C2F(zgetrf)(&_iCols1, &_iCols1, pAf, &_iCols1, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(zgecon)(&cNorm, &_iCols1, pAf, &_iCols1, &dblAnorm, &dblRcond, pDwork, pRwork, &iInfo);
			if(dblRcond > dsqrts(dblEps))
			{
				cNorm	= 'N';
				C2F(zgetrs)(&cNorm, &_iCols1, &_iCols2, pAf, &_iCols1, pIpiv, pXb, &_iCols1, &iInfo);
				cNorm	= 'F';
				C2F(zlacpy)(&cNorm, &_iCols1, &_iCols2, pXb, &_iCols1, poOut, &_iCols1);
				vGetPointerFromDoubleComplex(poOut, _iRowsOut * _iColsOut, _pdblRealOut, _pdblImgOut);
				iExit = 1;
			}
			else
			{
				//how to extract that ? Oo
				iReturn = -1;
				*_pdblRcond = dblRcond;
			}
		}
	}

	if(iExit == 0)
	{
		dblRcond = dsqrts(dblEps);
		cNorm = 'F';
		iMax = Max(_iRows1, _iCols1);
		C2F(zlacpy)(&cNorm, &_iRows1, &_iCols2, poVar2, &_iRows1, pXb, &iMax);
		memset(pJpvt, 0x00, sizeof(int) * _iCols1);
		C2F(zgelsy1)(	&_iRows1, &_iCols1, &_iCols2, poVar1, &_iRows1, pXb, &iMax,
			pJpvt, &dblRcond, &pRank[0], pDwork, &iWorkMin, pRwork, &iInfo);

		if(iInfo == 0)
		{
			if( _iRows1 != _iCols1 && pRank[0] < Min(_iRows1, _iCols1))
			{
				//how to extract that ? Oo
				iReturn = -2;
				*_pdblRcond = pRank[0];
			}

			cNorm = 'F';
			C2F(zlacpy)(&cNorm, &_iCols1, &_iCols2, pXb, &iMax, poOut, &_iCols1);
			vGetPointerFromDoubleComplex(poOut, _iRowsOut * _iColsOut, _pdblRealOut, _pdblImgOut);
		}
	}


	vFreeDoubleComplexFromPointer(poVar1);
	vFreeDoubleComplexFromPointer(poVar2);
	vFreeDoubleComplexFromPointer(poOut);

	free(pAf);
	free(pXb);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pRwork);
	free(pDwork);
	return 0;
}


/*
+---+---+
| 1 | 2 |		+---+---+---+
+---+---+		| 6 | 4 | 2 |
| 3 | 4 |	->	+---+---+---+
+---+---+		| 5 | 3 | 1 |
| 5 | 6 |		+---+---+---+
+---+---+
*/
void vPretransposeRealMatrix(double *_pdblRealIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
	{
		int Xa			= iIndex % _iRowsIn;
		int Ya			= iIndex / _iRowsIn;
		int Xb			= _iRowsIn - Xa - 1;
		int Yb			= _iColsIn - Ya - 1;
		int iNewCoord	= Xb * _iColsIn + Yb;

		_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
	}
}

void vPretransposeComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut, double *_pdblImgOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
	{
		int Xa			= iIndex % _iRowsIn;
		int Ya			= iIndex / _iRowsIn;
		int Xb			= _iRowsIn - Xa - 1;
		int Yb			= _iColsIn - Ya - 1;
		int iNewCoord	= Xb * _iColsIn + Yb;

		_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
		_pdblImgOut[iNewCoord]	= -_pdblImgIn[iIndex];
	}
}

void vTransposeRealMatrix(double *_pdblRealIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
	{
/*		int iNewCoord	= iIndex % _iColsIn * _iRowsIn + (iIndex / _iColsIn);
		_pdblRealOut[iIndex]	= _pdblRealIn[iNewCoord];
*/		int iNewCoord	= iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);
		_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
	}
}

void vTransposeComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut, double *_pdblImgOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
	{
		int iNewCoord	= iIndex % _iColsIn * _iRowsIn + (iIndex / _iColsIn);

		_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
		//Conjugate
		_pdblImgOut[iNewCoord]	= -_pdblImgIn[iIndex];
	}
}

void vTransposeDoubleComplexMatrix(doublecomplex *_poIn, int _iRowsIn, int _iColsIn, doublecomplex *_poOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
	{
		//int iNewCoord	= iIndex % _iColsIn * _iRowsIn + (iIndex / _iColsIn);
		int iNewCoord	= iIndex % _iRowsIn * _iColsIn + (iIndex / _iRowsIn);
		_poOut[iNewCoord].r	= _poIn[iIndex].r;
		//Conjugate
		_poOut[iNewCoord].i	= -_poIn[iIndex].i;
	}
}
