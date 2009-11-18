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

#include "stdio.h"
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
static SciErr createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress);
static SciErr createCommonListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress, int iNamed);
static SciErr createCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int _iNbItem, int** _piAddress);
static SciErr createCommonListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress);
static SciErr getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static SciErr getCommomListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static SciErr readCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int* _piNbItem, int** _piAddress);
static SciErr fillCommonList(void* _pvCtx, int* _piAddress, int _iListType, int _iNbItem);
static int isKindOfList(int* _piNode);
static int getParentList(void* _pvCtx, int* _piStart, int* _piToFind, int* _piDepth, int** _piParent);
static void closeList(int _iVar, int *_piEnd);
static void updateListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);
static void updateNamedListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);
static void updateCommunListOffset(void* _pvCtx, int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);

static SciErr allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
static SciErr getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
static SciErr createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);
static SciErr fillCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
static SciErr createCommomMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);
static SciErr readCommonMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);
static SciErr allocCommonItemInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piChildAddr);
static SciErr fillMatrixOfBoolInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);
static SciErr getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static SciErr createCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static SciErr fillCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen);
static SciErr fillCommonMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen);
static SciErr readCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);
static SciErr createCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);
static SciErr createCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

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
SciErr getListItemNumber(void* _pvCtx, int* _piAddress, int* _piNbItem)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType	= 0;

	sciErr = getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_LIST_ITEM_NUMBER, _("%s: Unable to get item number of list"), "getListItemNumber");
		return sciErr;
	}
	switch(iType)
	{
	case sci_list :
	case sci_mlist :
	case sci_tlist :
		*_piNbItem = _piAddress[1];
		break;
	default :
		addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "getListItemNumber", _("list"));
		return sciErr;
	}
	return sciErr;
}

SciErr getListItemAddress(void* _pvCtx, int* _piAddress, int _iItemNum, int** _piItemAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iItem						= 0;
	int* piOffset				= NULL;
	int* piItemAddress	= NULL;

	//get item count
	sciErr = getListItemNumber(_pvCtx, _piAddress, &iItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ITEM_ADDRESS, _("%s: Unable to get address of item #%d in argument #%d"), "getListItemAddress", _iItemNum + 1, getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	if(_iItemNum > iItem)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ITEM_ADDRESS, _("%s: Unable to get address of item #%d in argument #%d"), "getListItemAddress", _iItemNum + 1, getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	//get offset of item array
	piOffset				=	 _piAddress + 2;
	piItemAddress	= piOffset + iItem  + 1 + !(iItem % 2);
	*_piItemAddress	= piItemAddress + (piOffset[_iItemNum - 1] - 1) * (sizeof(double) / sizeof(int));
	return sciErr;
}

SciErr createList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_pvCtx, _iVar, sci_list, _iNbItem, _piAddress);
}

SciErr createMList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_pvCtx, _iVar, sci_mlist, _iNbItem, _piAddress);
}

SciErr createTList(void* _pvCtx, int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_pvCtx, _iVar, sci_tlist, _iNbItem, _piAddress);
}

SciErr createNamedList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pvCtx, _pstName, sci_list, _iNbItem, _piAddress);
}

SciErr createNamedTList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pvCtx, _pstName, sci_tlist, _iNbItem, _piAddress);
}

SciErr createNamedMList(void* _pvCtx, char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pvCtx, _pstName, sci_mlist, _iNbItem, _piAddress);
}

static SciErr createCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int _iNbItem, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piEnd				= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);
	
	sciErr = fillCommonList(_pvCtx, piAddr, _iListType, _iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_LIST,_("%s: Unable to create %s named \"%s\""), "createNamedList", getListTypeName(_iListType), _pstName);
		return sciErr;
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
	return sciErr;
}

static SciErr createCommonList(void* _pvCtx, int _iVar, int _iListType, int _iNbItem, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piAddr			= NULL;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	getNewVarAddressFromPosition(_pvCtx, iNewPos, &piAddr);

	sciErr = fillCommonList(_pvCtx, piAddr, _iListType, _iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_LIST, _("%s: Unable to create variable in Scilab memory"), "createList");
		return sciErr;
	}

	*_piAddress	= piAddr;
	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 2 + _iNbItem + 1 + !(_iNbItem % 2)));
	closeList(iNewPos, piAddr + 2 + _iNbItem + 1 + !(_iNbItem % 2));

	return sciErr;
}

