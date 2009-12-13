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

SciErr getMatrixOfString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piOffset = NULL;
	int *piData		= NULL;
	int iType			= 0;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
		return sciErr;
	}

	sciErr = getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfString", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(iType != sci_strings)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getMatrixOfString", _("string matrix"));
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfString", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(_piLength == NULL)
	{
		return sciErr;
	}

	piOffset = _piAddress + 4;

	//non cummulative length
	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		_piLength[i] = piOffset[i + 1] - piOffset[i];
	}

	if(_pstStrings == NULL || *_pstStrings == NULL)
	{
		return sciErr;
	}

	piData = piOffset + *_piRows * *_piCols + 1;

	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		if(_pstStrings[i] == NULL)
		{
			addErrorMessage(&sciErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
			return sciErr;
		}
		code2str(&_pstStrings[i], piData + iArraySum(_piLength, 0, i), _piLength[i]);
		_pstStrings[i][_piLength[i]] = 0;
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createMatrixOfString(void* _pvCtx, int _iVar, int _iRows, int _iCols, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int iTotalLen		= 0;
	int *piAddr			= NULL;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_STRING, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfString");
		return sciErr;
	}

	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 5 + _iRows * _iCols));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 5 + _iRows * _iCols + !((_iRows * _iCols) % 2)), (iTotalLen + 1) / (sizeof(double) / sizeof(int)));
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr fillMatrixOfString(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillMatrixOfString");
		return sciErr;
	}

	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		if(_pstStrings[i] == NULL)
		{
			addErrorMessage(&sciErr, API_ERROR_INVALID_SUBSTRING_POINTER, _("%s: Invalid argument address"), "getMatrixOfString");
			return sciErr;
		}

		int iLen = (int)strlen(_pstStrings[i]);
		str2code(piData + iOffset, &_pstStrings[i]);
		iOffset += iLen;
		piData[iOffset] = 0;
		piOffset[i + 1] = piOffset[i] + iLen;
	}

	*_piTotalLen	= piOffset[_iRows * _iCols] - 1;
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createNamedMatrixOfString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
	int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;

	int iTotalLen	= 0;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
	Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	sciErr = fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_STRING, _("%s: Unable to create %s named \"%s\""), "createNamedMatrixOfString", _("matrix of string"), _pstName);
		return sciErr;
	}

	//update "variable index"

	updateLstk(Top, sadr(iadr(*Lstk(Top)) + 5 + _iRows * _iCols), iTotalLen);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
	Rhs = iSaveRhs;
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr readNamedMatrixOfString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfString", _pstName);
		return sciErr;
	}

	sciErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfString", _pstName);
		return sciErr;
	}

	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getMatrixOfWideString(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType					= 0;
	char **pstStrings = NULL;
	int *piLenStrings = NULL;
	int strSize = 0;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfWideString");
		return sciErr;
	}

	sciErr = getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(iType != sci_strings)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getMatrixOfWideString", _("string matrix"));
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if (_piwLength == NULL)
	{
		return sciErr;
	}

	strSize = (*_piRows * *_piCols);
	piLenStrings	= (int*)MALLOC(sizeof(int) * strSize);

	// get length UTF size
	sciErr = getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLenStrings, pstStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
		return sciErr;
	}

	pstStrings = (char**)MALLOC(sizeof(char*) * strSize);

	for(int i = 0; i < strSize; i++)
	{
		pstStrings[i] = (char*)MALLOC(sizeof(char)*(piLenStrings[i] + 1));
	}

	// get strings UTF format
	sciErr = getMatrixOfString(_pvCtx, _piAddress, _piRows, _piCols, piLenStrings, pstStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_WIDE_STRING, _("%s: Unable to get argument #%d"), "getMatrixOfWideString", getRhsFromAddress(_pvCtx, _piAddress));
		if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
		freeArrayOfString(pstStrings,strSize);
		return sciErr;
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

	if ( (_pwstStrings == NULL) || (*_pwstStrings == NULL) )
	{
		if (piLenStrings) {FREE(piLenStrings);piLenStrings = NULL;}
		freeArrayOfString(pstStrings,strSize);
		return sciErr;
	}
	
	for (int i = 0; i < (*_piRows * *_piCols); i++)
	{
		if (pstStrings[i])
		{
			wchar_t *wcstring = to_wide_string(pstStrings[i]);
			if (wcstring)
			{
				if (_pwstStrings[i])
				{
					wcscpy(_pwstStrings[i], wcstring);
					_piwLength[i] = (int)wcslen(_pwstStrings[i]);
				}
				else
				{
					_pwstStrings[i] = NULL;
					_piwLength[i] = 0;
				}
				FREE(wcstring);
				wcstring = NULL;
			}
			else
			{
				// case to_wide_string fails
				_pwstStrings[i] = NULL;
				_piwLength[i] = 0;
			}
		}
		else
		{
			// case to_wide_string fails
			_pwstStrings[i] = NULL;
			_piwLength[i] = 0;
		}
	}

	freeArrayOfString(pstStrings, strSize);
	if (piLenStrings) {FREE(piLenStrings); piLenStrings = NULL;}

	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createMatrixOfWideString(void* _pvCtx, int _iVar, int _iRows, int _iCols, wchar_t** _pstwStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	char **pStrings = NULL;

	pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );

	for (int i = 0; i < (_iRows * _iCols) ; i++)
	{
		pStrings[i] = wide_string_to_UTF8(_pstwStrings[i]);
	}

	sciErr = createMatrixOfString(_pvCtx, _iVar, _iRows, _iCols, pStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_WIDE_STRING, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfWideString");
		return sciErr;
	}

	freeArrayOfString(pStrings, _iRows * _iCols);

	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr createNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int _iRows, int _iCols, wchar_t** _pwstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	char **pStrings = NULL;

	pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );

	for (int i = 0; i < (_iRows * _iCols) ; i++)
	{
		pStrings[i] = wide_string_to_UTF8(_pwstStrings[i]);
	}

	sciErr = createNamedMatrixOfString(_pvCtx, _pstName, _iRows, _iCols, pStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_WIDE_STRING, _("%s: Unable to create %s named \"%s\""), "createNamedMatrixOfWideString", _("matrix of wide string"), _pstName);
		return sciErr;
	}

	freeArrayOfString(pStrings, _iRows * _iCols);

	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr readNamedMatrixOfWideString(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_WIDE_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfWideString", _pstName);
		return sciErr;
	}

	sciErr = getMatrixOfWideString(_pvCtx, piAddr, _piRows, _piCols, _piwLength, _pwstStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_WIDE_STRING, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfWideString", _pstName);
		return sciErr;
	}

	return sciErr;
}
/*--------------------------------------------------------------------------*/
