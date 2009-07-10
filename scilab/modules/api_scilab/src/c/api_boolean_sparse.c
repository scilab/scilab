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


#include "CallScilab.h"
#include "stack-c.h"


int getBooleanSparseMatrix(int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	if(	_piAddress == NULL || getVarType(_piAddress) != sci_boolean_sparse)
	{
		return 1;
	}

	getVarDimension(_piAddress, _piRows, _piCols);

	*_piNbItem = _piAddress[4];

	if(_piNbItemRow == NULL)
	{
		return 0;
	}
	*_piNbItemRow = _piAddress + 5;//4 for header + 1 for NbItem

	if(_piColPos == NULL)
	{
		return 0;
	}
	*_piColPos = *_piNbItemRow + *_piRows;

	return 0;
}

int allocBooleanSparseMatrix(int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos)
{
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int iPos				= 0;
	int* piAddr			= NULL;

	getNewVarAddressFromPosition(iNewPos, &piAddr);
	fillBooleanSparseMatrix(piAddr, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);

	iPos	= iAddr + 5;//4 for header + 1 for NbItem
	iPos += _iRows + _iNbItem;

	updateInterSCI(_iVar, '$', iAddr, iPos);
	updateLstk(iNewPos, iPos, 0);
	return 0;
}

int fillBooleanSparseMatrix(int *_piAddress, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos)
{
	if(_piAddress == NULL)
	{
		return 1;
	}

	_piAddress[0]		= sci_boolean_sparse;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= 0;

	_piAddress[4]		= _iNbItem;

	*_piNbItemRow	= _piAddress + 5;//4 for header + 1 for NbItem
	*_piColPos		= *_piNbItemRow + _iRows;
	return 0;
}

int createBooleanSparseMatrix(int _iVar, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

	int iRet = allocBooleanSparseMatrix(_iVar, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
	if(iRet)
	{
		return 1;
	}

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
	return 0;
}

int createNamedBooleanSparseMatrix(char* _pstName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int iPos					= 0;

	int* piAddr				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

  C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);

	fillBooleanSparseMatrix(piAddr, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);

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

	return 0;
}

int readNamedBooleanSparseMatrix(char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos)
{
	int* piAddr				= NULL;
	int* piNbItemRow	= 0;
	int* piColPos			= 0;

	getVarAddressFromName(_pstName, &piAddr);
	getBooleanSparseMatrix(piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);

	if(_piNbItemRow == NULL)
	{
		return 0;
	}

	memcpy(_piNbItemRow, piNbItemRow, *_piRows * sizeof(int));

	if(_piColPos == NULL)
	{
		return 0;
	}
	memcpy(_piColPos, piColPos, *_piNbItem * sizeof(int));
	return 0;
}
/*--------------------------------------------------------------------------*/
