/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Antoine ELIAS
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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

/*--------------------------------------------------------------------------*/
#include <string.h>

#include "charEncoding.h"
#include "MALLOC.h"
#include "api_common.h"
#include "api_internal_common.h"
#include "api_string.h"
#include "api_internal_string.h"
#include "call_scilab.h"
#include "stack-c.h"
#include "localization.h"

extern "C"
{
#include "code2str.h"
#include "freeArrayOfString.h"
};
/*--------------------------------------------------------------------------*/

/*******************************/
/*   string matrix functions   */
/*******************************/

StrErr getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piOffset = NULL;
	int *piData		= NULL;
	int iType			= 0;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
		return strErr;
	}

	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfString", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if(iType != sci_strings)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getMatrixOfString", _("string matrix"));
		return strErr;
	}

	strErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfString", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if(_piLength == NULL)
	{
		return strErr;
	}

	piOffset = _piAddress + 4;

	//non cummulative length
	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		_piLength[i] = piOffset[i + 1] - piOffset[i];
	}

	if(_pstStrings == NULL || *_pstStrings == NULL)
	{
		return strErr;
	}

	piData = piOffset + *_piRows * *_piCols + 1;

	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		if(_pstStrings[i] == NULL)
		{
			addErrorMessage(&strErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
			return strErr;
		}
		code2str(&_pstStrings[i], piData + iArraySum(_piLength, 0, i), _piLength[i]);
		_pstStrings[i][_piLength[i]] = 0;
	}
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int iTotalLen		= 0;
	int *piAddr			= NULL;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_STRING, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfString");
		return strErr;
	}

	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 5 + _iRows * _iCols));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 5 + _iRows * _iCols + !((_iRows * _iCols) % 2)), (iTotalLen + 1) / (sizeof(double) / sizeof(int)));
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr fillMatrixOfString(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piOffset = NULL;
	int* piData		= NULL;
	int iOffset		= 0;

	_piAddress[0]	= sci_strings;
	_piAddress[1] = _iRows;
	_piAddress[2] = _iCols;
	_piAddress[3] = 0; //always 0

	piOffset	= _piAddress + 4;
	piOffset[0] = 1; //Always 1
	piData		= piOffset + _iRows * _iCols + 1;
	
	if(_pstStrings == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillMatrixOfString");
		return strErr;
	}

	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		if(_pstStrings[i] == NULL)
		{
			addErrorMessage(&strErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
			return strErr;
		}

		int iLen = (int)strlen(_pstStrings[i]);
		str2code(piData + iOffset, &_pstStrings[i]);
		iOffset += iLen;
		piData[iOffset] = 0;
		piOffset[i + 1] = piOffset[i] + iLen;
	}

	*_piTotalLen	= piOffset[_iRows * _iCols] - 1;
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr createNamedMatrixOfString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
	int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;

	int iTotalLen	= 0;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
	Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	strErr = fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_NAMED_STRING, _("%s: Unable to create %s named \"%s\""), "createNamedMatrixOfString", _("matrix of string"), _pstName);
		return strErr;
	}

	//update "variable index"

	updateLstk(Top, sadr(iadr(*Lstk(Top)) + 5 + _iRows * _iCols), iTotalLen);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
	Rhs = iSaveRhs;
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr readNamedMatrixOfString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfString", _pstName);
		return strErr;
	}

	strErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfString", _pstName);
		return strErr;
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iType					= 0;
	char **pstStrings = NULL;
	int *piLenStrings = NULL;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfWideString");
		return strErr;
	}

	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if(iType != sci_strings)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getMatrixOfWideString", _("string matrix"));
		return strErr;
	}

	strErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if (_piwLength == NULL)
	{
		return strErr;
	}

	piLenStrings	= (int*)MALLOC(sizeof(int*) * (*_piRows * *_piCols));

	// get length UTF size
	strErr = getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLenStrings, pstStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	pstStrings = (char**)MALLOC(sizeof(char*) * (*_piRows * *_piCols));

	for(int i = 0; i < (*_piRows * *_piCols); i++)
	{
		pstStrings[i] = (char*)MALLOC(sizeof(char)*(piLenStrings[i] + 1));
	}

	// get strings UTF format
	strErr = getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLenStrings, pstStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	for(int i = 0; i < (*_piRows * *_piCols); i++)
	{
		wchar_t* wString = to_wide_string(pstStrings[i]);
		if (wString)
		{
			_piwLength[i] = (int)wcslen(wString);
			FREE(wString);
			wString = NULL;
		}
		else
		{
			// it should not be here
			_piwLength[i] = 0;
		}
	}

	if(_pwstStrings == NULL)
	{
		return strErr;
	}
	
	for(int i = 0; i < (*_piRows * *_piCols); i++)
	{
		_pwstStrings[i] = to_wide_string(pstStrings[i]);
		_piwLength[i] = (int)wcslen(_pwstStrings[i]);
	}

	freeArrayOfString(pstStrings, (*_piRows * *_piCols));
	FREE(piLenStrings); piLenStrings = NULL;

	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, wchar_t** _pstwStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	char **pStrings = NULL;

	pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );

	for (int i = 0; i < (_iRows * _iCols) ; i++)
	{
		pStrings[i] = wide_string_to_UTF8(_pstwStrings[i]);
	}

	strErr = createMatrixOfString(_pvCtx, _iVar, _iRows, _iCols, pStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_WIDE_STRING, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfWideString");
		return strErr;
	}

	freeArrayOfString(pStrings, _iRows * _iCols);

	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr createNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, wchar_t** _pwstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	char **pStrings = NULL;

	pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );

	for (int i = 0; i < (_iRows * _iCols) ; i++)
	{
		pStrings[i] = wide_string_to_UTF8(_pwstStrings[i]);
	}

	strErr = createNamedMatrixOfString(_pvCtx, _pstName, _iRows, _iCols, pStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_NAMED_WIDE_STRING, _("%s: Unable to create %s named \"%s\""), "createNamedMatrixOfWideString", _("matrix of wide string"), _pstName);
		return strErr;
	}

	freeArrayOfString(pStrings, _iRows * _iCols);

	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr readNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_WIDE_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfWideString", _pstName);
		return strErr;
	}

	strErr = getMatrixOfWideString(_pvCtx, piAddr, _piRows, _piCols, _piwLength, _pwstStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_WIDE_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfWideString", _pstName);
		return strErr;
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/
