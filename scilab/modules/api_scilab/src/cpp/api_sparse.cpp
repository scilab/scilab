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
#include "api_internal_sparse.h"
#include "api_sparse.h"
#include "localization.h"

#include "MALLOC.h"
#include "call_scilab.h"
#include "stack-c.h"

//internal sparse functions
StrErr getSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return getCommonSparseMatrix(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr getComplexSparseMatrix(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return getCommonSparseMatrix(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

StrErr getCommonSparseMatrix(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iPos	= 0;
	int iType	= 0;

	if(	_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix");
		return strErr;
	}
	
	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if(iType != sci_sparse)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix", _("sparse matrix"));
		return strErr;
	}

	if(isVarComplex(_pvCtx, _piAddress) != _iComplex)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix");
		return strErr;
	}


	strErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_SPARSE, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexSparseMatrix" : "getSparseMatrix", getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	*_piNbItem = _piAddress[4];

	if(_piNbItemRow == NULL)
	{
		return strErr;
	}
	*_piNbItemRow = _piAddress + 5;//4 for header + 1 for NbItem

	if(_piColPos == NULL)
	{
		return strErr;
	}
	*_piColPos = *_piNbItemRow + *_piRows;

	if(_pdblReal == NULL)
	{
		return strErr;
	}

	iPos = (*_piNbItem + *_piRows) % 2 == 1 ? 0 : 1;
	*_pdblReal = (double*)(*_piColPos + *_piNbItem + iPos);
	if(_iComplex == 1 && _pdblImg != NULL)
	{
		*_pdblImg = *_pdblReal + *_piNbItem;
	}
	return strErr;
}

StrErr allocSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return allocCommonSparseMatrix(_pvCtx, _iVar, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr allocComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return allocCommonSparseMatrix(_pvCtx, _iVar, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

StrErr allocCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int	iTotalSize	= 0;
	int iOffset			= 0;
	int* piAddr			= NULL;

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = fillCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg, &iTotalSize);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_SPARSE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "allocComplexSparseMatrix" : "allocSparseMatrix");
		return strErr;
	}

	iOffset	= 5;//4 for header + 1 for NbItem
	iOffset		+= _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);

	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + iOffset));
	updateLstk(iNewPos, sadr(iadr(iAddr) + iOffset), iTotalSize);
	return strErr;
}

StrErr fillCommonSparseMatrix(void* _pvCtx, int *_piAddress, int _iComplex, int _iRows, int _iCols, int _iNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg, int* _piTotalSize)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "fillCommonSparseMatrix");
		return strErr;
	}

	_piAddress[0]		= sci_sparse;
	_piAddress[1]		= Min(_iRows, _iRows * _iCols);
	_piAddress[2]		= Min(_iCols, _iRows * _iCols);
	_piAddress[3]		= _iComplex;

	_piAddress[4]		= _iNbItem;

	*_piNbItemRow	= _piAddress + 5;//4 for header + 1 for NbItem
	*_piColPos		= *_piNbItemRow + _iRows;
	*_pdblReal		= (double*)(*_piColPos + _iNbItem + !((_iRows + _iNbItem) % 2));

	if(_iComplex == 1)
	{
		*_pdblImg			= *_pdblReal + _iNbItem;
	}

	*_piTotalSize	= _iNbItem * (_iComplex + 1);
	return strErr;
}

StrErr createSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonSparseMatrix(_pvCtx, _iVar, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr createComplexSparseMatrix(void* _pvCtx, int _iVar, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonSparseMatrix(_pvCtx, _iVar, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

StrErr createCommonSparseMatrix(void* _pvCtx, int _iVar, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	strErr = allocCommonSparseMatrix(_pvCtx, _iVar, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_SPARSE, _("%s: Unable to create variable in Scilab memory"), _iComplex ? "createComplexSparseMatrix" : "createSparseMatrix");
		return strErr;
	}

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
	memcpy(pdblReal, _pdblReal, _iNbItem  * sizeof(double));
	if(_iComplex)
	{
		memcpy(pdblImg, _pdblImg, _iNbItem  * sizeof(double));
	}
	return strErr;
}

StrErr createNamedSparseMatrix(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonNamedSparseMatrix(_pvCtx, _pstName, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr createNamedComplexSparseMatrix(void* _pvCtx, char* _pstName, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonNamedSparseMatrix(_pvCtx, _pstName, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

StrErr createCommonNamedSparseMatrix(void* _pvCtx, char* _pstName, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iTotalSize		= 0;
	int iPos					= 0;

	int* piAddr				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;


  C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = fillCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg, &iTotalSize);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_NAMED_SPARSE, _("%s: Unable to create %s named \"%s\""), _iComplex ? "createNamedComplexSparseMatrix" : "createNamedSparseMatrix", _("sparse matrix"), _pstName);
		return strErr;
	}

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));
	memcpy(pdblReal, _pdblReal, _iNbItem  * sizeof(double));
	if(_iComplex)
	{
		memcpy(pdblImg, _pdblImg, _iNbItem  * sizeof(double));
	}

	iPos	= 5;//4 for header + 1 for NbItem
	iPos += _iRows + _iNbItem;

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + iPos, iTotalSize);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;

}

StrErr readNamedSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return readCommonNamedSparseMatrix(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr readNamedComplexSparseMatrix(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return readCommonNamedSparseMatrix(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

StrErr readCommonNamedSparseMatrix(void* _pvCtx, char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piNbItemRow	= 0;
	int* piColPos			= 0;

	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_SPARSE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexSparseMatrix" : "readNamedSparseMatrix", _pstName);
		return strErr;
	}
	
	if(_iComplex == 1)
	{
		strErr = getComplexSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	}
	else
	{
		strErr = getSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_SPARSE, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexSparseMatrix" : "readNamedSparseMatrix", _pstName);
		return strErr;
	}

	if(_piNbItemRow == NULL)
	{
		return strErr;
	}

	memcpy(_piNbItemRow, piNbItemRow, *_piRows * sizeof(int));

	if(_piColPos == NULL)
	{
		return strErr;
	}

	memcpy(_piColPos, piColPos, *_piNbItem * sizeof(int));


	if(_pdblReal == NULL)
	{
		return strErr;
	}

	memcpy(_pdblReal, pdblReal, *_piNbItem  * sizeof(double));

	if(_iComplex && _pdblImg)
	{
		memcpy(_pdblImg, pdblImg, *_piNbItem  * sizeof(double));
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/

