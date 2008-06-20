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

#include "double.h"

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
			double *_pdblRealOut,	int _iRowsOut,	int _iColsOut)
{
	int iSize1	= _iRows1 * _iCols1;
	int iSize2	= _iRows2 * _iCols2;
	int iIndex	= 0;
	char cNorm	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iWork		= 0;
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

	/* Array allocations*/
	pAf			= (double*)malloc(sizeof(double) * _iRows1 * _iCols1);
	pAt			= (double*)malloc(sizeof(double) * _iRows1 * _iCols1);
	pBt			= (double*)malloc(sizeof(double) * Max(_iRows1,_iCols1) * _iRows2);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) * _iCols1);
	pJpvt		= (int*)malloc(sizeof(int) * _iRows1);
	pIwork		= (int*)malloc(sizeof(int) * _iCols1);

	iWorkMin	= Max(4 * _iCols1, Max(Min(_iRows1, _iCols1) + 3 * _iRows1 + 1, 2 * Min(_iRows1, _iCols1) + _iRows2));
	iWork		= Maxvol(11,"d");

	if(iWork <= iWorkMin)
	{//Not enough space in stack 
		Err = iWork - iWorkMin;
		return 17;//Error
	}

	//C'est du grand nawak ca, on reserve toute la stack ! Oo

	cNorm		= '1';
	pDwork		= (double*)malloc(sizeof(double) * iWork);
	dblEps		= F2C(dlamch)("eps",1L);
	dblAnorm	= C2F(dlange)(&cNorm, &_iRows1, &_iCols1, _pdblReal1, &_iRows1, pDwork);

	//tranpose A and B

	vTransposeRealMatrix(_pdblReal1, _iRows1, _iCols1, pBt);
	vTransposeRealMatrix(_pdblReal2, _iRows2, _iCols2, pAt);

	if(_iRows1 == _iCols1)
	{
		cNorm		= 'F';
		C2F(dlacpy)(&cNorm, &_iCols1, &_iCols1, pAt, &_iCols1, pAf, &_iCols1);
		C2F(dgetrf)(&_iCols1, &_iCols1, pAf, &_iCols1, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(dgecon)(&cNorm, &_iCols1, pAf, &_iCols1, &dblAnorm, &dblRcond, pDwork, pIwork, &iInfo);
			if(dblRcond > dsqrts(dblEps))
			{
				cNorm	= 'N';
				C2F(dgetrs)(&cNorm, &_iCols1, &_iRows2, pAf, &_iCols1, pIpiv, pBt, &_iCols1, &iInfo);
				vTransposeRealMatrix(pBt, _iCols1, _iRows2, _pdblRealOut);
				return 0;
			}
		}
		//how to extract that ? Oo
		sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
		Msgs(5,1);
	}

	dblRcond = dsqrts(dblEps);
	cNorm = 'F';
	iMax = Max(_iRows1, _iCols1);
	memset(pJpvt, 0x00, sizeof(int) * _iRows1);
	C2F(dgelsy1)(&_iCols1, &_iRows1, &_iRows2, pAt, &_iCols1, pBt, &iMax,
		pJpvt, &dblRcond, &pRank[0], pDwork, &iWork, &iInfo);

	if(iInfo != 0)
		return 0;

	if( _iRows1 != _iCols1 && pRank[0] < Min(_iRows1, _iCols1))
		//how to extract that ? Oo
		Msgs(9, pRank[0]);

//	TransposeRealMatrix(pBt, _iRows1, _iRows2, _pdblRealOut, Max(_iRows1,_iCols1), _iRows2);

	//Mega caca de la mort qui tue des ours a mains nues 
	//mais je ne sais pas comment le rendre "beau" :(
	{
		int i,j,ij,ji;
		for(j = 0 ; j < _iRows1 ; j++)
		{
			for(i = 0 ; i < _iRows2 ; i++)
			{
				ij = i + j * _iRows2;
				ji = j + i * Max(_iRows1, _iCols1);
				_pdblRealOut[ij]	= pBt[ji];
			}
		}
	}
	free(pAf);
	free(pAt);
	free(pBt);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pIwork);
	return 0;
}


