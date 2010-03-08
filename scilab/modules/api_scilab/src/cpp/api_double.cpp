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

SciErr getMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal)
{
	return getCommonMatrixOfDouble(_pvCtx, _piAddress, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr getComplexMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfDouble(_pvCtx, _piAddress, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

SciErr getComplexZMatrixOfDouble(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, doublecomplex** _pdblZ)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal = NULL;
	double *pdblImg	 = NULL;

	sciErr = getCommonMatrixOfDouble(_pvCtx, _piAddress, isVarComplex(_pvCtx, _piAddress), _piRows, _piCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ZDOUBLE, _("%s: Unable to get argument #%d"), "getComplexZMatrixOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	*_pdblZ	= oGetDoubleComplexFromPointer(pdblReal, pdblImg, *_piRows * *_piCols);
	return sciErr;
}

SciErr getCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType = 0;
	if(	_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble");
		return sciErr;
	}

	sciErr = getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr || iType != sci_matrix)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble", _("double matrix"));
		return sciErr;
	}

	if(isVarComplex(_pvCtx, _piAddress) != _iComplex)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), "getComplexMatrixOfDouble");
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfDouble" : "getMatrixOfDouble", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(_pdblReal != NULL)
	{
		*_pdblReal	= (double*)(_piAddress + 4);
	}
	if(_iComplex && _pdblImg != NULL)
	{
		*_pdblImg	= (double*)(_piAddress + 4) + *_piRows * *_piCols;
	}
	return sciErr;
}

SciErr allocMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal	= NULL;

	sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 0, _iRows, _iCols, &pdblReal, NULL);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocMatrixOfDouble");
		return sciErr;
	}

	*_pdblReal	= pdblReal;

	return sciErr;
}

SciErr allocComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return sciErr;
	}

	*_pdblReal	= pdblReal;
	*_pdblImg		= pdblImg;
	return sciErr;
}

SciErr allocCommonMatrixOfDouble(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int* piAddr			= NULL;

	int iMemSize = _iRows * _iCols * (_iComplex + 1) + 2;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
	fillCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 4));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 4), _iRows * _iCols * (_iComplex + 1));
	return sciErr;
}

SciErr fillCommonMatrixOfDouble(void* _pvCtx, int* _piAddress, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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

	return sciErr;
}

SciErr createMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double* _pdblReal)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal	= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	sciErr = allocMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return sciErr;
	}

	C2F(dcopy)(&iSize, _pdblReal, &iOne, pdblReal, &iOne);
	return sciErr;
}

SciErr createComplexMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	int iOne					= 1;
	int iSize					= _iRows * _iCols;

	sciErr = allocComplexMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_COMPLEX_DOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return sciErr;
	}

	C2F(dcopy)(&iSize, _pdblReal,	&iOne, pdblReal,	&iOne);
	C2F(dcopy)(&iSize, _pdblImg,	&iOne, pdblImg,		&iOne);
	return sciErr;
}

SciErr createComplexZMatrixOfDouble(void* _pvCtx, int _iVar, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;


	sciErr = allocComplexMatrixOfDouble(_pvCtx, _iVar, _iRows, _iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_ZDOUBLE, _("%s: Unable to create variable in Scilab memory"), "allocComplexMatrixOfDouble");
		return sciErr;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);
	return sciErr;
}

SciErr createNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommonNamedMatrixOfDouble(_pvCtx, _pstName, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr createNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommonNamedMatrixOfDouble(_pvCtx, _pstName, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

SciErr createNamedComplexZMatrixOfDouble(void* _pvCtx, char* _pstName, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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

	return sciErr;
}

SciErr createCommonNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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
		addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
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

	return sciErr;
}

SciErr readNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal)
{
	return readCommonNamedMatrixOfDouble(_pvCtx, _pstName, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr readNamedComplexMatrixOfDouble(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	return readCommonNamedMatrixOfDouble(_pvCtx, _pstName, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

SciErr readCommonNamedMatrixOfDouble(void* _pvCtx, char* _pstName, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_DOUBLE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfDouble" : "readNamedMatrixOfDouble", _pstName);
		return sciErr;
	}

	sciErr = getCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_DOUBLE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfDouble" : "readNamedMatrixOfDouble", _pstName);
		return sciErr;
	}

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return sciErr;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piRows * *_piCols);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piRows * *_piCols);
	}

	return sciErr;
}

/*shortcut functions*/

