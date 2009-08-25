/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 * However, the API (profile of the functions in the header files) will be 
 * still available and supported in Scilab 6.
 */

#include "api_common.h"
#include "api_internal_common.h"
#include "api_double.h"
#include "api_internal_double.h"


#include "CallScilab.h"
#include "stack-c.h"

/*******************************/
/*   double matrix functions   */
/*******************************/

int getMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
{
	return getCommonMatrixOfDouble(_piAddress, 0, _piRows, _piCols, _pdblReal, NULL);
}

int getComplexMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfDouble(_piAddress, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

int getComplexZMatrixOfDouble(int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ)
{
	double *pdblReal = NULL;
	double *pdblImg	 = NULL;

	getCommonMatrixOfDouble(_piAddress, isVarComplex(_piAddress), _piRows, _piCols, &pdblReal, &pdblImg);
	*_pdblZ	= oGetDoubleComplexFromPointer(pdblReal, pdblImg, *_piRows * *_piCols);
	return 0;
}

int getCommonMatrixOfDouble(int* _piAddress, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	if(	_piAddress == NULL || 
			getVarType(_piAddress) != sci_matrix || 
			isVarComplex(_piAddress) != _iComplex)
	{
		return 1;
	}
	
	getVarDimension(_piAddress, _piRows, _piCols);

	if(_pdblReal != NULL)
	{
		*_pdblReal	= (double*)(_piAddress + 4);
	}
	if(_iComplex && _pdblImg != NULL)
	{
		*_pdblImg	= (double*)(_piAddress + 4) + *_piRows * *_piCols;
	}
	return 0;
}

int allocMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal)
{
	double *pdblReal	= NULL;

	int iRet = allocCommonMatrixOfDouble(_iVar, 0, _iRows, _iCols, &pdblReal, NULL);
	if(iRet != 0)
	{
		return 1;
	}

	*_pdblReal	= pdblReal;

	return 0;
}

int allocComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	int iRet = allocCommonMatrixOfDouble(_iVar, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet != 0)
	{
		return 1;
	}

	*_pdblReal	= pdblReal;
	*_pdblImg		= pdblImg;
	return 0;
}

int allocCommonMatrixOfDouble(int _iVar, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int* piAddr			= NULL;

	getNewVarAddressFromPosition(iNewPos, &piAddr);
	fillCommonMatrixOfDouble(piAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 4), _iRows * _iCols * (_iComplex + 1));
	return 0;
}

int fillCommonMatrixOfDouble(int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	_piAddress[0]		= sci_matrix;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= _iComplex;


	if(_pdblReal != NULL)
	{
		*_pdblReal		= (double*)(_piAddress + 4);
	}

	if(_iComplex != 0 && _pdblImg != NULL)
	{
		*_pdblImg	= *_pdblReal + _iRows * _iCols;
	}

	return 0;
}

int createMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal)
{
	double *pdblReal	= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	int iRet = allocMatrixOfDouble(_iVar, _iRows, _iCols, &pdblReal);
	if(iRet)
	{
		return 1;
	}

	C2F(dcopy)(&iSize, _pdblReal, &iOne, pdblReal, &iOne);
	return 0;
}

int createComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	int iRet = allocComplexMatrixOfDouble(_iVar, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	C2F(dcopy)(&iSize, _pdblReal,	&iOne, pdblReal,	&iOne);
	C2F(dcopy)(&iSize, _pdblImg,	&iOne, pdblImg,		&iOne);
	return 0;
}

int createComplexZMatrixOfDouble(int _iVar, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	int iRet						= 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;


	iRet = allocComplexMatrixOfDouble(_iVar, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);
	return 0;
}

int createNamedMatrixOfDouble(char* _pstName, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommunNamedMatrixOfDouble(_pstName, 0, _iRows, _iCols, _pdblReal, NULL);
}

int createNamedComplexMatrixOfDouble(char* _pstName, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommunNamedMatrixOfDouble(_pstName, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

int createNamedComplexZMatrixOfDouble(char* _pstName, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iSize					= _iRows * _iCols;
	int iRet					= 0;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

  C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfDouble(piAddr, 1, _iRows, _iCols, &pdblReal, &pdblImg);

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + sadr(4), iSize * (2) * 2);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int createCommunNamedMatrixOfDouble(char* _pstName, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iSize					= _iRows * _iCols;
	int iRet					= 0;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

  C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfDouble(piAddr, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	//copy data in stack
	memcpy(pdblReal, _pdblReal, sizeof(double) * _iRows * _iCols);

	if(_iComplex)
	{
		memcpy(pdblImg, _pdblImg, sizeof(double) * _iRows * _iCols);
	}

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + sadr(4), iSize * (_iComplex + 1) * 2);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int readNamedMatrixOfDouble(char* _pstName, int* _piRows, int* _piCols, double* _pdblReal)
{
	return readCommonNamedMatrixOfDouble(_pstName, 0, _piRows, _piCols, _pdblReal, NULL);
}

int readNamedComplexMatrixOfDouble(char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	return readCommonNamedMatrixOfDouble(_pstName, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

int readCommonNamedMatrixOfDouble(char* _pstName, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	int* piAddr				= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	getVarAddressFromName(_pstName, &piAddr);
	
	getCommonMatrixOfDouble(piAddr, _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return 0;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piRows * *_piCols);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piRows * *_piCols);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
