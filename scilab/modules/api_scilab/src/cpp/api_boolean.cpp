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

#include <string.h>
#include <stdlib.h>
#include "machine.h"
#include "call_scilab.h"
#include "api_scilab.h"
#include "api_internal_boolean.h"
#include "api_internal_common.h"
#include "stack-c.h"
#include "api_oldstack.h"
#include "localization.h"
#include "MALLOC.h"
#include "context.hxx"

using namespace std;
using namespace types;

/********************************/
/*   boolean matrix functions   */
/********************************/
SciErr getMatrixOfBoolean(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType = 0;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getMatrixOfBoolean");
		return sciErr;
	}

	sciErr = getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr || iType != sci_boolean)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getMatrixOfBoolean", _("boolean matrix"));
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN, _("%s: Unable to get argument #%d"), "getMatrixOfBoolean", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(_piBool)
	{
		*_piBool = ((InternalType*)_piAddress)->getAsBool()->bool_get();
	}
	return sciErr;
}

SciErr allocMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	if(_pvCtx == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "allocMatrixOfBoolean");
		return sciErr;
	}

	GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
  typed_list in = *pStr->m_pIn;
  InternalType** out = pStr->m_pOut;
  int*	piRetCount = pStr->m_piRetCount;
  char* pstName = pStr->m_pstName;

	Bool *pBool = new Bool(_iRows, _iCols);
	if(pBool == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), "allocMatrixOfBoolean");
		return sciErr;
	}

	int rhs = _iVar - api_Rhs((int*)_pvCtx);
	out[rhs - 1] = pBool;
	*_piBool = pBool->bool_get();
	if(*_piBool == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), "allocMatrixOfBoolean");
		return sciErr;
	}

	return sciErr;
}

SciErr fillMatrixOfBoolean(void* _pvCtx, int* _piAddress, int _iRows, int _iCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	_piAddress[0]	= sci_boolean;
	_piAddress[1] = Min(_iRows, _iRows * _iCols);
	_piAddress[2] = Min(_iCols, _iRows * _iCols);

	*_piBool		= _piAddress + 3;
	return sciErr;
}

SciErr createMatrixOfBoolean(void* _pvCtx, int _iVar, int _iRows, int _iCols, int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piBool		= NULL;

	sciErr = allocMatrixOfBoolean(_pvCtx, _iVar, _iRows, _iCols, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createMatrixOfBoolean");
		return sciErr;
	}

	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);
	return sciErr;
}

SciErr createNamedMatrixOfBoolean(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
	int iSaveRhs			= api_Rhs((int*)_pvCtx);
	int iSaveTop			= api_Top((int*)_pvCtx);
	int* piBool				= NULL;
	int *piAddr				= NULL;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
	Top = Top + Nbvars + 1;

	int iMemSize = (int)(((double)(_iRows * _iCols) / 2) + 2);
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(*Lstk(Top)));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	sciErr = fillMatrixOfBoolean(_pvCtx, piAddr, _iRows, _iCols, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_BOOLEAN, _("%s: Unable to create %s named \"%s\""), "createNamedMatrixOfBoolean", _("matrix of boolean"), _pstName);
		return sciErr;
	}

	//copy data in stack
	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);

	updateLstk(Top, *Lstk(Top) + sadr(3), (_iRows * _iCols) / (sizeof(double)/sizeof(int)));

	//Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	//Top = iSaveTop;
	//Rhs = iSaveRhs;
	return sciErr;
}

SciErr readNamedMatrixOfBoolean(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piBool				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfBoolean", _pstName);
		return sciErr;
	}

	sciErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN, _("%s: Unable to get variable \"%s\""), "readNamedMatrixOfBoolean", _pstName);
		return sciErr;
	}

	if(_piBool)
	{
		memcpy(_piBool, piBool, sizeof(int) * *_piRows * *_piCols);
	}

	return sciErr;
}

/*shortcut functions*/

/*--------------------------------------------------------------------------*/
int isBooleanType(void* _pvCtx, int* _piAddress)
{
	return checkVarType(_pvCtx, _piAddress, sci_boolean);
}
/*--------------------------------------------------------------------------*/
int isNamedBooleanType(void* _pvCtx, char* _pstName)
{
	return checkNamedVarType(_pvCtx, _pstName, sci_boolean);
}
/*--------------------------------------------------------------------------*/
int getScalarBoolean(void* _pvCtx, int* _piAddress, int* _piBool)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	int* piBool = NULL;

	sciErr = getMatrixOfBoolean(_pvCtx, _piAddress, &iRows, &iCols, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_BOOLEAN, _("%s: Unable to get argument #%d"), "getScalarBoolean", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(isScalar(_pvCtx, _piAddress) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SCALAR_BOOLEAN, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), "getScalarBoolean", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(_piBool != NULL)
	{
		*_piBool	= piBool[0];
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedScalarBoolean(void* _pvCtx, char* _pstName, int* _piBool)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	int iBool = 0;

	sciErr = readNamedMatrixOfBoolean(_pvCtx, _pstName, &iRows, &iCols, &iBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_BOOLEAN, _("%s: Unable to get argument \"%s\""), "getNamedScalarBoolean", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(isNamedScalar(_pvCtx, _pstName) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_SCALAR_BOOLEAN, _("%s: Wrong type for input argument \"%s\": A scalar expected.\n"), "getNamedScalarBoolean", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	if(_piBool != NULL)
	{
		*_piBool	= iBool;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int createScalarBoolean(void* _pvCtx, int _iVar, int _iBool)
{
	SciErr sciErr;
	int* piBool = NULL;

	sciErr = allocMatrixOfBoolean(_pvCtx, _iVar, 1, 1, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SCALAR_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createScalarBoolean");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	piBool[0] = _iBool;
	return 0;
}
/*--------------------------------------------------------------------------*/
int createNamedScalarBoolean(void* _pvCtx, char* _pstName, int _iBool)
{
	SciErr sciErr;

	sciErr = createNamedMatrixOfBoolean(_pvCtx, _pstName, 1, 1, &_iBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_SCALAR_BOOLEAN, _("%s: Unable to create variable in Scilab memory"), "createNamedScalarBoolean");
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/

