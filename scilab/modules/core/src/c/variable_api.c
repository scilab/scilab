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

#include "variable_api.h"

#include <string.h>
#include "stack-c.h"
#include "code2str.h"
#include "CallScilab.h"

extern int C2F(stackp)(int *,int *);

//local declaration, for internal use only !
//double
int getCommonMatrixOfDouble(int* _piAddress, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
int allocCommomMatrixOfDouble(int _iVar, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
int fillCommomMatrixOfDouble(int _iVar, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
int createCommunNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg, int** _piAddress);
int readCommonNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);

//string
int allocMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress);
int fillMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress, int* _piTotalLen);


int isVarMatrixType(int* _piAddress);
int updateInterSCI(int _iVar, char _cType, int _iSCIAddress, int _iSCIDataAddress);
int updateLstk(int _iNewPos, int _iSCIDataAddress, int _iVarSize);
void createNamedVariable(int *_piVarID);


//Generic functions
int getVarDimension(int* _piAddress, int* _piRows, int* _piCols)
{
	if(_piAddress != NULL && isVarMatrixType(_piAddress))
	{
		*_piRows		= _piAddress[1];
		*_piCols		= _piAddress[2];
		return 0;
	}
	else
	{
		*_piRows		= 0;
		*_piCols		= 0;
		return 1;
	}
}

int getVarAddressFromNumber(int _iVar, int** _piAddress)
{
	int iAddr = iadr(*Lstk(_iVar));
	*_piAddress = istk(iAddr);
	return 0;
}

int getVarType(int* _piAddress)
{
	if(_piAddress == NULL)
	{
		return 0;
	}
	return _piAddress[0];
}

int isVarComplex(int* _piAddress)
{
	int iType			= 0;
	int iComplex	= 0;

	if(_piAddress == NULL)
	{
		return 0;
	}

	iType = getVarType(_piAddress);
	switch(iType)
	{
	case sci_matrix :
	case sci_poly :
	case sci_sparse :
		iComplex = _piAddress[3];
		break;
	default:
		iComplex = 0;
	}
	return iComplex;
}

void createNamedVariable(int *_piVarID)
{
	int iZero				= 0;
	//it seems this part setting up the output format but "stackp" print anything
//	int iSaveLct		= C2F(iop).lct[3];
//  C2F(iop).lct[3] = -1;
  C2F(stackp)(_piVarID, &iZero);
//  C2F(iop).lct[3] = iSaveLct;
}

int updateInterSCI(int _iVar, char _cType, int _iSCIAddress, int _iSCIDataAddress)
{

	intersci_.ntypes[_iVar - 1]	= _cType;
	intersci_.iwhere[_iVar - 1]	= _iSCIAddress;
	intersci_.lad[_iVar - 1]		= _iSCIDataAddress;
	return 0;
}

int updateLstk(int _iNewpos, int _iSCIDataAddress, int _iVarSize)
{
	*Lstk(_iNewpos + 1) = _iSCIDataAddress + _iVarSize;
	return 0;
}

int isVarMatrixType(int* _piAddress)
{
	if(_piAddress != NULL)
	{
		int iType = getVarType(_piAddress);
		
		if(	iType == sci_matrix ||
				iType == sci_poly ||
				iType == sci_boolean ||
				iType == sci_sparse ||
				iType == sci_boolean_sparse ||
				iType == sci_matlab_sparse ||
				iType == sci_ints ||
				iType == sci_handles ||
				iType == sci_strings)
				return 1;
		else
		{
			return 0;
		}
	}
	return 1;
}

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

	iRet = allocCommomMatrixOfDouble(_iVar, piAddr, 0, _iRows, _iCols, &pdblReal, NULL);
	if(iRet != 0)
	{
		return 1;
	}

	*_piAddress = piAddr;
	*_pdblReal	= pdblReal;

	return iRet;
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

	iRet = allocCommomMatrixOfDouble(_iVar, piAddr, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet != 0)
	{
		return 1;
	}

	*_piAddress = piAddr;
	*_pdblReal	= pdblReal;
	*_pdblImg		= pdblImg;
	return iRet;
}

