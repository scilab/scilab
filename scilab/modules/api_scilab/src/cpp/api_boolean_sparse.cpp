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
#include "api_boolean_sparse.h"
#include "api_internal_boolean_sparse.h"
#include "localization.h"


#include "call_scilab.h"
#include "stack-c.h"
#include "MALLOC.h"

SciErr getBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType = 0;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getBooleanSparseMatrix");
		return sciErr;
	}

	sciErr =  getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr || iType != sci_boolean_sparse)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "getBooleanSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "getBooleanSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	*_piNbItem = _piAddress[4];

	if(_piNbItemRow == NULL)
	{
		return sciErr;
	}
	*_piNbItemRow = _piAddress + 5;//4 for header + 1 for NbItem

	if(_piColPos == NULL)
	{
		return sciErr;
	}
	*_piColPos = *_piNbItemRow + *_piRows;

	return sciErr;
}

SciErr allocBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int iPos				= 5 + _iRows + _iNbItem;
	int* piAddr			= NULL;

	int iMemSize = (int)( ( (double)iPos / 2 ) + 0.5);
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(iAddr));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);
	sciErr = fillBooleanSparseMatrix(_pvCtx, piAddr, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to create variable in Scilab memory"), "allocBooleanSparseMatrix");
		return sciErr;
	}

	iPos += iAddr;
	updateInterSCI(_iVar, '$', iAddr, iPos);
	updateLstk(iNewPos, iPos, 0);
	return sciErr;
}

SciErr fillBooleanSparseMatrix(void* _pvCtx, int *_piAddress, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	if(_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillBooleanSparseMatrix");
		return sciErr;
	}

	_piAddress[0]		= sci_boolean_sparse;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= 0;

	_piAddress[4]		= _iNbItem;

	*_piNbItemRow	= _piAddress + 5;//4 for header + 1 for NbItem
	*_piColPos		= *_piNbItemRow + _iRows;
	return sciErr;
}

SciErr createBooleanSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

	sciErr = allocBooleanSparseMatrix(_pvCtx, _iVar, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_SPARSE, _("%s: Unable to create variable in Scilab memory"), "createBooleanSparseMatrix");
		return sciErr;
	}

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
	return sciErr;
}

SciErr createNamedBooleanSparseMatrix(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iPos					= 0;

	int* piAddr				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

  C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	int iMemSize = (int)( ( (double)iPos / 2) + 0.5);
	int iFreeSpace = iadr(*Lstk(Bot)) - (iadr(Top));
	if (iMemSize > iFreeSpace)
	{
		addStackSizeError(&sciErr, ((StrCtx*)_pvCtx)->pstName, iMemSize);
		return sciErr;
	}

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);
	sciErr = fillBooleanSparseMatrix(_pvCtx, piAddr, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_BOOLEAN_SPARSE, _("%s: Unable to create %s named \"%s\""), "createNamedBooleanSparseMatrix", _("boolean sparse matrix"), _pstName);
		return sciErr;
	}

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));

	iPos	= 5;//4 for header + 1 for NbItem
	iPos += _iRows + _iNbItem;

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + iPos, 0);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return sciErr;
}

SciErr readNamedBooleanSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piNbItemRow	= 0;
	int* piColPos			= 0;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_BOOLEAN_SPARSE, _("%s: Unable to get variable \"%s\""), "readNamedBooleanSparseMatrix", _pstName);
		return sciErr;
	}

	sciErr = getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_BOOLEAN_SPARSE, _("API_ERROR_READ_NAMED_BOOLEAN_SPARSE"));
		return sciErr;
	}

	if(_piNbItemRow == NULL)
	{
		return sciErr;
	}

	memcpy(_piNbItemRow, piNbItemRow, *_piRows * sizeof(int));

	if(_piColPos == NULL)
	{
		return sciErr;
	}

	memcpy(_piColPos, piColPos, *_piNbItem * sizeof(int));
	return sciErr;
}
/*--------------------------------------------------------------------------*/
int isBooleanSparseType(void* _pvCtx, int* _piAddress)
{
	return checkVarType(_pvCtx, _piAddress, sci_boolean_sparse);
}
/*--------------------------------------------------------------------------*/
int isNamedBooleanSparseType(void* _pvCtx, char* _pstName)
{
	return checkNamedVarType(_pvCtx, _pstName, sci_boolean_sparse);
}
/*--------------------------------------------------------------------------*/
int getAllocatedBooleanSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

	sciErr = getBooleanSparseMatrix(_pvCtx, _piAddress, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to get argument #%d"), "getAllocatedBooleanSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_piNbItemRow		= (int*)MALLOC(sizeof(int) * *_piRows);
	memcpy(*_piNbItemRow, piNbItemRow, sizeof(int) * *_piRows);

	*_piColPos			= (int*)MALLOC(sizeof(int) * *_piNbItem);
	memcpy(*_piColPos, piColPos, sizeof(int) * *_piNbItem);

	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedAllocatedBooleanSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr;

	sciErr = readNamedBooleanSparseMatrix(_pvCtx, _pstName, _piRows, _piCols, _piNbItem, NULL, NULL);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to get argument \"%s\""), "getNamedAllocatedBooleanSparseMatrix", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_piNbItemRow		= (int*)MALLOC(sizeof(int) * *_piRows);
	*_piColPos			= (int*)MALLOC(sizeof(int) * *_piNbItem);

	sciErr = readNamedBooleanSparseMatrix(_pvCtx, _pstName, _piRows, _piCols, _piNbItem, *_piNbItemRow, *_piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_ALLOC_BOOLEAN_SPARSE, _("%s: Unable to get argument \"%s\""), "getNamedAllocatedBooleanSparseMatrix", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
void freeAllocatedBooleanSparse(int* _piNbItemRow, int* _piColPos)
{
	FREE(_piNbItemRow);
	FREE(_piColPos);
}