SciErr fillCommonList(void* _pvCtx, int* _piAddress, int _iListType, int _iNbItem)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piOffset = NULL;

	_piAddress[0]	= _iListType;
	_piAddress[1] = _iNbItem;

	piOffset		= _piAddress + 2;
	piOffset[0]	= 1; //always

	for(int i = 0 ; i < _iNbItem; i++)
	{//initialize item offset
		piOffset[i + 1] = -1;
	}
	return sciErr;
}

SciErr readNamedList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pvCtx, _pstName, sci_list, _piNbItem, _piAddress);
}

SciErr readNamedTList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pvCtx, _pstName, sci_tlist, _piNbItem, _piAddress);
}

SciErr readNamedMList(void* _pvCtx, char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pvCtx, _pstName, sci_mlist, _piNbItem, _piAddress);
}

static SciErr readCommonNamedList(void* _pvCtx, char* _pstName, int _iListType, int* _piNbItem, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;
	int iNbItem		= 0;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_LIST, _("%s: Unable to get variable \"%s\""), "readNamedList", _pstName);
		return sciErr;
	}

	if(piAddr[0] != _iListType)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "readNamedList", getListTypeName(_iListType));
		return sciErr;
	}

	sciErr = getListItemNumber(_pvCtx, piAddr, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_LIST, _("%s: Unable to get variable \"%s\""), "readNamedList", _pstName);
		return sciErr;
	}

	*_piNbItem = iNbItem;
	*_piAddress = piAddr;

	return sciErr;
}

SciErr getListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_list, _piAddress);
}

SciErr getTListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_tlist, _piAddress);
}

SciErr getMListInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_pvCtx, _piParent, _iItemPos, sci_mlist, _piAddress);
}

SciErr getCommonListInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, _piAddress);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_LIST_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getListInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	if((*_piAddress)[0] != _iListType)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "getListInList", getListTypeName(_iListType));
		return sciErr;
	}
	return sciErr;
}

SciErr getListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_list, _piAddress);
}

SciErr getTListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_tlist, _piAddress);
}

SciErr getMListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_mlist, _piAddress);
}

SciErr getCommomListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_LIST_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "getListInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	if(piAddr[0] != _iListType)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_LIST_TYPE, _("%s: Invalid argument type, %s excepted"), "getListInNamedList", getListTypeName(_iListType));
		return sciErr;
	}

	*_piAddress = piAddr;
	return sciErr;
}

SciErr createListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_pvCtx, _iVar, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress, 0);
}

SciErr createTListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_pvCtx, _iVar, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress, 0);
}

SciErr createMListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_pvCtx, _iVar, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress, 0);
}

static SciErr createCommonListInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress, int iNamed)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int iNbItem				= 0;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_LIST_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillCommonList(_pvCtx, piChildAddr, _iListType, _iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_LIST_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return sciErr;
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

	return sciErr;
}

SciErr createListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress);
}

SciErr createTListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress);
}

SciErr createMListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress);
}

SciErr createCommonListInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
	int iSaveTop = Top;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
	Top = Top + Nbvars + 1;

	sciErr = createCommonListInList(_pvCtx, Top, _piParent, _iItemPos, _iListType, _iNbItem, _piAddress, 1);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_LIST_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createListInList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	if(_iNbItem == 0 && _iItemPos == _piParent[1])
	{
		int* piEnd = *_piAddress + 4;
		updateNamedListOffset(_pvCtx, Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
	return sciErr;
}

SciErr	allocCommonItemInList(void* _pvCtx, int* _piParent, int _iItemPos, int** _piChildAddr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piOffset = NULL;

	//Does previous items was already inserted
	piOffset		= _piParent + 2;
	if(piOffset[_iItemPos - 1] == -1)
	{//Previous items wasn't inserted
		addErrorMessage(&sciErr, API_ERROR_NON_ORDERED_INSERTION, _("%s: Items must be inserted in order"), "allocItemInList");
		return sciErr;
	}

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, _piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocItemInList", _iItemPos + 1);
		return sciErr;
	}

	return sciErr;
}

/*********************
 * Double functions *
 *********************/