int allocCommomMatrixOfDouble(int _iVar, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	fillCommomMatrixOfDouble(_iVar, _piAddress, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	updateInterSCI(_iVar, '$', iAddr, iAddr + 4);
	updateLstk(iNewPos, iAddr + 4, _iRows * _iCols * (_iComplex + 1));
	return 0;
}

int fillCommomMatrixOfDouble(int _iVar, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
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

int createNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iRows, int _iCols, double* _pdblReal, int** _piAddress)
{
	return createCommunNamedMatrixOfDouble(_pstName, _iNameLen, 0, _iRows, _iCols, _pdblReal, NULL, _piAddress);
}

int createNamedComplexMatrixOfDouble(char* _pstName, int _iNameLen, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg, int** _piAddress)
{
	return createCommunNamedMatrixOfDouble(_pstName, _iNameLen, 1, _iRows, _iCols, _pdblReal, _pdblImg, _piAddress);
}

int createCommunNamedMatrixOfDouble(char* _pstName, int _iNameLen, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg, int** _piAddress)
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
	fillCommomMatrixOfDouble(Top, piAddr, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
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

/*******************************/
/*   string matrix functions   */
/*******************************/

int getMatrixOfString(int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	int i = 0;
	int *piOffset = NULL;
	int *piData		= NULL;

	if(	_piAddress == NULL || getVarType(_piAddress) != sci_strings)
	{
		return 1;
	}
	
	getVarDimension(_piAddress, _piRows, _piCols);

	if(_piLength == NULL)
	{
		return 0;
	}

	piOffset = _piAddress + 4;

	//non cummulative length
	for(i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		_piLength[i] = piOffset[i + 1] - piOffset[i];
	}

	if(_pstStrings == NULL)
	{
		return 0;
	}

	piData = piOffset + *_piRows * *_piCols + 1;

	for(i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		if(_pstStrings[i] == NULL)
		{
			return 1;
		}
		code2str(&_pstStrings[i], piData + iArraySum(_piLength, 0, i), _piLength[i]);
		_pstStrings[i][_piLength[i]] = 0;
	}

//	code2str(&_pstStrings, piData, iArraySum(_piLength, 0, *_piRows * *_piCols));
	return 0;
}

int getMatrixOfStringCumulativeLength(int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char* _pstStrings)
{
	int i = 0;
	int *piOffset = NULL;
	int *piData		= NULL;

	if(	_piAddress == NULL || getVarType(_piAddress) != sci_strings)
	{
		return 1;
	}
	
	getVarDimension(_piAddress, _piRows, _piCols);

	if(_piLength == NULL)
	{
		return 0;
	}

	piOffset = _piAddress + 4;

	//cummulative length
	for(i = 0 ; i < *_piRows * *_piCols + 1; i++)
	{
		_piLength[i] = piOffset[i];
	}

	if(_pstStrings == NULL)
	{
		return 0;
	}

	piData = piOffset + *_piRows * *_piCols + 1;
	code2str(&_pstStrings, piData, iArraySum(_piLength, 0, *_piRows * *_piCols));
	return 0;
}

int createMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress)
{
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int iTotalLen		= 0;

	fillMatrixOfString(iNewPos, _iRows, _iCols, _pstStrings, _piAddress, &iTotalLen);
	updateInterSCI(_iVar, '$', iAddr, iAddr + 5 + _iRows * _iCols);
	updateLstk(iNewPos, iAddr + 5 + _iRows * _iCols, iTotalLen);
	return 0;
}

int fillMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress, int* _piTotalLen)
{
	int iAddr				= *Lstk(_iVar);

	int* piOffset = NULL;
	int* piAddr		= NULL;
	int* piData		= NULL;
	int i					= 0;
	int iOffset		= 0;

	int iRet = getVarAddressFromNumber(_iVar, &piAddr);
	if(iRet != 0)
	{
		return 1;
	}

	piAddr[0]	= sci_strings;
	piAddr[1] = _iRows;
	piAddr[2] = _iCols;
	piAddr[3] = 0; //always 0

	piOffset	= piAddr + 4;
	piOffset[0] = 1; //Always 1
	piData		= piOffset + _iRows * _iCols + 1;

	for(i = 0 ; i < _iRows * _iCols ; i++)
	{
		int iLen = (int)strlen(_pstStrings[i]);
		str2code(piData + iOffset, &_pstStrings[i]);
		iOffset += iLen;
		piData[iOffset] = 0;
		piOffset[i + 1] = piOffset[i] + iLen;
	}

	*_piAddress		= piAddr;
	*_piTotalLen	= piOffset[_iRows * _iCols - 1];
	return 0;
}

int createNamedMatrixOfString(char* _pstName, int _iNameLen, int _iRows, int _iCols, char** _pstStrings)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iZero					= 0;
	int iOne					= 1;
	int iRet					= 0;
	int *piAddr				= NULL;

	int iTotalLen	= 0;

  C2F(str2name)(_pstName, iVarID, _iNameLen);
  Top = Top + Nbvars + 1;

	iRet = getVarAddressFromNumber(Top, &piAddr);


	//write matrix information
	fillMatrixOfString(Top, _iRows, _iCols, _pstStrings, &piAddr, &iTotalLen);

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 5, iTotalLen);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;
	return 0;
}

int readNamedMatrixOfString(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	int iVarID[nsiz];
	int* piAddr				= NULL;
	char** pstData		= NULL;
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
	
	return getMatrixOfString(piAddr, _piRows, _piCols, _piLength, _pstStrings);
}
