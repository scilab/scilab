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
#include "api_double.h"
#include "api_boolean.h"
#include "api_internal_common.h"
#include "api_internal_boolean.h"
#include "localization.h"

#include "Scierror.h"
#include "call_scilab.h"
#include "stack-c.h"


/********************************/
/*   boolean matrix functions   */
/********************************/
StrErr getMatrixOfBoolean(int* _piAddress, int* _piRows, int* _piCols, int** _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iType = 0;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("API_ERROR_INVALID_POINTER"));
		addErrorMessage(&strErr, API_ERROR_GET_BOOLEAN, _("API_ERROR_GET_BOOLEAN"));
		return strErr;
	}
	
	strErr = getVarType(_piAddress, &iType);
	if(strErr.iErr || iType != sci_boolean)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("API_ERROR_INVALID_TYPE"));
		return strErr;
	}

	strErr = getVarDimension(_piAddress, _piRows, _piCols);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_BOOLEAN, _("API_ERROR_GET_BOOLEAN"));
		return strErr;
	}

	if(_piBool)
	{
		*_piBool = _piAddress + 3;
	}
	return strErr;
}

StrErr allocMatrixOfBoolean(int _iVar, int _iRows, int _iCols, int** _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr	= NULL;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	int iSize = _iRows * _iCols;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr) + 3);
	if (iSize > iFreeSpace)
	{
		addErrorMessage(&strErr, API_ERROR_NO_MORE_MEMORY, _("API_ERROR_NO_MORE_MEMORY"));
		addErrorMessage(&strErr, API_ERROR_GET_BOOLEAN, _("API_ERROR_GET_BOOLEAN"));
		return strErr;
	}

	getNewVarAddressFromPosition(iNewPos, &piAddr);
	fillMatrixOfBoolean(piAddr, _iRows, _iCols, _piBool);

	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 3));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 3), (_iRows * _iCols) / (sizeof(double) / sizeof(int)));

	return strErr;
}

StrErr fillMatrixOfBoolean(int* _piAddress, int _iRows, int _iCols, int** _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	_piAddress[0]	= sci_boolean;
	_piAddress[1] = Min(_iRows, _iRows * _iCols);
	_piAddress[2] = Min(_iCols, _iRows * _iCols);

	*_piBool		= _piAddress + 3;
	return strErr;
}

StrErr createMatrixOfBoolean(int _iVar, int _iRows, int _iCols, int* _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piBool		= NULL;

	strErr = allocMatrixOfBoolean(_iVar, _iRows, _iCols, &piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN, _("API_ERROR_CREATE_BOOLEAN"));
		return strErr;
	}

	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);
	return strErr;
}

StrErr createNamedMatrixOfBoolean(char* _pstName, int _iRows, int _iCols, int* _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int* piBool				= NULL;
	int *piAddr				= NULL;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	int iSize = _iRows * _iCols;
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(*Lstk(Top)) + 3);
	if (iSize > iFreeSpace)
	{
		addErrorMessage(&strErr, API_ERROR_NO_MORE_MEMORY, _("API_ERROR_NO_MORE_MEMORY"));
		addErrorMessage(&strErr, API_ERROR_CREATE_NAMED_BOOLEAN, _("API_ERROR_CREATE_NAMED_BOOLEAN"));
		return strErr;
	}

	getNewVarAddressFromPosition(Top, &piAddr);

	//write matrix information
	strErr = fillMatrixOfBoolean(piAddr, _iRows, _iCols, &piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_NAMED_BOOLEAN, _("API_ERROR_CREATE_NAMED_BOOLEAN"));
		return strErr;
	}

	//copy data in stack
	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);

	updateLstk(Top, *Lstk(Top) + sadr(3), (_iRows * _iCols) / (sizeof(double)/sizeof(int)));

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;
	return strErr;
}

StrErr readNamedMatrixOfBoolean(char* _pstName, int* _piRows, int* _piCols, int* _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piBool				= NULL;

	strErr = getVarAddressFromName(_pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN, _("API_ERROR_READ_BOOLEAN"));
		return strErr;
	}

	strErr = getMatrixOfBoolean(piAddr, _piRows, _piCols, &piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN, _("API_ERROR_READ_BOOLEAN"));
		return strErr;
	}
	
	if(_piBool)
	{
		memcpy(_piBool, piBool, sizeof(int) * *_piRows * *_piCols);
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/