int	iRightDivisionOfComplexMatrix(
			double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut)
{
	int iSize1	= _iRows1 * _iCols1;
	int iSize2	= _iRows2 * _iCols2;
	int iIndex	= 0;
	char cNorm	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iWork		= 0;
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

	/* Array allocations*/
	poVar1		= oGetDoubleComplexFromPointer(_pdblReal1,		_pdblImg1,		_iRows1 * _iCols1);
	poVar2		= oGetDoubleComplexFromPointer(_pdblReal2,		_pdblImg2,		_iRows2 * _iCols2);
	poOut		= oGetDoubleComplexFromPointer(_pdblRealOut,	_pdblImgOut,	_iRowsOut * _iColsOut);
	
	poAf		= (doublecomplex*)malloc(sizeof(doublecomplex) * _iRows1 * _iCols1);
	poAt		= (doublecomplex*)malloc(sizeof(doublecomplex) * _iRows2 * _iCols2);
	poBt		= (doublecomplex*)malloc(sizeof(doublecomplex) * Max(_iRows1, _iCols1) * _iRows2);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) * _iCols1);
	pJpvt		= (int*)malloc(sizeof(int) * _iRows1);
	pRwork		= (double*)malloc(sizeof(double) * 2 * _iRows1);

	iWorkMin	= Max(2*_iCols1, Min(_iRows1, _iCols1) + Max(2 * Min(_iRows1, _iCols1), Max(_iRows1 + 1, Min(_iRows1, _iCols1) + _iRows2)));
	iWork		= Maxvol(11,"z");

	if(iWork <= iWorkMin)
	{//Not enough space in stack 
		Err = iWork - iWorkMin;
		return 17;//Error
	}

	//C'est du grand nawak ca, on reserve toute la stack ! Oo

	cNorm		= '1';
	poDwork		= (doublecomplex*)malloc(sizeof(doublecomplex) * iWorkMin);
	dblEps		= F2C(dlamch)("eps",1L);
	dblAnorm	= C2F(dlange)(&cNorm, &_iRows1, &_iCols1, poVar1, &_iRows1, poDwork);

	//tranpose A and B

	vTransposeDoubleComplexMatrix(poVar1, _iRows1, _iCols1, poBt);
	vTransposeDoubleComplexMatrix(poVar2, _iRows2, _iCols2, poAt);

	if(_iRows1 == _iCols1)
	{
		cNorm		= 'F';
		C2F(zlacpy)(&cNorm, &_iCols1, &_iCols1, poAt, &_iCols1, poAf, &_iCols1);
		C2F(zgetrf)(&_iCols1, &_iCols1, poAf, &_iCols1, pIpiv, &iInfo);
		if(iInfo == 0)
		{
			cNorm = '1';
			C2F(zgecon)(&cNorm, &_iCols1, poAf, &_iCols1, &dblAnorm, &dblRcond, poDwork, pRwork, &iInfo);
			if(dblRcond > dsqrts(dblEps))
			{
				cNorm	= 'N';
				C2F(zgetrs)(&cNorm, &_iCols1, &_iRows2, poAf, &_iCols1, pIpiv, poBt, &_iCols1, &iInfo);
				vTransposeDoubleComplexMatrix(poBt, _iCols1, _iRows2, poOut);
				vGetPointerFromDoubleComplex(poOut, _iRowsOut * _iColsOut, _pdblRealOut, _pdblImgOut);
				return 0;
			}
		}
		//how to extract that ? Oo
		sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
		Msgs(5,1);
	}

	dblRcond = dsqrts(dblEps);
	cNorm = 'F';
	iMax = Max(_iRows1, _iCols1);
	memset(pJpvt, 0x00, sizeof(int) * _iRows1);
	C2F(zgelsy1)(&_iCols1, &_iRows1, &_iRows2, poAt, &_iCols1, poBt, &iMax,
		pJpvt, &dblRcond, &pRank[0], poDwork, &iWork, pRwork, &iInfo);

	if(iInfo != 0)
		return 0;

	if( _iRows1 != _iCols1 && pRank[0] < Min(_iRows1, _iCols1))
		//how to extract that ? Oo
		Msgs(9, pRank[0]);

//	TransposeRealMatrix(pBt, _iRows1, _iRows2, _pdblRealOut, Max(_iRows1,_iCols1), _iRows2);

	//Mega caca de la mort qui tue des ours a mains nues 
	//mais je ne sais pas comment le rendre "beau" :(
	{
		int i,j,ij,ji;
		for(j = 0 ; j < _iRows1 ; j++)
		{
			for(i = 0 ; i < _iRows2 ; i++)
			{
				ij = i + j * _iRows2;
				ji = j + i * Max(_iRows1, _iCols1);
				_pdblRealOut[ij]	= poBt[ji].r;
				//Conjugate
				_pdblImgOut[ij]		= -poBt[ji].i;
			}
		}
	}
	vFreeDoubleComplexFromPointer(poAf);
	vFreeDoubleComplexFromPointer(poAt);
	vFreeDoubleComplexFromPointer(poBt);
	vFreeDoubleComplexFromPointer(poDwork);

	free(pRank);
	free(pIpiv);
	free(pJpvt);
	return 0;
}

