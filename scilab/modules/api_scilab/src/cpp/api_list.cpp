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

#include "MALLOC.h"
#include "stack-c.h"
#include "call_scilab.h"

#include "api_common.h"
#include "api_internal_common.h"
#include "api_internal_double.h"
#include "api_internal_string.h"
#include "api_internal_boolean.h"
#include "api_internal_poly.h"
#include "api_internal_int.h"
#include "api_internal_sparse.h"
#include "api_internal_boolean_sparse.h"
#include "api_internal_pointer.h"
#include "api_list.h"
#include "api_string.h"
#include "api_boolean.h"
#include "api_int.h"
#include "api_boolean_sparse.h"
#include "api_pointer.h"
#include "localization.h"

//internal functions
static StrErr createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress);
static StrErr createCommonListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress, int iNamed);
static StrErr createCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int _iNbItem, int** _piAddress);
static StrErr createCommonListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress);
static StrErr getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static StrErr getCommomListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static StrErr readCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int* _piNbItem, int** _piAddress);
static StrErr fillCommonList(void* _pvCtx, int* _piAddress, int _iListType, int _iNbItem);
static int isKindOfList(int* _piNode);
static int getParentList(void* _pvCtx, int* _piStart, int* _piToFind, int* _piDepth, int** _piParent);
static void closeList(int _iVar, int *_piEnd);
static void updateListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);
static void updateNamedListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);
static void updateCommunListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);

static StrErr allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
static StrErr getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
static StrErr createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);
static StrErr fillCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
static StrErr createCommomMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);
static StrErr readCommonMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);
static StrErr allocCommonItemInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piChildAddr);
static StrErr fillMatrixOfBoolInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);
static StrErr getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static StrErr createCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static StrErr fillCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen);
static StrErr fillCommonMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen);
static StrErr readCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);
static StrErr createCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);
static StrErr createCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

char* getListTypeName(int _iType)
{
	switch(_iType)
	{
	case sci_list :
		return "list";
		break;
	case sci_tlist :
		return "tlist";
		break;
	case sci_mlist :
		return "mlist";
		break;
	default:
		break;
	}
	return "";
}
StrErr getListItemNumber(void* _pvCtx, int* _piAddress, int* _piNbItem)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iType	= 0;

	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_LIST_ITEM_NUMBER, _("%s: Unable to get item number of list"), "getListItemNumber");
		return strErr;
	}
	switch(iType)
	{
	case sci_list :
	case sci_mlist :
	case sci_tlist :
		*_piNbItem = _piAddress[1];
		break;
	default :
		addErrorMessage(&strErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "getListItemNumber", _("list"));
		return strErr;
	}
	return strErr;
}

StrErr getListItemAddress(void* _pvCtx, int* _piAddress, int _iItemNum, int** _piItemAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iItem						= 0;
	int* piOffset				= NULL;
	int* piItemAddress	= NULL;

	//get item count
	strErr = getListItemNumber(_pvCtx, _piAddress, &iItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_ITEM_ADDRESS, _("%s: Unable to get address of item #%d in argument #%d"), "getListItemAddress", _iItemNum + 1, getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	if(_iItemNum > iItem)
	{
		addErrorMessage(&strErr, API_ERROR_GET_ITEM_ADDRESS, _("%s: Unable to get address of item #%d in argument #%d"), "getListItemAddress", _iItemNum + 1, getRhsFromAddress(_pvCtx, _piAddress));
		return strErr;
	}

	//get offset of item array
	piOffset				=	 _piAddress + 2;
	piItemAddress	= piOffset + iItem  + 1 + !(iItem % 2);
	*_piItemAddress	= piItemAddress + (piOffset[_iItemNum - 1] - 1) * (sizeof(double) / sizeof(int));
	return strErr;
}

StrErr createList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_pvCtx, _iVar, sci_list, _iNbItem, _piAddress);
}

StrErr createMList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_pvCtx, _iVar, sci_mlist, _iNbItem, _piAddress);
}

StrErr createTList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_pvCtx, _iVar, sci_tlist, _iNbItem, _piAddress);
}

StrErr createNamedList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pvCtx, _pstName, sci_list, _iNbItem, _piAddress);
}

StrErr createNamedTList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pvCtx, _pstName, sci_tlist, _iNbItem, _piAddress);
}

StrErr createNamedMList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pvCtx, _pstName, sci_mlist, _iNbItem, _piAddress);
}

