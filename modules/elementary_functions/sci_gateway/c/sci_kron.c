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
#include "basic_functions.h"
#include "sciprint.h"
#include "localization.h"

#define _NEW_TONIO_

void vKronR(	double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
				double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
				double* _pdblDataOut, int _iIncOut);
void vKronC(	double* _pdblRealIn1, double* _pdblImgIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
				double* _pdblRealIn2, double* _pdblImgIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
				double* _pdblRealOut, double* _pdblImgOut, int _iIncOut);


/*--------------------------------------------------------------------------*/
extern int C2F(intkron) _PARAMS((int *id));
extern int C2F(dcopy)(integer *,double *,integer *,double *,integer *);
/*--------------------------------------------------------------------------*/
int C2F(sci_kron) _PARAMS((char *fname,unsigned long fname_len))
{
	static int id[6];
#ifdef _NEW_TONIO_
	int iRows1					= 0;
	int iCols1					= 0;
	int iRealData1				= 0;
	int iImgData1				= 0;
	double *pdblRealData1		= NULL;
	double *pdblImgData1		= NULL;
	int iRows2					= 0;
	int iCols2					= 0;
	int iRealData2				= 0;
	int iImgData2				= 0;
	double *pdblRealData2		= NULL;
	double *pdblImgData2		= NULL;
	int iType					= 0;

	double* pReturnRealData1	= NULL;
	double* pReturnImgData1		= NULL;
	int iComplex1				= 0;
	int iComplex2				= 0;

	CheckRhs(2,2);
	CheckLhs(1,1);

	/*get first parameter*/
	if(iIsComplex(1))
	{
		GetRhsCVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex1, &iRows1, &iCols1, &iRealData1, &iImgData1);
		pdblRealData1 = stk(iRealData1);
		pdblImgData1 = stk(iImgData1);
	}
	else
	{
		iComplex1 = 0;
		GetRhsVar(1, MATRIX_OF_DOUBLE_DATATYPE, &iRows1, &iCols1, &iRealData1);
		pdblRealData1 = stk(iRealData1);
	}
	/*get second parameter*/
	if(iIsComplex(2))
	{
		GetRhsCVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iComplex2, &iRows2, &iCols2, &iRealData2, &iImgData2);
		pdblRealData2 = stk(iRealData2);
		pdblImgData2 = stk(iImgData2);
	}
	else
	{
		iComplex2 = 0;
		GetRhsVar(2, MATRIX_OF_DOUBLE_DATATYPE, &iRows2, &iCols2, &iRealData2);
		pdblRealData2 = stk(iRealData2);
	}

	/*Specials cases ./. and .\. */
	if(Fin == 20) // operator is ./.
	{
		OverLoad(1);
		return 0;
	}
	else if(Fin == 21) // operator is .\.
	{
		OverLoad(1);
		return 0;
	}

	if(iComplex1 == 0 && iComplex2 == 0)
	{//A rela and B real
		int iReturnRows = iRows1 * iRows2;
		int iReturnCols = iCols1 * iCols2;
		pReturnRealData1 = (double*)malloc(iReturnRows * iReturnCols * sizeof(double));
		vKronR(	pdblRealData1, iRows1, iRows1, iCols1, 
				pdblRealData2, iRows2, iRows2, iCols2,
				pReturnRealData1, iRows1 * iRows2);
		CreateVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iReturnRows, &iReturnCols, &pReturnRealData1);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData1);
	}
	else if(iComplex1 == 1 && iComplex2 == 0)
	{//A complex and B real
		int iReturnRows = iRows1 * iRows2;
		int iReturnCols = iCols1 * iCols2;
		pReturnRealData1 = (double*)malloc(iReturnRows * iReturnCols * sizeof(double));
		pReturnImgData1 = (double*)malloc(iReturnRows * iReturnCols * sizeof(double));
		/*Real part*/
		vKronR(	pdblRealData1, iRows1, iRows1, iCols1, 
				pdblRealData2, iRows2, iRows2, iCols2,
				pReturnRealData1, iRows1 * iRows2);
		/*Img part*/
		vKronR(	pdblImgData1, iRows1, iRows1, iCols1, 
				pdblRealData2, iRows2, iRows2, iCols2,
				pReturnImgData1, iRows1 * iRows2);
		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex1, &iReturnRows, &iReturnCols, &pReturnRealData1, &pReturnImgData1);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData1);
		free(pReturnImgData1);
	}
	else if(iComplex1 == 0 && iComplex2 == 1)
	{//A real and B complex
		int iReturnRows = iRows1 * iRows2;
		int iReturnCols = iCols1 * iCols2;
		pReturnRealData1 = (double*)malloc(iReturnRows * iReturnCols * sizeof(double));
		pReturnImgData1 = (double*)malloc(iReturnRows * iReturnCols * sizeof(double));
		/*Real part*/
		vKronR(	pdblRealData1, iRows1, iRows1, iCols1, 
				pdblRealData2, iRows2, iRows2, iCols2,
				pReturnRealData1, iRows1 * iRows2);
		/*Img part*/
		vKronR(	pdblRealData1, iRows1, iRows1, iCols1, 
				pdblImgData2, iRows2, iRows2, iCols2,
				pReturnImgData1, iRows1 * iRows2);
		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex2, &iReturnRows, &iReturnCols, &pReturnRealData1, &pReturnImgData1);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData1);
		free(pReturnImgData1);
	}
	else
	{//A complex and B complex
		int iReturnRows = iRows1 * iRows2;
		int iReturnCols = iCols1 * iCols2;
		pReturnRealData1 = (double*)malloc(iReturnRows * iReturnCols * sizeof(double));
		pReturnImgData1 = (double*)malloc(iReturnRows * iReturnCols * sizeof(double));
		/*Real part*/
		vKronC(	pdblRealData1, pdblImgData1, iRows1, iRows1, iCols1, 
				pdblRealData2, pdblImgData2, iRows2, iRows2, iCols2,
				pReturnRealData1, pReturnImgData1, iRows1 * iRows2);
		CreateCVarFromPtr(Rhs + 1, MATRIX_OF_DOUBLE_DATATYPE, &iComplex2, &iReturnRows, &iReturnCols, &pReturnRealData1, &pReturnImgData1);
		LhsVar(1) = Rhs + 1;
		PutLhsVar();
		free(pReturnRealData1);
		free(pReturnImgData1);
	}