SciErr getMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal)
{
	return getCommonMatrixOfDoubleInList(_pvCtx, _piParent,_iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr getComplexMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfDoubleInList(_pvCtx, _piParent,_iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static SciErr getCommonMatrixOfDoubleInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "getMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfDoubleInList" : "getMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}
	return sciErr;
}

SciErr allocMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal)
{
	return allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr allocComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	return allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

static SciErr allocCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	sciErr = fillCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	piEnd = (int*) (*_pdblReal + _iRows * _iCols * (_iComplex + 1));
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	return sciErr;
}

static SciErr fillCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInList" : "createMatrixOfDoubleInList", _iItemPos + 1);
		return sciErr;
	}


	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = fillCommonMatrixOfDouble(_pvCtx, piChildAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_DOUBLE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "allocComplexMatrixOfDoubleInList" : "allocMatrixOfDoubleInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + _iRows * _iCols * (_iComplex + 1) + 2;

	return sciErr;
}

SciErr createMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr createComplexMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

SciErr createComplexZMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	sciErr = allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createComplexZMatrixOfDoubleInList", _iItemPos + 1);
		return sciErr;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	return sciErr;
}

SciErr createCommonMatrixOfDoubleInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	sciErr = allocCommonMatrixOfDoubleInList(_pvCtx, _iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfDoubleInList" : "createMatrixOfDoubleInList", _iItemPos + 1);
		return sciErr;
	}

	if(_pdblReal != NULL)
	{
		memcpy(pdblReal, _pdblReal, _iRows * _iCols * sizeof(double));
	}

	if(_iComplex && _pdblImg != NULL)
	{
		memcpy(pdblImg, _pdblImg, _iRows * _iCols * sizeof(double));
	}
	return sciErr;
}

SciErr createMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommomMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

SciErr createComplexMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommomMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

SciErr createComplexZMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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

	sciErr = fillCommonMatrixOfDoubleInList(_pvCtx, Top, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_ZDOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createComplexZMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_ZDOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createComplexZMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
}

SciErr createCommomMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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

	sciErr = fillCommonMatrixOfDoubleInList(_pvCtx, Top, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfDoubleInNamedList" : "createMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	memcpy(pdblReal, _pdblReal, sizeof(double) * _iRows * _iCols);
	if(_iComplex)
	{
		memcpy(pdblImg, _pdblImg, sizeof(double) * _iRows * _iCols);
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_DOUBLE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfDoubleInNamedList" : "createMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
}

SciErr readMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal)
{
	return readCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

SciErr readComplexMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	return readCommonMatrixOfDoubleInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static SciErr readCommonMatrixOfDoubleInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""),_iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""),_iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getCommonMatrixOfDouble(_pvCtx, piAddr, _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_DOUBLE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""),_iComplex ? "readComplexMatrixOfDoubleInNamedList" : "readMatrixOfDoubleInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return sciErr;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piRows * *_piCols);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piRows * *_piCols);
	}
	return sciErr;
}


/*********************
 * Strings functions *
 *********************/

SciErr getMatrixOfStringInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iRet			= 0;
	int* piAddr		= NULL;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfStringInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_STRING_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfStringInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	return sciErr;
}


SciErr createMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int iTotalLen			= 0;
	int iNewPos				= Top - Rhs + _iVar;

	int* piItemAddr		= NULL;
	int* piEnd				= NULL;

	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piItemAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillCommonMatrixOfStringInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return sciErr;
	}

	piEnd = piItemAddr + iTotalLen + 5 + _iRows * _iCols + !((iTotalLen + _iRows * _iCols) % 2);
	closeList(iNewPos, piEnd);

	if(_iItemPos == iNbItem)
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	return sciErr;
}

SciErr fillCommonMatrixOfStringInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem			= 0;

	int* piAddr			= NULL;
	int* piOffset		= NULL;

	//Does item can be added in the list
	getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfStringInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "fillMatrixOfStringInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillMatrixOfString(_pvCtx, piAddr, _iRows, _iCols, _pstStrings, _piTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_STRING_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "fillMatrixOfStringInList", _iItemPos + 1);
		return sciErr;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + (*_piTotalLen + 5 + _iRows * _iCols + !((*_piTotalLen + _iRows * _iCols) %2)) / 2;

	return sciErr;
}

SciErr createMatrixOfStringInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
	int iTotalLen			= 0;
	int iSaveRhs			= Rhs;
	int iSaveTop			= Top;

	int* piItemAddr		= NULL;
	int* piEnd				= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
	Top = Top + Nbvars + 1;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piItemAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_STRING_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = fillCommonMatrixOfStringInList(_pvCtx, Top, _piParent, _iItemPos, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_STRING_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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


	return sciErr;
}