static StrErr createCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int _iNbItem, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piEnd				= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);
	
	strErr = fillCommonList(_pvCtx, piAddr, _iListType, _iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_NAMED_LIST,_("%s: Unable to create %s named \"%s\""), "createNamedList", getListTypeName(_iListType), _pstName);
		return strErr;
	}

	piEnd = piAddr + 3 + _iNbItem + !(_iNbItem % 2);
	closeList(Top, piEnd);


	Rhs = 0;
	
	if(_iNbItem == 0)
	{//Add name in stack reference list
		createNamedVariable(iVarID);
	}

	Top						= iSaveTop;
  Rhs						= iSaveRhs;

	*_piAddress = piAddr;
	return strErr;
}

static StrErr createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piAddr			= NULL;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	strErr = fillCommonList(_pvCtx, piAddr, _iListType, _iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_LIST, _("%s: Unable to create variable in Scilab memory"), "createList");
		return strErr;
	}

	*_piAddress	= piAddr;
	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 2 + _iNbItem + 1 + !(_iNbItem % 2)));
	closeList(iNewPos, piAddr + 2 + _iNbItem + 1 + !(_iNbItem % 2));

	return strErr;
}

StrErr fillCommonList(void* _pvCtx, int* _piAddress, int _iListType, int _iNbItem)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piOffset = NULL;

	_piAddress[0]	= _iListType;
	_piAddress[1] = _iNbItem;

	piOffset		= _piAddress + 2;
	piOffset[0]	= 1; //always

	for(int i = 0 ; i < _iNbItem; i++)
	{//initialize item offset
		piOffset[i + 1] = -1;
	}
	return strErr;
}

StrErr readNamedList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pvCtx, _pstName, sci_list, _piNbItem, _piAddress);
}

StrErr readNamedTList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pvCtx, _pstName, sci_tlist, _piNbItem, _piAddress);
}

StrErr readNamedMList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pvCtx, _pstName, sci_mlist, _piNbItem, _piAddress);
}

static StrErr readCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int* _piNbItem, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;
	int iNbItem		= 0;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_LIST, _("%s: Unable to get variable \"%s\""), "readNamedList", _pstName);
		return strErr;
	}

	if(piAddr[0] != _iListType)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "readNamedList", getListTypeName(_iListType));
		return strErr;
	}

	strErr = getListItemNumber(_pvCtx, piAddr, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_NAMED_LIST, _("%s: Unable to get variable \"%s\""), "readNamedList", _pstName);
		return strErr;
	}

	*_piNbItem = iNbItem;
	*_piAddress = piAddr;

	return strErr;
}

StrErr getListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_list, _piAddress);
}

StrErr getTListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_tlist, _piAddress);
}

StrErr getMListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_mlist, _piAddress);
}

StrErr getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, _piAddress);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_LIST_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getListInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	if((*_piAddress)[0] != _iListType)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "getListInList", getListTypeName(_iListType));
		return strErr;
	}
	return strErr;
}

StrErr getListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_list, _piAddress);
}

StrErr getTListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_tlist, _piAddress);
}

StrErr getMListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_mlist, _piAddress);
}

StrErr getCommomListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_LIST_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "getListInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	if(piAddr[0] != _iListType)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "getListInNamedList", getListTypeName(_iListType));
		return strErr;
	}

	*_piAddress = piAddr;
	return strErr;
}

StrErr createListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_pvCtx, _iVar, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress, 0);
}

StrErr createTListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_pvCtx, _iVar, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress, 0);
}

StrErr createMListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_pvCtx, _iVar, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress, 0);
}

static StrErr createCommonListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress, int iNamed)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int iNbItem				= 0;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_LIST_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillCommonList(_pvCtx, piChildAddr, _iListType, _iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_LIST_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return strErr;
	}

	*_piAddress = piChildAddr;
	if(iNamed)
	{
		closeList(_iVar, piChildAddr + 2 + _iNbItem + 1 + !(_iNbItem % 2));
	}
	else
	{
		closeList(iNewPos, piChildAddr + 2 + _iNbItem + 1 + !(_iNbItem % 2));
	}

	if(_iNbItem == 0)
	{//for empty list
		int *piOffset				= _piParent + 2;
		int* piEnd					= piChildAddr + 4;

		piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 2;
		if(iNamed)
		{
			updateNamedListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
		}
		else
		{
			updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
		}
	}

	return strErr;
}