#else //_NEW_TONIO_
	C2F(intkron)(id);
#endif
	return 0;
}

void vKronR(	double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
				double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
				double* _pdblDataOut, int _iIncOut)
{
	int iUn		= 1;
	int iLoop1	= 0;
	int iLoop2	= 0;
	int iLoop3	= 0;
	int iIndex1 = - _iIncIn1;
	int iIndex2 = - _iColsIn2;

	for(iLoop1 = 0 ; iLoop1 < _iColsIn1 ; iLoop1++)
	{
		int iIndex3 = 0; //Index a 1 ???
		iIndex1		+= _iIncIn1;
		iIndex2		+= _iColsIn2;
		for(iLoop2 = 0 ; iLoop2 < _iColsIn2 ; iLoop2++)
		{
			int iIndex4 = iIndex1;
			int iIndex5 = (iLoop2 + iIndex2) * _iIncOut;
			for(iLoop3 = 0 ; iLoop3 < _iRowsIn1 ; iLoop3++)
			{
				C2F(dcopy)(&_iRowsIn2, &_pdblDataIn2[iIndex3], &iUn, &_pdblDataOut[iIndex5], &iUn);
				ddscals(&_pdblDataOut[iIndex5], _iRowsIn2, _pdblDataIn1[iIndex4], &_pdblDataOut[iIndex5]);
				iIndex5 += _iRowsIn2;
				iIndex4++;
			}
			iIndex3 += _iIncIn2;
		}
	}
}
				
void vKronC(	double* _pdblRealIn1, double* _pdblImgIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
				double* _pdblRealIn2, double* _pdblImgIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
				double* _pdblRealOut, double* _pdblImgOut, int _iIncOut)
{

	int iLoop1 = 0, iLoop2 = 0, iLoop3 = 0, iLoop4 = 0;
	int iIndex1 = -_iIncIn1;
	int iIndex2 = -_iColsIn2;
	for(iLoop1 = 0 ; iLoop1 < _iColsIn1 ; iLoop1++)
	{
		int iIndex3 = 0;
		iIndex1		+= _iIncIn1;
		iIndex2		+= _iColsIn2;
		for(iLoop2 = 0 ; iLoop2 < _iColsIn2 ; iLoop2++)
		{
			int iIndex4 = iIndex1;
			int iIndex5 = (iLoop2 + iIndex2) * _iIncOut;
			for(iLoop3 = 0 ; iLoop3 < _iRowsIn1 ; iLoop3++)
			{
				for(iLoop4 = 0 ; iLoop4 < _iRowsIn2 ; iLoop4++)
				{
					_pdblRealOut[iIndex5 + iLoop4] = 
							_pdblRealIn1[iIndex4] * _pdblRealIn2[iIndex3 + iLoop4] - 
							_pdblImgIn1[iIndex4] * _pdblImgIn2[iIndex3 + iLoop4];
					_pdblImgOut[iIndex5 + iLoop4] = 
							_pdblRealIn1[iIndex4] * _pdblImgIn2[iIndex3 + iLoop4] + 
							_pdblImgIn1[iIndex4] * _pdblRealIn2[iIndex3 + iLoop4];
				}
				iIndex5 += _iRowsIn2;
				iIndex4++;
			}
			iIndex3 += _iIncIn2;
		}
	}
}
/*--------------------------------------------------------------------------*/