/*Matrix left division*/
int	iLeftDivisionOfRealMatrix(	
			double *_pdblReal1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	int _iRowsOut,	int _iColsOut)
{
	int iSize1	= _iRows1 * _iCols1;
	int iSize2	= _iRows2 * _iCols2;
	int iIndex	= 0;
	char cNorm	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iWork		= 0;
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

	/* Array allocations*/
	pAf			= (double*)malloc(sizeof(double) * _iRows1 * _iCols1);
	pXb			= (double*)malloc(sizeof(double) * Max(_iRows1,_iCols1) * _iCols1);

	pRank		= (int*)malloc(sizeof(int));
	pIpiv		= (int*)malloc(sizeof(int) * _iCols1);
	pJpvt		= (int*)malloc(sizeof(int) * _iCols1);
	pIwork		= (int*)malloc(sizeof(int) * _iCols1);
	
	iWorkMin	= Max(4 * _iCols1, Max(Min(_iRows1, _iCols1) + 3 * _iRows1 + 1, 2 * Min(_iRows1, _iCols1) + _iCols2));
	iWork		= Maxvol(10,"d");

	if(iWork <= iWorkMin)
	{//Not enough space in stack 
		Err = iWork - iWorkMin;
		return 17;//Error
	}

	//C'est du grand nawak ca, on reserve toute la stack ! Oo

	cNorm		= '1';
	pDwork		= (double*)malloc(sizeof(double) * iWork);
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
				return 0;
			}
		}
		//how to extract that ? Oo
		sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
		Msgs(5,1);
	}

	dblRcond = dsqrts(dblEps);
	cNorm = 'F';
	iMax = Max(_iRows1, _iCols1);
	C2F(dlacpy)(&cNorm, &_iRows1, &_iCols2, _pdblReal2, &_iRows1, pXb, &iMax);
	memset(pJpvt, 0x00, sizeof(int) * _iCols1);
	C2F(dgelsy1)(	&_iRows1, &_iCols1, &_iCols2, _pdblReal1, &_iRows1, pXb, &iMax,
					pJpvt, &dblRcond, &pRank[0], pDwork, &iWork, &iInfo);

	if(iInfo != 0)
		return 0;

	if( _iRows1 != _iCols1 && pRank[0] < Min(_iRows1, _iCols1))
		//how to extract that ? Oo
		Msgs(9, pRank[0]);

	cNorm = 'F';
	C2F(dlacpy)(&cNorm, &_iCols1, &_iCols2, pXb, &iMax, _pdblRealOut, &_iCols1);

	free(pAf);
	free(pXb);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pIwork);
	return 0;
}


/*Complex matrixes left division*/
int	iLeftDivisionOfComplexMatrix(	
			double *_pdblReal1,		double *_pdblImg1,		int _iRows1,	int _iCols1,
			double *_pdblReal2,		double *_pdblImg2,		int _iRows2,	int _iCols2,
			double *_pdblRealOut,	double *_pdblImgOut,	int _iRowsOut,	int _iColsOut)
{
	int iSize1	= _iRows1 * _iCols1;
	int iSize2	= _iRows2 * _iCols2;
	int iIndex	= 0;
	char cNorm	= 0;

	/*temporary variables*/
	int iWorkMin	= 0;
	int iWork		= 0;
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

	iWorkMin	= Max(2*_iCols1, Min(_iRows1, _iCols1) + Max(2 * Min(_iRows1, _iCols1), Max(_iCols1, Min(_iRows1, _iCols1) + _iCols2)));
	iWork		= Maxvol(10,"z");

	if(iWork <= iWorkMin)
	{//Not enough space in stack 
		Err = 2 * (iWork - iWorkMin);
		return 17;//Error
	}

	//C'est du grand nawak ca, on reserve toute la stack ! Oo

	cNorm		= '1';
	pDwork		= (doublecomplex*)malloc(sizeof(doublecomplex) * iWork);
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
				return 0;
			}
			else
			{
				//how to extract that ? Oo
				sprintf(C2F(cha1).buf, "%1.4E", dblRcond);
				Msgs(5,1);
			}
		}
	}

	dblRcond = dsqrts(dblEps);
	cNorm = 'F';
	iMax = Max(_iRows1, _iCols1);
	C2F(zlacpy)(&cNorm, &_iRows1, &_iCols2, poVar2, &_iRows1, pXb, &iMax);
	memset(pJpvt, 0x00, sizeof(int) * _iCols1);
	C2F(zgelsy1)(	&_iRows1, &_iCols1, &_iCols2, poVar1, &_iRows1, pXb, &iMax,
		pJpvt, &dblRcond, &pRank[0], pDwork, &iWork, pRwork, &iInfo);

	if(iInfo != 0)
		return 0;

	if( _iRows1 != _iCols1 && pRank[0] < Min(_iRows1, _iCols1))
		//how to extract that ? Oo
		Msgs(9, pRank[0]);

	cNorm = 'F';
	C2F(zlacpy)(&cNorm, &_iCols1, &_iCols2, pXb, &iMax, poOut, &_iCols1);

	vGetPointerFromDoubleComplex(poOut, _iRowsOut * _iColsOut, _pdblRealOut, _pdblImgOut);

	vFreeDoubleComplexFromPointer(pAf);
	vFreeDoubleComplexFromPointer(pXb);
	vFreeDoubleComplexFromPointer(pDwork);
	free(pRank);
	free(pIpiv);
	free(pJpvt);
	free(pRwork);
	return 0;
}