StrErr createListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress);
}

StrErr createTListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress);
}

StrErr createMListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress);
}

StrErr createCommonListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
	int iSaveTop = Top;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
	Top = Top + Nbvars + 1;

	strErr = createCommonListInList(_pvCtx, Top, _piParent, _iItemPos, _iListType, _iNbItem, _piAddress, 1);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_LIST_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createListInList", _iItemPos + 1, _pstName);
		return strErr;
	}

	if(_iNbItem == 0 && _iItemPos == _piParent[1])
	{
		int* piEnd = *_piAddress + 4;
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
	return strErr;
}

StrErr	allocCommonItemInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piChildAddr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piOffset = NULL;

	//Does previous items was already inserted
	piOffset		= _piParent + 2;
	if(piOffset[_iItemPos - 1] == -1)
	{//Previous items wasn't inserted
		addErrorMessage(&strErr, API_ERROR_NON_ORDERED_INSERTION, _("%s: Items must be inserted in order"), "allocItemInList");
		return strErr;
	}

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, _piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocItemInList", _iItemPos + 1);
		return strErr;
	}

	return strErr;
}

/*********************
 * Double functions *
 *********************/

StrErr getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal)
{
	return getCommonMatrixOfDoubleInList(_pvCtx, _piParent,_iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

StrErr getComplexMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfDoubleInList(_pvCtx, _piParent,_iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static StrErr getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "getMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _pdblReal, _pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "getMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}
	return strErr;
}

StrErr allocMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal)
{
	return allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

StrErr allocComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	return allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

static StrErr allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	strErr = fillCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	piEnd = (int*) (*_pdblReal + _iRows * _iCols * (_iComplex + 1));
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	return strErr;
}

static StrErr fillCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInList" : "createMatrixOfDoubleInList", _iItemPos + 1);
		return strErr;
	}


	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = fillCommonMatrixOfDouble(_pvCtx, piChildAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + _iRows * _iCols * (_iComplex + 1) + 2;

	return strErr;
}

StrErr createMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

StrErr createComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

StrErr createComplexZMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	strErr = allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_DOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createComplexZMatrixOfDoubleInList", _iItemPos + 1);
		return strErr;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	return strErr;
}

StrErr createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	strErr = allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_DOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInList" : "createMatrixOfDoubleInList", _iItemPos + 1);
		return strErr;
	}

	if(_pdblReal != NULL)
	{
		memcpy(pdblReal, _pdblReal, _iRows * _iCols * sizeof(double));
	}

	if(_iComplex && _pdblImg != NULL)
	{
		memcpy(pdblImg, _pdblImg, _iRows * _iCols * sizeof(double));
	}
	return strErr;
}

StrErr createMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommomMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

StrErr createComplexMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommomMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

StrErr createComplexZMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = fillCommonMatrixOfDoubleInList(_pvCtx, Top, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_ZDOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createComplexZMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_ZDOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createComplexZMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	piEnd = piChildAddr + 4 + (_iRows * _iCols * 4);//4 -> 2*2 real + img * double
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr createCommomMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = fillCommonMatrixOfDoubleInList(_pvCtx, Top, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_DOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfDoubleInNamedList" : "createMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	memcpy(pdblReal, _pdblReal, sizeof(double) * _iRows * _iCols);
	if(_iComplex)
	{
		memcpy(pdblImg, _pdblImg, sizeof(double) * _iRows * _iCols);
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_DOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfDoubleInNamedList" : "createMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	piEnd = piChildAddr + 4 + (_iRows * _iCols * 2 * (_iComplex + 1));
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr readMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal)
{
	return readCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

StrErr readComplexMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	return readCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static StrErr readCommonMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""),_iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""),_iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""),_iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return strErr;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piRows * *_piCols);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piRows * *_piCols);
	}
	return strErr;
}


/*********************
 * Strings functions *
 *********************/

StrErr getMatrixOfStringInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iRet			= 0;
	int* piAddr		= NULL;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfStringInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfStringInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	return strErr;
}


StrErr createMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int iTotalLen			= 0;
	int iNewPos				= Top - Rhs + _iVar;

	int* piItemAddr		= NULL;
	int* piEnd				= NULL;

	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return strErr;
	}

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piItemAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillCommonMatrixOfStringInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return strErr;
	}

	piEnd = piItemAddr + iTotalLen + 5 + _iRows * _iCols + !((iTotalLen + _iRows * _iCols) % 2);
	closeList(iNewPos, piEnd);

	if(_iItemPos == iNbItem)
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	return strErr;
}

