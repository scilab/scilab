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
#include "api_scilab.h"
#include "Scierror.h"

void vKronR(	double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
				double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
				double* _pdblDataOut, int _iIncOut);
void vKronC(	double* _pdblRealIn1, double* _pdblImgIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
				double* _pdblRealIn2, double* _pdblImgIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
				double* _pdblRealOut, double* _pdblImgOut, int _iIncOut);

int conv_real_input(double* _pdblData, int _iSize);
int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize);

/*--------------------------------------------------------------------------*/
extern int C2F(intkron) (int *id);
extern int C2F(dcopy)();
extern int C2F(dscal)();

/*--------------------------------------------------------------------------*/
int C2F(sci_kron) (char *fname,unsigned long fname_len)
{
	int iRet						= 0;

	int iRows1					= 0;
	int iCols1					= 0;
	double *pdblReal1		= NULL;
	double *pdblImg1		= NULL;
	int iComplex1				= 0;

	int iRows2					= 0;
	int iCols2					= 0;
	double *pdblReal2		= NULL;
	double *pdblImg2		= NULL;
	int iComplex2				= 0;

	int iRowsRet				= 0;
	int iColsRet				= 0;
	double* pdblRealRet	= NULL;
	double* pdblImgRet	= NULL;
	int iComplexRet			= 0;

	int* piAddr1				= NULL;
	int* piAddr2				= NULL;

	//static int id[6];
	//C2F(intkron)(id);
	//return 0;
	CheckRhs(2,2);
	CheckLhs(1,1);

	iRet = getVarAddressFromPosition(1, &piAddr1);
	if(iRet)
	{
		return 1;
	}

	iRet = getVarAddressFromPosition(2, &piAddr2);
	if(iRet)
	{
		return 1;
	}

	iComplex1 = isVarComplex(piAddr1);
	iComplex2 = isVarComplex(piAddr2);
	iComplexRet = iComplex1 | iComplex2;

	/*get first parameter*/
	if(isVarComplex(piAddr1))
	{
		iRet = getComplexMatrixOfDouble(piAddr1, &iRows1, &iCols1, &pdblReal1, &pdblImg1);
	}
	else
	{
		iRet = getMatrixOfDouble(piAddr1, &iRows1, &iCols1, &pdblReal1);
	}
	
	if(iRet)
	{
		return 1;
	}
	/*get second parameter*/
	if(isVarComplex(piAddr2))
	{
		iRet = getComplexMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal2, &pdblImg2);
	}
	else
	{
		iRet = getMatrixOfDouble(piAddr2, &iRows2, &iCols2, &pdblReal2);
	}

	if(iRet)
	{
		return 1;
	}
	if(iComplexRet)
	{
		iRowsRet = iRows1 * iRows2;
		iColsRet = iCols1 * iCols2;
		iRet = allocComplexMatrixOfDouble(Rhs + 1, iRowsRet, iColsRet, &pdblRealRet, &pdblImgRet);
	}
	else
	{
		iRowsRet = iRows1 * iRows2;
		iColsRet = iCols1 * iCols2;
		iRet = allocMatrixOfDouble(Rhs + 1, iRowsRet, iColsRet, &pdblRealRet);
	}

	if(iRet)
	{
		return 1;
	}

	/*Specials cases ./. and .\. */
	if(Fin == 20) // operator is ./.
	{
		if(iComplex2)
		{
			iRet = conv_img_input(pdblReal2, pdblImg2, iRows2 * iCols2);
		}
		else
		{
			iRet = conv_real_input(pdblReal2, iRows2 * iCols2);
		}

		if(iRet)
		{
			return 1;
		}
	}
	else if(Fin == 21) // operator is .\.
	{
		if(iComplex1)
		{
			iRet = conv_img_input(pdblReal1, pdblImg1, iRows1 * iCols1);
		}
		else
		{
			iRet = conv_real_input(pdblReal1, iRows1 * iCols1);
		}

		if(iRet)
		{
			return 1;
		}
	}

	if(iComplex1 == 0 && iComplex2 == 0)
	{//A real and B real
		vKronR(	pdblReal1, iRows1, iRows1, iCols1, 
						pdblReal2, iRows2, iRows2, iCols2,
						pdblRealRet, iRows1 * iRows2);
	}
	else if(iComplex1 == 1 && iComplex2 == 0)
	{//A complex and B real
		/*Real part*/
		vKronR(	pdblReal1, iRows1, iRows1, iCols1, 
						pdblReal2, iRows2, iRows2, iCols2,
						pdblRealRet, iRows1 * iRows2);
		/*Img part*/
		vKronR(	pdblImg1, iRows1, iRows1, iCols1, 
						pdblReal2, iRows2, iRows2, iCols2,
						pdblImgRet, iRows1 * iRows2);
	}
	else if(iComplex1 == 0 && iComplex2 == 1)
	{//A real and B complex
		/*Real part*/
		vKronR(	pdblReal1, iRows1, iRows1, iCols1, 
						pdblReal2, iRows2, iRows2, iCols2,
						pdblRealRet, iRows1 * iRows2);
		/*Img part*/
		vKronR(	pdblReal1, iRows1, iRows1, iCols1, 
						pdblImg2, iRows2, iRows2, iCols2,
						pdblImgRet, iRows1 * iRows2);
	}
	else
	{//A complex and B complex
		/*Real part*/
		vKronC(	pdblReal1, pdblImg1, iRows1, iRows1, iCols1, 
						pdblReal2, pdblImg2, iRows2, iRows2, iCols2,
						pdblRealRet, pdblImgRet, iRows1 * iRows2);
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

void vKronR(	double* _pdblDataIn1, int _iIncIn1, int _iRowsIn1, int _iColsIn1,
				double* _pdblDataIn2, int _iIncIn2, int _iRowsIn2, int _iColsIn2,
				double* _pdblDataOut, int _iIncOut)
{
	int iOne	= 1;
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
				C2F(dcopy)(&_iRowsIn2, &_pdblDataIn2[iIndex3], &iOne, &_pdblDataOut[iIndex5], &iOne);
//				ddscals(&_pdblDataOut[iIndex5], _iRowsIn2, _pdblDataIn1[iIndex4], &_pdblDataOut[iIndex5]);
				C2F(dscal)(&_iRowsIn2, &_pdblDataIn1[iIndex4], &_pdblDataOut[iIndex5], &iOne);

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

int conv_real_input(double* _pdblData, int _iSize)
{
	int i;
	for(i = 0 ; i < _iSize ; i++)
	{
		if(_pdblData[i] != 0)
		{
			_pdblData[i] = 1.0 / _pdblData[i];
		}
		else
		{
			Error(27);
			return 1;
		}
	}
	return 0;
}


int conv_img_input(double* _pdblReal, double* _pdblImg, int _iSize)
{
	int i;
	for(i = 0 ; i < _iSize ; i++)
	{
		double dblR			= _pdblReal[i];
		double dblI			= _pdblImg[i];

		double dblTemp	= dblR * dblR + dblI + dblI;
		if(dblTemp != 0)
		{
			_pdblReal[i]	= _pdblReal[i]	/ dblTemp;
			_pdblImg[i]	= - _pdblImg[i] / dblTemp;
		}
		else
		{
			Error(27);
			return 1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