SciErr readMatrixOfStringInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;

	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getMatrixOfString(_pvCtx, piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_STRING_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfStringInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	return sciErr;
}

/*********************
 * boolean functions *
 *********************/

SciErr getMatrixOfBooleanInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfBooleanInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, _piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfBooleanInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	return sciErr;
}

SciErr createMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int *piBool			= NULL;

	sciErr = allocMatrixOfBooleanInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInList", _iItemPos + 1);
		return sciErr;
	}

	if(_piBool != NULL)
	{
		memcpy(piBool, _piBool, _iRows * _iCols * sizeof(int));
	}
	return sciErr;
}

SciErr allocMatrixOfBooleanInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	sciErr = fillMatrixOfBoolInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, _piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocMatrixOfBooleanInList", _iItemPos + 1);
		return sciErr;
	}

	piEnd = *_piBool + _iRows * _iCols + !((_iRows * _iCols) % 2);
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}
	return sciErr;
}

static SciErr fillMatrixOfBoolInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBooleanInList", _iItemPos + 1);
		return sciErr;
	}


	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillMatrixOfBoolean(_pvCtx, piChildAddr, _iRows, _iCols, _piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_BOOLEAN_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfBoolInList", _iItemPos + 1);
		return sciErr;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((3 + _iRows * _iCols + !((_iRows * _iCols) % 2)) / 2);

	return sciErr;
}

SciErr createMatrixOfBooleanInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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
	
	sciErr = fillMatrixOfBoolInList(_pvCtx, Top, _piParent, _iItemPos, _iRows, _iCols, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
}

SciErr readMatrixOfBooleanInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;
	int* piBool				= NULL;


	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getMatrixOfBoolean(_pvCtx, piAddr, _piRows, _piCols, &piBool);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfBooleanInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	if(_piBool == NULL)
	{
		return sciErr;
	}

	memcpy(_piBool, piBool, *_piRows * *_piCols * sizeof(int));
	return sciErr;
}


/*************************
 * polynomials functions *
 *************************/

SciErr getMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr getComplexMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfPolyInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr getCommonMatrixOfPolyInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_POLY_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexMatrixOfPolyInList" : "getMatrixOfPolyInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getCommonMatrixOfPoly(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_POLY_IN_LIST, _("API_ERROR_GET_POLY_IN_LIST"));
		return sciErr;
	}

	return sciErr;
}

SciErr createMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createComplexMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piItemAddr			= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;
	int iTotalLen				= 0;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piItemAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillCommonMatrixOfPolyInList(_pvCtx, _iVar, _piParent, _iItemPos, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return sciErr;
	}

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;
	piEnd								=	piItemAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return sciErr;
}

static SciErr fillCommonMatrixOfPolyInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int iTotalLen			= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	int iItemLen			= 0;

	//Does item can be added in the list
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillCommonMatrixOfPoly(_pvCtx, piChildAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_POLY_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexMatrixOfPolyInList" : "createMatrixOfPolyInList", _iItemPos + 1);
		return sciErr;
	}

	piOffset						= _piParent + 2;

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

	*_piTotalLen = iTotalLen;
	return sciErr;
}


SciErr createMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createComplexMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createCommonMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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

	sciErr = fillCommonMatrixOfPolyInList(_pvCtx, Top, _piParent, _iItemPos, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POLY_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfPolyInNamedList" : "createMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POLY_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexMatrixOfPolyInNamedList" : "createMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
}

SciErr readMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return readCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr readComplexMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return readCommonMatrixOfPolyInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr readCommonMatrixOfPolyInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;

	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getCommonMatrixOfPoly(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_POLY_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexMatrixOfPolyInNamedList" : "readMatrixOfPolyInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	return sciErr;
}

/**********************
 * integers functions *
 **********************/

static SciErr fillCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return sciErr;
	}


	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillCommonMatrixOfInteger(_pvCtx, piChildAddr, _iPrecision, _iRows, _iCols, _pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return sciErr;
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

	return sciErr;
}

static SciErr allocCommonMatrixOfIntegerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	sciErr = fillCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, _pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_ALLOC_INT_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "allocMatrixOfIntegerInList", _iItemPos + 1);
		return sciErr;
	}

	piEnd = (int*)*_pvData + _iRows * _iCols / (sizeof(int) / (_iPrecision % 10)) + (int)(!!(_iRows * _iCols)) % (sizeof(int) / (_iPrecision % 10));
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}
	return sciErr;
}

