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
#include "CallScilab.h"
#include "stack-c.h"
extern "C"
{
#include "code2str.h"
#include "freeArrayOfString.h"
};

#include "alltypes.hxx"

using namespace types;
/*--------------------------------------------------------------------------*/

/*******************************/
/*   string matrix functions   */
/*******************************/

int getMatrixOfString(int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	String *pS = dynamic_cast<String*>((InternalType*)_piAddress);
	if(pS == NULL)
	{
		return 1;
	}

	*_piRows = pS->rows_get();
	*_piCols = pS->cols_get();

	if(_piLength == NULL)
	{
		return 0;
	}

	for(int i = 0 ; i < pS->size_get() ; i++)
	{
		_piLength[i] = (int)strlen(pS->string_get(i));
	}

	if(_pstStrings == NULL)
	{
		return 0;
	}

	for(int i = 0 ; i < pS->size_get() ; i++)
	{
		strcpy(_pstStrings[i], pS->string_get(i));
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int* _piKey)
{
	bool bRet = true;
	GatewayStruct *pStr =  (GatewayStruct*)_piKey;

	int iNewPos			= _iVar - (int)pStr->m_pin->size() - 1;

	String *pS = NULL;

	if(iNewPos < 0 /*|| iNewPos > *pStr->m_piRetCount*/)
	{
		return 1;
	}

	pS = new String(_iRows, _iCols);
	bRet = pS->string_set(_pstStrings);
	if(bRet == false)
	{
		return 1;
	}

	for(int i = (int)pStr->m_pout->size() ; i <= iNewPos ; i++)
	{
		pStr->m_pout->push_back(NULL);
	}

	(*pStr->m_pout)[iNewPos] = pS;

	return 0;
}
/*--------------------------------------------------------------------------*/
int fillMatrixOfString(int* _piAddress, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen)
{
	int* piOffset = NULL;
	int* piData		= NULL;
	int i					= 0;
	int iOffset		= 0;

	_piAddress[0]	= sci_strings;
	_piAddress[1] = _iRows;
	_piAddress[2] = _iCols;
	_piAddress[3] = 0; //always 0

	piOffset	= _piAddress + 4;
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

	*_piTotalLen	= piOffset[_iRows * _iCols] - 1;
	return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedMatrixOfString(char* _pstName, int _iRows, int _iCols, char** _pstStrings)
{
	int iVarID[nsiz];
	int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;

	int iTotalLen	= 0;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
	Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);


	//write matrix information
	fillMatrixOfString(piAddr, _iRows, _iCols, _pstStrings, &iTotalLen);

	//update "variable index"

	updateLstk(Top, sadr(iadr(*Lstk(Top)) + 5 + _iRows * _iCols), iTotalLen);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
	Rhs = iSaveRhs;
	return 0;
}
/*--------------------------------------------------------------------------*/
int readNamedMatrixOfString(char* _pstName, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	int iRet					= 0;
	int* piAddr				= NULL;

	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 1;
	}
	return getMatrixOfString(piAddr, _piRows, _piCols, _piLength, _pstStrings);
}
/*--------------------------------------------------------------------------*/
int getMatrixOfWideString(int* _piAddress, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
	char **pstStrings = NULL;
	int *piLenStrings = NULL;
	int i = 0;
	int ierr = 0;

	if(	_piAddress == NULL || getVarType(_piAddress) != sci_strings)
	{
		return 1;
	}

	getVarDimension(_piAddress, _piRows, _piCols);

	if (_piwLength == NULL)
	{
		return 0;
	}

	piLenStrings	= (int*)MALLOC(sizeof(int*) * (*_piRows * *_piCols));
	if (piLenStrings == NULL)
	{
		return 1;
	}

	// get length UTF size
	ierr = getMatrixOfString(_piAddress, _piRows, _piCols, piLenStrings, pstStrings);
	if (ierr)
	{
		if (piLenStrings) {FREE(piLenStrings); piLenStrings = NULL;}
		return ierr;
	}

	pstStrings = (char**)MALLOC(sizeof(char*) * (*_piRows * *_piCols));
	if (pstStrings == NULL)
	{
		if (piLenStrings) {FREE(piLenStrings); piLenStrings = NULL;}
		return 1;
	}

	for(i = 0; i < (*_piRows * *_piCols); i++)
	{
		pstStrings[i] = (char*)MALLOC(sizeof(char)*(piLenStrings[i] + 1));
		if (pstStrings[i] == NULL)
		{
			if (piLenStrings) {FREE(piLenStrings); piLenStrings = NULL;}
			freeArrayOfString(pstStrings, i);
			return 1;
		}
	}

	// get strings UTF format
	ierr = getMatrixOfString(_piAddress, _piRows, _piCols, piLenStrings, pstStrings);

	if (ierr == 0)
	{
		if (_pwstStrings == NULL)
		{
			for(i = 0; i < (*_piRows * *_piCols); i++)
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
		}
		else
		{
			for(i = 0; i < (*_piRows * *_piCols); i++)
			{
				_pwstStrings[i] = to_wide_string(pstStrings[i]);
				_piwLength[i] = (int)wcslen(_pwstStrings[i]);
			}
		}
	}

	freeArrayOfString(pstStrings, (*_piRows * *_piCols));
	FREE(piLenStrings); piLenStrings = NULL;

	return ierr;
}
/*--------------------------------------------------------------------------*/
int createMatrixOfWideString(int _iVar, int _iRows, int _iCols, wchar_t** _pstwStrings, int* _piKey)
{
	char **pStrings = NULL;
	int i = 0;
	int ierr = 0;

	pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );
	if(pStrings == NULL)
	{
		return 1;
	}

	for (i = 0; i < (_iRows * _iCols) ; i++)
	{
		pStrings[i] = wide_string_to_UTF8(_pstwStrings[i]);
	}

	ierr = createMatrixOfString(_iVar, _iRows, _iCols, pStrings, _piKey);
	freeArrayOfString(pStrings, _iRows * _iCols);

	return ierr;
}
/*--------------------------------------------------------------------------*/
int createNamedMatrixOfWideString(char* _pstName, int _iRows, int _iCols, wchar_t** _pwstStrings)
{
	char **pStrings = NULL;
	int i = 0;
	int ierr = 0;

	pStrings = (char**)MALLOC( sizeof(char*) * (_iRows * _iCols) );
	if(pStrings == NULL)
	{
		return 1;
	}

	for (i = 0; i < (_iRows * _iCols) ; i++)
	{
		pStrings[i] = wide_string_to_UTF8(_pwstStrings[i]);
	}

	ierr = createNamedMatrixOfString(_pstName, _iRows, _iCols, pStrings);
	freeArrayOfString(pStrings, _iRows * _iCols);

	return ierr;
}
/*--------------------------------------------------------------------------*/
int readNamedMatrixOfWideString(char* _pstName, int* _piRows, int* _piCols, int* _piwLength, wchar_t** _pwstStrings)
{
	int iRet				= 0;
	int* piAddr				= NULL;

	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 1;
	}

	return getMatrixOfWideString(piAddr, _piRows, _piCols, _piwLength, _pwstStrings);
}
/*--------------------------------------------------------------------------*/
