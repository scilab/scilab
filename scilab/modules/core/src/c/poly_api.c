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

#include "common_api.h"
#include "internal_common_api.h"
#include "poly_api.h"

#include "MALLOC.h"
#include "CallScilab.h"
#include "stack-c.h"
#include "code2str.h"

//internal poly functions
static int getCommonMatrixOfPoly(int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static int createCommonMatrixOfPoly(int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int** _piAddress);
static int fillCommonMatrixOfPoly(int* _piAddress, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen);
static int createCommonNamedComplexMatrixOfPoly(char* _pstName, int _iNameLen, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static int readCommonNamedMatrixOfPoly(char* _pstName, int _iNameLen, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);

int getPolyVariableName(int* _piAddress, char* _pstVarName, int* _piVarNameLen)
{
	int i							= 0;
	char *pstVarName	= NULL;

	pstVarName = (char*)MALLOC(sizeof(char) * 5);
	if(_piAddress == NULL || _piAddress[0] != sci_poly)
	{
		return 1;
	}

	*_piVarNameLen = 4;
	code2str(&pstVarName, &_piAddress[4], *_piVarNameLen);
	for(i = 0 ; i < *_piVarNameLen ; i++)
	{
		if(pstVarName[i] == ' ')
		{
			memset(pstVarName + i, 0x00, *_piVarNameLen - i);
			*_piVarNameLen = i;
			break;
		}
	}
	pstVarName[4] = 0;

	if(_pstVarName == NULL)
	{
		return 0;	
	}

	strcpy(_pstVarName, pstVarName);
	
	return 0;
}

int getMatrixOfPoly(int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return getCommonMatrixOfPoly(_piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

int getComplexMatrixOfPoly(int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfPoly(_piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

static int getCommonMatrixOfPoly(int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	int i							= 0;
	int iSize					= 0;
	int *piOffset			= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	if(	_piAddress == NULL || 
			getVarType(_piAddress) != sci_poly || 
			isVarComplex(_piAddress) != _iComplex)
	{
		return 1;
	}

	getVarDimension(_piAddress, _piRows, _piCols);
	iSize	= *_piRows * *_piCols;

	if(_piNbCoef == NULL)
	{
		return 0;
	}

	piOffset = _piAddress + 8; //4 for header and 4 for variable name
	for(i = 0 ; i < iSize ; i++)
	{
		_piNbCoef[i]	= piOffset[i + 1] - piOffset[i];
	}

	if(_pdblReal == NULL)
	{
		return 0;
	}

	pdblReal = (double*)(piOffset + iSize + 1 + ((iSize + 1) % 2 == 0 ? 0 : 1 ));
	for(i = 0 ; i < iSize ; i++)
	{
		memcpy(_pdblReal[i], pdblReal + piOffset[i] - 1, sizeof(double) * _piNbCoef[i]);
	}

	if(_iComplex == 1)
	{
		pdblImg = pdblReal + piOffset[iSize] - 1;
		for(i = 0 ; i < iSize ; i++)
		{
			memcpy(_pdblImg[i], pdblImg + piOffset[i] - 1, sizeof(double) * _piNbCoef[i]);
		}
	}
	return 0;
}

int createMatrixOfPoly(int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, int** _piAddress)
{
	return createCommonMatrixOfPoly(_iVar, 0, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, NULL, _piAddress);
}

int createComplexMatrixOfPoly(int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int** _piAddress)
{
	return createCommonMatrixOfPoly(_iVar, 1, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, _piAddress);
}

static int createCommonMatrixOfPoly(int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int** _piAddress)
{
	int *piAddr				= NULL;
	int iSize					= _iRows * _iCols;
	int iNewPos				= Top - Rhs + _iVar;
	int iAddr					= *Lstk(iNewPos);
	int iTotalLen			= 0;

	int iRet = getCommonVarAddressFromNumber(iNewPos, &piAddr);
	if(iRet != 0)
	{
		return 1;
	}

	fillCommonMatrixOfPoly(piAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);

	*_piAddress = piAddr;

	updateInterSCI(_iVar, '$', iAddr, iAddr + 4 + 4 + iSize + 1);
	updateLstk(iNewPos, iAddr + 4 + 4 + iSize + 1, iTotalLen);

	return 0;
}

static int fillCommonMatrixOfPoly(int* _piAddress, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen)
{
	int i							= 0;
	int* piOffset			= NULL;
	int *piVarName		= NULL;
	int iSize					= _iRows * _iCols;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	//header
	_piAddress[0] = sci_poly;
	_piAddress[1] = _iRows;
	_piAddress[2] = _iCols;
	_piAddress[3] = _iComplex;


	//4 for header
	piVarName = _piAddress + 4;//4 for header
	if(strlen(_pstVarName) > 4)//4 characters max
	{
		return 1;
	}

	//fill variable name with space ( 40 sergescii )
	piVarName[0] = 40;
	piVarName[1] = 40;
	piVarName[2] = 40;
	piVarName[3] = 40;
	str2code(piVarName, &_pstVarName);

	piOffset = _piAddress + 8; //4 for header and 4 for variable name
	piOffset[0] = 1;
	for(i = 0 ; i < iSize ; i++)
	{
		piOffset[i + 1] = piOffset[i] + _piNbCoef[i];
	}

	pdblReal = (double*)(piOffset + iSize + 1 + ((iSize + 1) % 2 == 0 ? 0 : 1 ));

	for(i = 0 ; i < iSize ; i++)
	{
		memcpy(pdblReal + piOffset[i] - 1, _pdblReal[i], _piNbCoef[i] * sizeof(double));
	}
	if(_iComplex == 1)
	{
		pdblImg = pdblReal + piOffset[iSize] - 1;
		for(i = 0 ; i < iSize ; i++)
		{
			memcpy(pdblImg + piOffset[i] - 1, _pdblImg[i], _piNbCoef[i] * sizeof(double));
		}
	}

	*_piTotalLen = (piOffset[iSize] - 1) * (_iComplex + 1) * 2;
	return 0;
}

int createNamedMatrixOfPoly(char* _pstName, int _iNameLen, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonNamedComplexMatrixOfPoly(_pstName, _iNameLen, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

int createNamedComplexMatrixOfPoly(char* _pstName, int _iNameLen, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonNamedComplexMatrixOfPoly(_pstName, _iNameLen, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

static int createCommonNamedComplexMatrixOfPoly(char* _pstName, int _iNameLen, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iSize					= _iRows * _iCols;
	int iRet					= 0;
	int *piAddr				= NULL;
	int iTotalLen			= 0;

	C2F(str2name)(_pstName, iVarID, _iNameLen);
  Top = Top + Nbvars + 1;

	iRet = getCommonVarAddressFromNumber(Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfPoly(piAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 4, iTotalLen);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int readNamedMatrixOfPoly(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return readCommonNamedMatrixOfPoly(_pstName, _iNameLen, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

int readNamedComplexMatrixOfPoly(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return readCommonNamedMatrixOfPoly(_pstName, _iNameLen, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

static int readCommonNamedMatrixOfPoly(char* _pstName, int _iNameLen, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
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
	getCommonVarAddressFromNumber(Fin, &piAddr);
	
	if(_iComplex == 1)
	{
		getComplexMatrixOfPoly(piAddr, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	}
	else
	{
		getMatrixOfPoly(piAddr, _piRows, _piCols, _piNbCoef, _pdblReal);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