StrErr fillCommonMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem			= 0;

	int* piAddr			= NULL;
	int* piOffset		= NULL;

	//Does item can be added in the list
	getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "fillMatrixOfStringInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, _piTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "fillMatrixOfStringInList", _iItemPos + 1);
		return strErr;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + (*_piTotalLen + 5 + _iRows * _iCols + !((*_piTotalLen + _iRows * _iCols) %2)) / 2;

	return strErr;
}

StrErr createMatrixOfStringInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
	int iTotalLen			= 0;
	int iSaveRhs			= Rhs;
	int iSaveTop			= Top;

	int* piItemAddr		= NULL;
	int* piEnd				= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
	Top = Top + Nbvars + 1;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piItemAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_STRING_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = fillCommonMatrixOfStringInList(_pvCtx, Top, _piParent, _iItemPos, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_STRING_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	piEnd = piItemAddr + (iTotalLen + 5 + _iRows * _iCols + !((_iRows * _iCols) %2));
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;


	return strErr;
}

StrErr readMatrixOfStringInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;

	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	return strErr;
}

/*********************
 * boolean functions *
 *********************/

StrErr getMatrixOfBooleanInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfBooleanInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, _piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfBooleanInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	return strErr;
}

StrErr createMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int *piBool			= NULL;

	strErr = allocMatrixOfBooleanInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, &piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInList", _iItemPos + 1);
		return strErr;
	}

	if(_piBool != NULL)
	{
		memcpy(piBool, _piBool, _iRows * _iCols * sizeof(int));
	}
	return strErr;
}

StrErr allocMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	strErr = fillMatrixOfBoolInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, _piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocMatrixOfBooleanInList", _iItemPos + 1);
		return strErr;
	}

	piEnd = *_piBool + _iRows * _iCols + !((_iRows * _iCols) % 2);
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}
	return strErr;
}

static StrErr fillMatrixOfBoolInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInList", _iItemPos + 1);
		return strErr;
	}


	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillMatrixOfBoolean(_pvCtx, piChildAddr, _iRows, _iCols, _piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
		return strErr;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((3 + _iRows * _iCols + !((_iRows * _iCols) % 2)) / 2);

	return strErr;
}

StrErr createMatrixOfBooleanInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piBool				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);
	
	strErr = fillMatrixOfBoolInList(_pvCtx, Top, _piParent, _iItemPos, _iRows, _iCols, &piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	piEnd = piChildAddr + 4 + (_iRows * _iCols) + ((_iRows * _iCols) % 2);
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr readMatrixOfBooleanInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;
	int* piBool				= NULL;


	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, &piBool);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	if(_piBool == NULL)
	{
		return strErr;
	}

	memcpy(_piBool, piBool, *_piRows * *_piCols * sizeof(int));
	return strErr;
}


/*************************
 * polynomials functions *
 *************************/

StrErr getMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

StrErr getComplexMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_POLY_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfPolyInList" : "getMatrixOfPolyInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getCommonMatrixOfPoly(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_POLY_IN_LIST, _("API_ERROR_GET_POLY_IN_LIST"));
		return strErr;
	}

	return strErr;
}

StrErr createMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

StrErr createComplexMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr createCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piItemAddr			= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;
	int iTotalLen				= 0;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piItemAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return strErr;
	}

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;
	piEnd								=	piItemAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return strErr;
}

static StrErr fillCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int iTotalLen			= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	int iItemLen			= 0;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillCommonMatrixOfPoly(_pvCtx, piChildAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return strErr;
	}

	piOffset						= _piParent + 2;

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

	*_piTotalLen = iTotalLen;
	return strErr;
}


StrErr createMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

StrErr createComplexMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr createCommonMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;
	int iTotalLen			= 0;
	int iItemLen			= 0;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = fillCommonMatrixOfPolyInList(_pvCtx, Top, _piParent, _iItemPos, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POLY_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfPolyInNamedList" : "createMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POLY_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfPolyInNamedList" : "createMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	piEnd								=	piChildAddr + iItemLen;
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}


	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr readMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return readCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

StrErr readComplexMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return readCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

StrErr readCommonMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;

	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getCommonMatrixOfPoly(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	return strErr;
}