int isDoubleType(void* _pvCtx, int* _piAddress)
{
	return checkVarType(_pvCtx, _piAddress, sci_matrix);
}
/*--------------------------------------------------------------------------*/
int isNamedDoubleType(void* _pvCtx, char* _pstName)
{
	return checkNamedVarType(_pvCtx, _pstName, sci_matrix);
}
/*--------------------------------------------------------------------------*/
int getScalarDouble(void* _pvCtx, int* _piAddress, double* _pdblReal)
{
	return getCommonScalarDouble(_pvCtx, _piAddress, 0, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getScalarComplexDouble(void* _pvCtx, int* _piAddress, double* _pdblReal, double* _pdblImg)
{
	return getCommonScalarDouble(_pvCtx, _piAddress, 1, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonScalarDouble(void* _pvCtx, int* _piAddress, int _iComplex, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	double* pdblReal = NULL;
	double* pdblImg	 = NULL;

	sciErr = getCommonMatrixOfDouble(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_DOUBLE, _("%s: Unable to get argument #%d"), _iComplex ? "getScalarComplexDouble" : "getScalarDouble", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(isScalar(_pvCtx, _piAddress) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_DOUBLE, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), _iComplex ? "getScalarComplexDouble" : "getScalarDouble", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(_pdblReal != NULL)
	{
		*_pdblReal	= pdblReal[0];
	}

	if(_pdblImg != NULL)
	{
		*_pdblImg		= pdblImg[0];
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarDouble(void* _pvCtx, char* _pstName, double* _pdblReal)
{
	return getCommonNamedScalarDouble(_pvCtx, _pstName, 0, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getNamedScalarComplexDouble(void* _pvCtx, char* _pstName, double* _pdblReal, double* _pdblImg)
{
	return getCommonNamedScalarDouble(_pvCtx, _pstName, 1, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonNamedScalarDouble(void* _pvCtx, char* _pstName, int _iComplex, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	double dblReal = 0;
	double dblImg	 = 0;

	if(isNamedScalar(_pvCtx, _pstName) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_DOUBLE, _("%s: Wrong type for input argument \"%s\": A scalar expected.\n"), _iComplex ? "getNamedScalarComplexDouble" : "getNamedScalarDouble", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = readCommonNamedMatrixOfDouble(_pvCtx, _pstName, _iComplex, &iRows, &iCols, &dblReal, &dblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_DOUBLE, _("%s: Unable to get argument \"%s\""), _iComplex ? "getNamedScalarComplexDouble" : "getNamedScalarDouble", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(_pdblReal != NULL)
	{
		_pdblReal[0]	= dblReal;
	}

	if(_pdblImg != NULL)
	{
		_pdblImg[0]		= dblImg;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarDouble(void* _pvCtx, int _iVar, double _dblReal)
{
	return createCommonScalarDouble(_pvCtx, _iVar, 0, _dblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int createScalarComplexDouble(void* _pvCtx, int _iVar, double _dblReal, double _dblImg)
{
	return createCommonScalarDouble(_pvCtx, _iVar, 1, _dblReal, _dblImg);
}
/*--------------------------------------------------------------------------*/
static int createCommonScalarDouble(void* _pvCtx, int _iVar, int _iComplex, double _dblReal, double _dblImg)
{
	SciErr sciErr;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	sciErr = allocCommonMatrixOfDouble(_pvCtx, _iVar, _iComplex, 1, 1, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_DOUBLE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createScalarComplexDouble" : "createScalarDouble");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	pdblReal[0] = _dblReal;
	if(_iComplex)
	{
		pdblImg[0]	= _dblImg;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarDouble(void* _pvCtx, char* _pstName, double _dblReal)
{
	return createCommonNamedScalarDouble(_pvCtx, _pstName, 0, _dblReal, 0);
}
/*--------------------------------------------------------------------------*/
int createNamedScalarComplexDouble(void* _pvCtx, char* _pstName, double _dblReal, double _dblImg)
{
	return createCommonNamedScalarDouble(_pvCtx, _pstName, 1, _dblReal, _dblImg);
}
/*--------------------------------------------------------------------------*/
static int createCommonNamedScalarDouble(void* _pvCtx, char* _pstName, int _iComplex, double _dblReal, double _dblImg)
{
	SciErr sciErr;

	sciErr = createCommonNamedMatrixOfDouble(_pvCtx, _pstName, _iComplex, 1, 1, &_dblReal, &_dblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createNamedScalarComplexDouble" : "createNamedScalarDouble");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
