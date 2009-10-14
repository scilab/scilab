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
#include "api_internal_poly.h"
#include "api_poly.h"
#include "localization.h"

#include "MALLOC.h"
#include "call_scilab.h"
#include "stack-c.h"
extern "C" {
#include "code2str.h"
};

StrErr getPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	char *pstVarName	= NULL;

	if(_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getPolyVariableName");
		return strErr;
	}

	if(_piAddress[0] != sci_poly)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getPolyVariableName", _("polynomial matrix"));
		return strErr;
	}

	*_piVarNameLen = 4;
	pstVarName = (char*)MALLOC(sizeof(char) * 5);
	code2str(&pstVarName, &_piAddress[4], *_piVarNameLen);
	for(int i = 0 ; i < *_piVarNameLen ; i++)
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
		addErrorMessage(&strErr, API_ERROR_INVALID_VAR_POINTER, _("%s: Unable to get formal variable name"), "getPolyVariableName");
		return strErr;
	}

	strcpy(_pstVarName, pstVarName);
	
	return strErr;
}

StrErr getMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return getCommonMatrixOfPoly(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

StrErr getComplexMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfPoly(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr getCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iType					= 0;
	int iSize					= 0;
	int *piOffset			= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	if(_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly");
		return strErr;
	}

	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}
	
	if(iType != sci_poly)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", _("polynomial matrix"));
		return strErr;
	}

	if(isVarComplex(_pvCtx, _piAddress) != _iComplex)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly");
		return strErr;
	}

	strErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	iSize	= *_piRows * *_piCols;

	if(_piNbCoef == NULL)
	{
		return strErr;
	}

	piOffset = _piAddress + 8; //4 for header and 4 for variable name
	for(int i = 0 ; i < iSize ; i++)
	{
		_piNbCoef[i]	= piOffset[i + 1] - piOffset[i];
	}

	if(_pdblReal == NULL)
	{
		return strErr;
	}

	pdblReal = (double*)(piOffset + iSize + 1 + ((iSize + 1) % 2 == 0 ? 0 : 1 ));
	for(int i = 0 ; i < iSize ; i++)
	{
		memcpy(_pdblReal[i], pdblReal + piOffset[i] - 1, sizeof(double) * _piNbCoef[i]);
	}

	if(_iComplex == 1)
	{
		pdblImg = pdblReal + piOffset[iSize] - 1;
		for(int i = 0 ; i < iSize ; i++)
		{
			memcpy(_pdblImg[i], pdblImg + piOffset[i] - 1, sizeof(double) * _piNbCoef[i]);
		}
	}
	return strErr;
}

StrErr createMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPoly(_pvCtx, _iVar, 0, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

StrErr createComplexMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPoly(_pvCtx, _iVar, 1, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr createCommonMatrixOfPoly(void* _pvCtx, int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr				= NULL;
	int iSize					= _iRows * _iCols;
	int iNewPos				= Top - Rhs + _iVar;
	int iAddr					= *Lstk(iNewPos);
	int iTotalLen			= 0;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
	strErr = fillCommonMatrixOfPoly(_pvCtx, piAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POLY, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createComplexMatrixOfPoly" : "createMatrixOfPoly");
		return strErr;
	}

	updateInterSCI(_iVar, '$', iAddr, iAddr + 4 + 4 + iSize + 1);
	updateLstk(iNewPos, iAddr + 4 + 4 + iSize + 1, iTotalLen);

	return strErr;
}

StrErr fillCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
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
		addErrorMessage(&strErr, API_ERROR_TOO_LONG_VAR, _("%s: Formal variable name of polynomial can't exceed 4 characters"));
		return strErr;
	}

	//fill variable name with space ( 40 sergescii )
	piVarName[0] = 40;
	piVarName[1] = 40;
	piVarName[2] = 40;
	piVarName[3] = 40;
	str2code(piVarName, &_pstVarName);

	piOffset = _piAddress + 8; //4 for header and 4 for variable name
	piOffset[0] = 1;
	for(int i = 0 ; i < iSize ; i++)
	{
		piOffset[i + 1] = piOffset[i] + _piNbCoef[i];
	}

	pdblReal = (double*)(piOffset + iSize + 1 + ((iSize + 1) % 2 == 0 ? 0 : 1 ));

	for(int i = 0 ; i < iSize ; i++)
	{
		memcpy(pdblReal + piOffset[i] - 1, _pdblReal[i], _piNbCoef[i] * sizeof(double));
	}

	if(_iComplex == 1)
	{
		pdblImg = pdblReal + piOffset[iSize] - 1;
		for(int i = 0 ; i < iSize ; i++)
		{
			memcpy(pdblImg + piOffset[i] - 1, _pdblImg[i], _piNbCoef[i] * sizeof(double));
		}
	}

	*_piTotalLen = (piOffset[iSize] - 1) * (_iComplex + 1) * 2;
	return strErr;
}

StrErr createNamedMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonNamedMatrixOfPoly(_pvCtx, _pstName, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

StrErr createNamedComplexMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonNamedMatrixOfPoly(_pvCtx, _pstName, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr createCommonNamedMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int iTotalLen			= 0;

	C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	strErr = fillCommonMatrixOfPoly(_pvCtx, piAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_NAMED_POLY, _("%s: Unable to create %s named \"%s\""), _iComplex ? "createNamedComplexMatrixOfPoly" : "createNamedMatrixOfPoly", _("matrix of double"), _pstName);
		return strErr;
	}


	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 4, iTotalLen);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr readNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return readCommonNamedMatrixOfPoly(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

StrErr readNamedComplexMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return readCommonNamedMatrixOfPoly(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr readCommonNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_POLY, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfPoly" :"readNamedMatrixOfPoly", _pstName);
		return strErr;
	}
	
	if(_iComplex == 1)
	{
		strErr = getComplexMatrixOfPoly(_pvCtx, piAddr, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	}
	else
	{
		strErr = getMatrixOfPoly(_pvCtx, piAddr, _piRows, _piCols, _piNbCoef, _pdblReal);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_POLY, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfPoly" :"readNamedMatrixOfPoly", _pstName);
		return strErr;
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/