/**********************
 * integers functions *
 **********************/

static StrErr fillCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return strErr;
	}


	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillCommonMatrixOfInteger(_pvCtx, piChildAddr, _iPrecision, _iRows, _iCols, _pvData);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return strErr;
	}

	piOffset						= _piParent + 2;

	//integer : size in double
	//1st case, 5 * 1 int8  -> (10,5) (1,1) (1,2,3,4,5,x,x,x)											-> 3 : 2 + 5/8 + !!(5%8) -> 2 + 0 + 1 -> 3
	//2nd case, 5 * 1 int16 -> (10,5) (1,2)			(1,2,3,4)				(5,x,x,x)					-> 4 : 2 + 5/4 + !!(5%4) -> 2 + 1 + 1 -> 4
	//3th case, 5 * 1 int32 -> (10,5) (1,3)				(1,2)						(3,4)			(5,x)	-> 5 : 2 + 5/2 + !!(5%2) -> 2 + 2 + 1 -> 5

	//with 5*5 int matrix
	//1st case, 5 * 5 int8  -> (10,5) (5,1) (1:25) -> 3 : 2 + 25/8 + !!(25%8) -> 2 + 3  + 1 -> 6
	//2nd case, 5 * 1 int16 -> (10,5) (5,2)	(1:25) -> 4 : 2 + 25/4 + !!(25%4) -> 2 + 6  + 1 -> 9
	//3th case, 5 * 5 int32 -> (10,5) (5,3) (1:25) -> 5 : 2 + 25/2 + !!(25%2) -> 2 + 12 + 1 -> 15
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 2 + _iRows * _iCols / (sizeof(double) / (_iPrecision % 10 )) + (int)(!!(_iRows * _iCols)) % (sizeof(double) / (_iPrecision % 10 ));

	return strErr;
}

static StrErr allocCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	strErr = fillCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, _pvData);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_ALLOC_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocMatrixOfIntegerInList", _iItemPos + 1);
		return strErr;
	}

	piEnd = (int*)*_pvData + _iRows * _iCols / (sizeof(int) / (_iPrecision % 10)) + (int)(!!(_iRows * _iCols)) % (sizeof(int) / (_iPrecision % 10));
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}
	return strErr;
}

StrErr allocMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, (void **)_pucData);
}

StrErr allocMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, (void**)_pusData);
}

StrErr allocMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, (void**)_puiData);
}

StrErr allocMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, (void**)_pcData);
}

StrErr allocMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, (void**)_psData);
}

StrErr allocMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, (void**)_piData);
}

static StrErr createCommomMatrixOfIntegerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	void *pvData = NULL;

	strErr = allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, &pvData);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT_IN_LIST,_("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return strErr;
	}

	if(pvData != NULL)
	{
		memcpy(pvData, _pvData, _iRows * _iCols * (_iPrecision % 10));
	}
	return strErr;
}

StrErr createMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

StrErr createMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

StrErr createMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

StrErr createMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

StrErr createMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

StrErr createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

static StrErr getCommonMatrixOfIntegerInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void** _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfIntegerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, _pvData);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfIntegerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	return strErr;
}

StrErr getMatrixOfUnsignedInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, (void**)_pucData);
}

StrErr getMatrixOfUnsignedInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, (void**)_pusData);
}

StrErr getMatrixOfUnsignedInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, (void**)_puiData);
}

StrErr getMatrixOfInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, (void**)_pcData);
}

StrErr getMatrixOfInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, (void**)_psData);
}

StrErr getMatrixOfInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, (void**)_piData);
}

static StrErr createCommonMatrixOfIntegerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = createCommomMatrixOfIntegerInList(_pvCtx, Top, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, _pvData);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT_IN_NAMED_LIST,_("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfIntegerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_INT_IN_NAMED_LIST,_("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfIntegerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	//integer : size in int32
	//1st case, 5 * 1 int8  -> 10 5 1 1 (1,2,3,4) (5,x,x,x)						-> 6 : 4 + 5/4 + !!(5%4) -> 4 + 1 + 1 -> 6
	//2nd case, 5 * 1 int16 -> 10 5 1 2   (1,2)     (3,4)   (5,x)			-> 7 : 4 + 5/2 + !!(5%2) -> 4 + 2 + 1 -> 7
	//3th case, 5 * 1 int32 -> 10 5 1 4     1         2       3   4 5	-> 9 : 4 + 5/1 + !!(5%1) -> 4 + 5 + 0 -> 9
	piEnd = piChildAddr + 4 + _iRows * _iCols / (sizeof(int) / (_iPrecision % 10)) + (int)(!!(_iRows * _iCols)) % ((sizeof(int) / (_iPrecision % 10)));
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr createMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

StrErr createMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

StrErr createMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

StrErr createMatrixOfInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

StrErr createMatrixOfInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

StrErr createMatrixOfInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

static StrErr readCommonMatrixOfIntgerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void* _pvData)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	void* pvData			= NULL;

	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, &pvData);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	if(_pvData == NULL)
	{
		return strErr;
	}

	memcpy(_pvData, pvData, (_iPrecision % 10 ) * *_piRows * *_piCols);
	return strErr;
}