doublecomplex* oGetDoubleComplexFromPointer(double *_pdblReal, double *_pdblImg, int _iSize)
{
	int iIndex = 0;
	doublecomplex *poComplex = (doublecomplex*)malloc(sizeof(doublecomplex) * _iSize);

	if(_pdblReal != NULL && _pdblImg != NULL)
	{
		for(iIndex = 0; iIndex < _iSize ; iIndex++)
		{
			poComplex[iIndex].r = _pdblReal[iIndex];
			poComplex[iIndex].i = _pdblImg[iIndex];
		}
	}
	else if(_pdblReal != NULL && _pdblImg == NULL)
	{
		for(iIndex = 0; iIndex < _iSize ; iIndex++)
		{
			poComplex[iIndex].r = _pdblReal[iIndex];
			poComplex[iIndex].i = 0;
		}
	}
	else if(_pdblReal == NULL && _pdblImg != NULL)
	{
		for(iIndex = 0; iIndex < _iSize ; iIndex++)
		{
			poComplex[iIndex].r = 0;
			poComplex[iIndex].i = _pdblImg[iIndex];
		}
	}
	else
	{
		free(poComplex);
		return NULL;
	}
	return poComplex;
}

void vFreeDoubleComplexFromPointer(doublecomplex *_poComplex)
{
	if(_poComplex != NULL)
		free(_poComplex);
}

void vGetPointerFromDoubleComplex(doublecomplex *_poComplex, int _iSize, double *_pdblReal, double *_pdblImg)
{
	int iIndex = 0;

	if(_pdblReal != NULL && _pdblImg != NULL)
	{
		for(iIndex = 0; iIndex < _iSize ; iIndex++)
		{
			_pdblReal[iIndex] = _poComplex[iIndex].r;
			_pdblImg[iIndex] = _poComplex[iIndex].i;
		}
	}
	else if(_pdblReal != NULL && _pdblImg == NULL)
	{
		for(iIndex = 0; iIndex < _iSize ; iIndex++)
			_pdblReal[iIndex] = _poComplex[iIndex].r;
	}
	else if(_pdblReal == NULL && _pdblImg != NULL)
	{
		for(iIndex = 0; iIndex < _iSize ; iIndex++)
			_pdblImg[iIndex] = _poComplex[iIndex].i;
	}
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
		int iNewCoord	= iIndex % _iRowsIn * _iColsIn + ( iIndex / _iRowsIn);

		_pdblRealOut[iNewCoord]	= _pdblRealIn[iIndex];
	}
}

void vTransposeComplexMatrix(double *_pdblRealIn, double *_pdblImgIn, int _iRowsIn, int _iColsIn, double *_pdblRealOut, double *_pdblImgOut)
{
	int iIndex = 0;
	for(iIndex = 0 ; iIndex < _iRowsIn * _iColsIn ; iIndex++)
	{
		int iNewCoord	= iIndex % _iRowsIn * _iColsIn + ( iIndex / _iRowsIn);

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
		int iNewCoord	= iIndex % _iRowsIn * _iColsIn + ( iIndex / _iRowsIn);

		_poOut[iNewCoord].r	= _poIn[iIndex].r;
		//Conjugate
		_poOut[iNewCoord].i	= -_poIn[iIndex].i;
	}
}
