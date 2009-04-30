/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 */

#include "common_api.h"
#include "double_api.h"

//#include <string.h>
#include "CallScilab.h"
#include "stack-c.h"

//double
int getCommonMatrixOfDouble(int* _piAddress, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
int allocCommonMatrixOfDouble(int _iVar, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
int fillCommonMatrixOfDouble(int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
int createCommunNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);
int readCommonNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

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

int allocMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal, int** _piAddress)
{
	int *piAddr				= NULL;
	double *pdblReal	= NULL;

	int iRet = getVarAddressFromNumber(Top - Rhs + _iVar, &piAddr);
	if(iRet != 0)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfDouble(_iVar, piAddr, 0, _iRows, _iCols, &pdblReal, NULL);
	if(iRet != 0)
	{
		return 1;
	}

	*_piAddress = piAddr;
	*_pdblReal	= pdblReal;

	return 0;
}

int allocComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg, int** _piAddress)
{
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	int iRet = getVarAddressFromNumber(Top - Rhs + _iVar, &piAddr);
	if(iRet != 0)
	{
		return 1;
	}

	iRet = allocCommonMatrixOfDouble(_iVar, piAddr, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet != 0)
	{
		return 1;
	}

	*_piAddress = piAddr;
	*_pdblReal	= pdblReal;
	*_pdblImg		= pdblImg;
	return 0;
}

int allocCommonMatrixOfDouble(int _iVar, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	fillCommonMatrixOfDouble(_piAddress, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	updateInterSCI(_iVar, '$', iAddr, iAddr + 4);
	updateLstk(iNewPos, iAddr + 4, _iRows * _iCols * (_iComplex + 1));
	return 0;
}

int fillCommonMatrixOfDouble(int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	_piAddress[0]		= sci_matrix;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= _iComplex;


	*_pdblReal		= (double*)(_piAddress + 4);
	if(_iComplex != 0 && _pdblImg != NULL)
		*_pdblImg	= *_pdblReal + _iRows * _iCols;

	return 0;
}

int createMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal, int** _piAddress)
{
	double *pdblReal	= NULL;
	int * piAddr			= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	int iRet = allocMatrixOfDouble(_iVar, _iRows, _iCols, &pdblReal, &piAddr);
	if(iRet)
	{
		return 1;
	}

	C2F(dcopy)(&iSize, _pdblReal, &iOne, pdblReal, &iOne);
	return 0;
}

int createComplexMatrixOfDouble(int _iVar, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg, int** _piAddress)
{
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;
	int * piAddr			= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	int iRet = allocComplexMatrixOfDouble(_iVar, _iRows, _iCols, &pdblReal, &pdblImg, &piAddr);
	if(iRet)
	{
		return 1;
	}

	C2F(dcopy)(&iSize, _pdblReal,	&iOne, pdblReal,	&iOne);
	C2F(dcopy)(&iSize, _pdblImg,	&iOne, pdblImg,		&iOne);
	return 0;
}

int createNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommunNamedMatrixOfDouble(_pstName, _iNameLen, 0, _iRows, _iCols, _pdblReal, NULL);
}

int createNamedComplexMatrixOfDouble(char* _pstName, int _iNameLen, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg, int** _piAddress)
{
	return createCommunNamedMatrixOfDouble(_pstName, _iNameLen, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

int createCommunNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iZero					= 0;
	int iOne					= 1;
	int iSize					= _iRows * _iCols;
	int iRet					= 0;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

  C2F(str2name)(_pstName, iVarID, _iNameLen);
  Top = Top + Nbvars + 1;

	iRet = getVarAddressFromNumber(Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfDouble(piAddr, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	//copy data in stack
	C2F(dcopy)(&iSize, _pdblReal, &iOne, pdblReal, &iOne);

	if(_pdblImg != NULL)
	{
		C2F(dcopy)(&iSize, _pdblImg, &iOne, pdblImg, &iOne);
	}

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 4, iSize * (_iComplex + 1));

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int readNamedMatrixOfDouble(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, double* _pdblReal)
{
	return readCommonNamedMatrixOfDouble(_pstName, _iNameLen, 0, _piRows, _piCols, _pdblReal, NULL);
}

int readNamedComplexMatrixOfDouble(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	return readCommonNamedMatrixOfDouble(_pstName, _iNameLen, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

int readCommonNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	int iVarID[nsiz];
	int* piAddr				= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;
	int iSize					= 0;
	int iOne					= 1;

	//get variable id from name
	C2F(str2name)(_pstName, iVarID, _iNameLen);

	//define scope of search
  Fin = -1;
	//search variable 
  C2F(stackg)(iVarID);

	if (Err > 0 || Fin == 0)
	{
		return 1;
	}

	//No idea :(
  if ( *Infstk(Fin) == 2)
		Fin = *istk(iadr(*Lstk(Fin )) + 1 + 1);

	//get variable address
	getVarAddressFromNumber(Fin, &piAddr);
	
	if(_iComplex == 1)
	{
		getComplexMatrixOfDouble(piAddr, _piRows, _piCols, &pdblReal, &pdblImg);
	}
	else
	{
		getMatrixOfDouble(piAddr, _piRows, _piCols, &pdblReal);
	}

	iSize = *_piRows * *_piCols;

	if(_pdblReal == NULL)
	{
		return 0;
	}

	C2F(dcopy)(&iSize, pdblReal, &iOne, _pdblReal, &iOne);
	if(_iComplex == 1)
	{
		C2F(dcopy)(&iSize, pdblImg, &iOne, _pdblImg, &iOne);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