StrErr readMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, _pucData);
}

StrErr readMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, _pusData);
}

StrErr readMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, _puiData);
}

StrErr readMatrixOfIntger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, _pcData);
}

StrErr readMatrixOfIntger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, _psData);
}

StrErr readMatrixOfIntger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, _piData);
}

/*********************
 * sparses functions *
 *********************/

static StrErr fillCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg, int* _piTotalSize)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int iTotalLen			= 0;
	int* piOffset			= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	int* piChildAddr	= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	int iItemLen			= 0;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createSparseMatrixInNamedList", _iItemPos + 1);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillCommonSparseMatrix(_pvCtx, piChildAddr, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	piOffset						= _piParent + 2;

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	iItemLen						+= iTotalLen * 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));

	memcpy(pdblReal, _pdblReal, _iNbItem * sizeof(double));
	if(_iComplex)
	{
		memcpy(pdblImg, _pdblImg, _iNbItem * sizeof(double));
	}

	*_piTotalSize = iTotalLen;
	return strErr;
}

static StrErr createCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr					= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;
	int iTotalLen				= 0;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillCommonSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg, &iTotalLen);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}
	
	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	iItemLen						+= iTotalLen * 2;
	piEnd								=	piAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return strErr;
}

StrErr createSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr createComplexSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

StrErr createCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iItemLen			= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = createCommonSparseMatrixInList(_pvCtx, Top, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexSparseMatrixInNamedList" : "createSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexSparseMatrixInNamedList" : "createSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	iItemLen						+= _iNbItem * (_iComplex + 1) * 2;
	piEnd								=	piChildAddr + iItemLen;
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr createSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr createComplexSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static StrErr getCommonSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	return strErr;
}

StrErr getSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr getComplexSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static StrErr readCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST,_("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST,_("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST,_("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
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

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return strErr;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piNbItem);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piNbItem);
	}

	return strErr;
}

StrErr readSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return readCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

StrErr readComplexSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return readCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

/*****************************
 * boolean sparses functions *
 *****************************/
static StrErr fillBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	int* piChildAddr	= NULL;

	int iItemLen			= 0;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillBooleanSparseMatrix(_pvCtx, piChildAddr, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	piOffset						= _piParent + 2;

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));

	return strErr;
}

StrErr createBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr					= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;

	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillBooleanSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return strErr;
	}
	
	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piEnd								=	piAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return strErr;
}

StrErr createBooleanSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iItemLen			= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = createBooleanSparseMatrixInList(_pvCtx, Top, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piEnd								=	piChildAddr + iItemLen;
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}

StrErr getBooleanSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	return strErr;
}

StrErr readBooleanSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
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

	return strErr;
}

/*********************
 * Pointer functions *
 *********************/
StrErr getPointerInList(void* _pvCtx, int* _piParent, int _iItemPos, void** _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	strErr = getPointer(_pvCtx, piAddr, _pvPtr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return strErr;
	}

	return strErr;
}

StrErr createPointerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, void* _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;
	void* pvPtr				= NULL;
	int* piEnd					= NULL;
	int iNewPos				= Top - Rhs + _iVar;

	//Does item can be added in the list
	strErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return strErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&strErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createPointerInList", _iItemPos + 1);
		return strErr;
	}


	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return strErr;
	}

	strErr = fillPointer(_pvCtx, piChildAddr, &pvPtr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return strErr;
	}

	((double*)pvPtr)[0] = (double) ((unsigned long int) _pvPtr);

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 3;//2 for header and 1 for data ( n * 64 bits )

	piEnd = piChildAddr + 6;//4 for header and 2 for data ( n * 32 bits )
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}
	return strErr;
}