SciErr allocMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, (void **)_pucData);
}

SciErr allocMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, (void**)_pusData);
}

SciErr allocMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, (void**)_puiData);
}

SciErr allocMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, (void**)_pcData);
}

SciErr allocMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, (void**)_psData);
}

SciErr allocMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData)
{
	return allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, (void**)_piData);
}

static SciErr createCommomMatrixOfIntegerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	void *pvData = NULL;

	sciErr = allocCommonMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, &pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT_IN_LIST,_("%s: Unable to create list item #%d in Scilab memory"), "createMatrixOfIntegerInList", _iItemPos + 1);
		return sciErr;
	}

	if(pvData != NULL)
	{
		memcpy(pvData, _pvData, _iRows * _iCols * (_iPrecision % 10));
	}
	return sciErr;
}

SciErr createMatrixOfUnsignedInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

SciErr createMatrixOfUnsignedInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

SciErr createMatrixOfUnsignedInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

SciErr createMatrixOfInteger8InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

SciErr createMatrixOfInteger16InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

SciErr createMatrixOfInteger32InList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData)
{
	return createCommomMatrixOfIntegerInList(_pvCtx, _iVar, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

static SciErr getCommonMatrixOfIntegerInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void** _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfIntegerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, _pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_INT_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getMatrixOfIntegerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	return sciErr;
}

SciErr getMatrixOfUnsignedInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, (void**)_pucData);
}

SciErr getMatrixOfUnsignedInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, (void**)_pusData);
}

SciErr getMatrixOfUnsignedInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, (void**)_puiData);
}

SciErr getMatrixOfInteger8InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, (void**)_pcData);
}

SciErr getMatrixOfInteger16InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, (void**)_psData);
}

SciErr getMatrixOfInteger32InList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData)
{
	return getCommonMatrixOfIntegerInList(_pvCtx, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, (void**)_piData);
}

static SciErr createCommonMatrixOfIntegerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	sciErr = createCommomMatrixOfIntegerInList(_pvCtx, Top, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, _pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT_IN_NAMED_LIST,_("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfIntegerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_INT_IN_NAMED_LIST,_("%s: Unable to create list item #%d in variable \"%s\""), "createMatrixOfIntegerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
}

SciErr createMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

SciErr createMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

SciErr createMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

SciErr createMatrixOfInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

SciErr createMatrixOfInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

SciErr createMatrixOfInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData)
{
	return createCommonMatrixOfIntegerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

static SciErr readCommonMatrixOfIntgerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void* _pvData)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	void* pvData			= NULL;

	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getCommonMatrixOfInteger(_pvCtx, piAddr, _iPrecision, _piRows, _piCols, &pvData);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_INT_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readMatrixOfIntgerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	if(_pvData == NULL)
	{
		return sciErr;
	}

	memcpy(_pvData, pvData, (_iPrecision % 10 ) * *_piRows * *_piCols);
	return sciErr;
}

SciErr readMatrixOfUnsignedInteger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, _pucData);
}

SciErr readMatrixOfUnsignedInteger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, _pusData);
}

SciErr readMatrixOfUnsignedInteger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, _puiData);
}

SciErr readMatrixOfIntger8InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, _pcData);
}

SciErr readMatrixOfIntger16InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, _psData);
}

SciErr readMatrixOfIntger32InNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData)
{
	return readCommonMatrixOfIntgerInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, _piData);
}

/*********************
 * sparses functions *
 *********************/

static SciErr fillCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg, int* _piTotalSize)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createSparseMatrixInNamedList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillCommonSparseMatrix(_pvCtx, piChildAddr, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return sciErr;
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
	return sciErr;
}

static SciErr createCommonSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr					= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;
	int iTotalLen				= 0;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillCommonSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), _iComplex ? "createComplexSparseMatrixInList" : "createComplexSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}
	
	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	iItemLen						+= iTotalLen * 2;
	piEnd								=	piAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return sciErr;
}

SciErr createSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr createComplexSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

SciErr createCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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

	sciErr = createCommonSparseMatrixInList(_pvCtx, Top, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexSparseMatrixInNamedList" : "createSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), _iComplex ? "createComplexSparseMatrixInNamedList" : "createSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
}

SciErr createSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr createComplexSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static SciErr getCommonSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), _iComplex ? "getComplexSparseMatrixInList" : "getSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	return sciErr;
}

