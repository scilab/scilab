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
#include "localization.h"


#include "call_scilab.h"
#include "stack-c.h"

/*******************************/
/*   double matrix functions   */
/*******************************/

StrErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
{
	return getCommonMatrixOfDouble(_pvCtx, _piAddress, 0, _piRows, _piCols, _pdblReal, NULL);
}

StrErr getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfDouble(_pvCtx, _piAddress, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

StrErr getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal = NULL;
	double *pdblImg	 = NULL;

	strErr = getCommonMatrixOfDouble(_pvCtx, _piAddress, isVarComplex(_pvCtx, _piAddress), _piRows, _piCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_ZDOUBLE, _("%s: Unable to get argument #%d"), "getComplexZMatrixOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}
	
	*_pdblZ	= oGetDoubleComplexFromPointer(pdblReal, pdblImg, *_piRows * *_piCols);
	return strErr;
}

StrErr getCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iType = 0;
	if(	_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble");
		return strErr;
	}
	
	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr || iType != sci_matrix)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble", _("double matrix"));
		return strErr;
	}

	if(isVarComplex(_pvCtx, _piAddress) != _iComplex)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), "getComplexMatrixOfDouble");
		return strErr;
	}

	strErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_DOUBLE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if(_pdblReal != NULL)
	{
		*_pdblReal	= (double*)(_piAddress + 4);
	}
	if(_iComplex && _pdblImg != NULL)
	{
		*_pdblImg	= (double*)(_piAddress + 4) + *_piRows * *_piCols;
	}
	return strErr;
}

StrErr allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal	= NULL;

	strErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 0, _iRows, _iCols, &pdblReal, NULL);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfDouble");
		return strErr;
	}

	*_pdblReal	= pdblReal;

	return strErr;
}

StrErr allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	strErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return strErr;
	}

	*_pdblReal	= pdblReal;
	*_pdblImg		= pdblImg;
	return strErr;
}

StrErr allocCommonMatrixOfDouble(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int* piAddr			= NULL;

	int iMemSize = _iRows * _iCols * (_iComplex + 1) + 2;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&strErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return strErr;
	}

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
	fillCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 4), _iRows * _iCols * (_iComplex + 1));
	return strErr;
}

StrErr fillCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
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

	return strErr;
}

StrErr createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double* _pdblReal)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal	= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	strErr = allocMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return strErr;
	}

	C2F(dcopy)(&iSize, _pdblReal, &iOne, pdblReal, &iOne);
	return strErr;
}

StrErr createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	strErr = allocComplexMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return strErr;
	}

	C2F(dcopy)(&iSize, _pdblReal,	&iOne, pdblReal,	&iOne);
	C2F(dcopy)(&iSize, _pdblImg,	&iOne, pdblImg,		&iOne);
	return strErr;
}

StrErr createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;


	strErr = allocComplexMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_ZDOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return strErr;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);
	return strErr;
}

StrErr createNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommunNamedMatrixOfDouble(_pvCtx, _pstName, 0, _iRows, _iCols, _pdblReal, NULL);
}

StrErr createNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommunNamedMatrixOfDouble(_pvCtx, _pstName, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

StrErr createNamedComplexZMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iSize					= _iRows * _iCols;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

  C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfDouble(_pvCtx, piAddr, 1, _iRows, _iCols, &pdblReal, &pdblImg);

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + sadr(4), iSize * (2) * 2);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr createCommunNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iSize					= _iRows * _iCols;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

  C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	int iMemSize = _iRows * _iCols * (_iComplex + 1) + 2;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(Top));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&strErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return strErr;
	}

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	fillCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
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

	return strErr;
}

StrErr readNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal)
{
	return readCommonNamedMatrixOfDouble(_pvCtx, _pstName, 0, _piRows, _piCols, _pdblReal, NULL);
}

StrErr readNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	return readCommonNamedMatrixOfDouble(_pvCtx, _pstName, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

StrErr readCommonNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_DOUBLE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfDouble" : "readNamedMatrixOfDouble", _pstName);
		return strErr;
	}
	
	strErr = getCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_DOUBLE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfDouble" : "readNamedMatrixOfDouble", _pstName);
		return strErr;
	}

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return strErr;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piRows * *_piCols);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piRows * *_piCols);
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/