StrErr readPointerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;

	if(_piParent == NULL)
	{
		strErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
			return strErr;
		}

		strErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		strErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = getPointer(_pvCtx, piAddr, _pvPtr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	return strErr;
}

StrErr createPointerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	strErr = createPointerInList(_pvCtx, Top, _piParent, _iItemPos, _pvPtr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POINTER_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createPointerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	strErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_CREATE_POINTER_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createPointerInNamedList", _iItemPos + 1, _pstName);
		return strErr;
	}

	piEnd = piChildAddr + 6;//4 for header + 2 for data
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return strErr;
}



/********************
 * tools  functions *
 ********************/

static void updateNamedListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
	updateCommunListOffset(_pvCtx, _iVar, _piCurrentNode, _iItemPos, _piEnd);
}

static void updateListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
	int iNewPos				= Top - Rhs + _iVar;
	updateCommunListOffset(_pvCtx, iNewPos, _piCurrentNode, _iItemPos, _piEnd);
}

//internal tool functions
static void updateCommunListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
	//find list depth and update list offset for last item
	int i							= 0;
	int *piRoot				= istk(iadr(*Lstk(_iVar)));
	int iDepth				= 1; //we are already in a list
	int iMaxDepth			= 0; //we are already in a list
	int **piParent			= NULL;

	getParentList(_pvCtx, piRoot, _piCurrentNode, &iDepth, NULL);
	piParent = (int**)MALLOC(sizeof(int*) * iDepth);
	iMaxDepth = iDepth;
	iDepth = 1;
	piParent[0] = piRoot;
	getParentList(_pvCtx, piRoot, _piCurrentNode, &iDepth, piParent);
	for(i = iMaxDepth - 2 ; i >= 0 ; i--)
	{
		int j					=	0;
		int iItem			= piParent[i][1];
		int *piOffset = piParent[i] + 2;
		int *piData		= piOffset + iItem + 1 + !(iItem % 2);

		//for all nodes
		for(j = 0 ; j < iItem ; j++)
		{
			int* piItem = piData + ((piOffset[j] - 1) * 2);

			if(piItem == piParent[i + 1])
			{
				int iOffset = 0;
				iOffset		= piOffset[j] + (int)((_piEnd - piItem + 1) / 2);
				piOffset[j + 1] = iOffset;
			}
			//else
			//{
			//	break;
			//	//if this item is not the last of the parent list
			//	//we don't need to continue to check the uppers levels
			//}
		}
	}

	FREE(piParent);
}

static void closeList(int _iVar, int *_piEnd)
{
	//Get Root address;
	int *piRoot				= istk(iadr(*Lstk(_iVar)));
	int iAddr					= *Lstk(_iVar);

	int iOffsetData		=	2 + piRoot[1] + 1 + !(piRoot[1] % 2);
	int iScale				= (int)(_piEnd - (piRoot + iOffsetData));
	int iDoubleSclale = (iScale + 1)/ 2;

	updateLstk(_iVar, sadr(iadr(iAddr) + iOffsetData), iDoubleSclale);
}

static int getParentList(void* _pvCtx, int* _piStart, int* _piToFind, int* _piDepth, int** _piParent)
{
	if(isKindOfList(_piStart))
	{
		int iItem	= _piStart[1];
		int iIndex	= 0;
		*_piDepth += 1;
		for(iIndex = 0 ; iIndex < iItem ; iIndex++)
		{
			int *piChild = NULL;
			int iRet = 0;
			getListItemAddress(_pvCtx, _piStart, iIndex + 1, &piChild);
			if(piChild == _piToFind)
			{
				if(_piParent != NULL)
				{
					_piParent[*_piDepth - 1] = piChild;
					*_piDepth -= 1;
				}
				return 1;
			}

			iRet = getParentList(_pvCtx, piChild, _piToFind, _piDepth, _piParent);
			if(iRet != 0)
			{//find a child
				if(_piParent != NULL)
				{
					_piParent[*_piDepth - 1] = piChild;
					*_piDepth -= 1;
				}
				return 1;
			}

		}
		*_piDepth -= 1;
	}
	return 0;
}

static int isKindOfList(int* _piNode)
{
	if(_piNode[0] == sci_list || _piNode[0] == sci_tlist || _piNode[0] == sci_mlist)
		return 1;
	else
		return 0;
}

/*--------------------------------------------------------------------------*/