SciErr getSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr getComplexSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return getCommonSparseMatrixInList(_pvCtx, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static SciErr readCommonSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST,_("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST,_("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getCommonSparseMatrix(_pvCtx, piAddr, _iComplex, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_SPARSE_IN_NAMED_LIST,_("%s: Unable to get address of item #%d in variable \"%s\""), _iComplex ? "readComplexSparseMatrixInNamedList" : "readSparseMatrixInNamedList", _iItemPos + 1, _pstName);
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

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return sciErr;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piNbItem);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piNbItem);
	}

	return sciErr;
}

SciErr readSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return readCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

SciErr readComplexSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return readCommonSparseMatrixInNamedList(_pvCtx, _pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

/*****************************
 * boolean sparses functions *
 *****************************/
static SciErr fillBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	int* piChildAddr	= NULL;

	int iItemLen			= 0;

	//Does item can be added in the list
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillBooleanSparseMatrix(_pvCtx, piChildAddr, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_FILL_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	piOffset						= _piParent + 2;

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));

	return sciErr;
}

SciErr createBooleanSparseMatrixInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr					= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;

	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillBooleanSparseMatrixInList(_pvCtx, _iVar, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createBooleanSparseMatrixInList", _iItemPos + 1);
		return sciErr;
	}
	
	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piEnd								=	piAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_pvCtx, _iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return sciErr;
}

SciErr createBooleanSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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

	sciErr = createBooleanSparseMatrixInList(_pvCtx, Top, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
}

SciErr getBooleanSparseMatrixInList(void* _pvCtx, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_BOOLEAN_SPARSE_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getBooleanSparseMatrixInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	return sciErr;
}

SciErr readBooleanSparseMatrixInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getBooleanSparseMatrix(_pvCtx, piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_BOOLEAN_SPARSE_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readBooleanSparseMatrixInNamedList", _iItemPos + 1, _pstName);
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

/*********************
 * Pointer functions *
 *********************/
SciErr getPointerInList(void* _pvCtx, int* _piParent, int _iItemPos, void** _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;
	
	sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	sciErr = getPointer(_pvCtx, piAddr, _pvPtr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_POINTER_IN_LIST, _("%s: Unable to get address of item #%d in argument #%d"), "getPointerInList", _iItemPos + 1, getRhsFromAddress(_pvCtx, _piParent));
		return sciErr;
	}

	return sciErr;
}

SciErr createPointerInList(void* _pvCtx, int _iVar, int* _piParent, int _iItemPos, void* _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;
	void* pvPtr				= NULL;
	int* piEnd					= NULL;
	int iNewPos				= Top - Rhs + _iVar;

	//Does item can be added in the list
	sciErr = getListItemNumber(_pvCtx, _piParent, &iNbItem);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return sciErr;
	}

	if(iNbItem < _iItemPos)
	{
		addErrorMessage(&sciErr, API_ERROR_ITEM_LIST_NUMBER, _("%s: Unable to create list item #%d in Scilab memory"), "createPointerInList", _iItemPos + 1);
		return sciErr;
	}


	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return sciErr;
	}

	sciErr = fillPointer(_pvCtx, piChildAddr, &pvPtr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POINTER_IN_LIST, _("%s: Unable to create list item #%d in Scilab memory"), "createListInList", _iItemPos + 1);
		return sciErr;
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
	return sciErr;
}

SciErr readPointerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;

	if(_piParent == NULL)
	{
		sciErr = readNamedList(_pvCtx, _pstName, &iNbItem, &piRoot);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
			return sciErr;
		}

		sciErr = getListItemAddress(_pvCtx, piRoot, _iItemPos, &piAddr);
	}
	else
	{
		sciErr = getListItemAddress(_pvCtx, _piParent, _iItemPos, &piAddr);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = getPointer(_pvCtx, piAddr, _pvPtr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_POINTER_IN_NAMED_LIST, _("%s: Unable to get address of item #%d in variable \"%s\""), "readPointerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	return sciErr;
}

SciErr createPointerInNamedList(void* _pvCtx, char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	sciErr = createPointerInList(_pvCtx, Top, _piParent, _iItemPos, _pvPtr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POINTER_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createPointerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
	}

	sciErr = allocCommonItemInList(_pvCtx, _piParent, _iItemPos, &piChildAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_POINTER_IN_NAMED_LIST, _("%s: Unable to create list item #%d in variable \"%s\""), "createPointerInNamedList", _iItemPos + 1, _pstName);
		return sciErr;
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

	return sciErr;
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
