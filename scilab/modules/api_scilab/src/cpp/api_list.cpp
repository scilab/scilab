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
#include "CallScilab.h"

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

//internal functions
static int createCommonList(int _iVar, int _iListType, int _iNbItem, int** _piAddress);
static int createCommonListInList(int _iVar, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress, int iNamed);
static int createCommonNamedList(char* _pstName, int _iListType, int _iNbItem, int** _piAddress);
static int createCommonListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress);
static int getCommonListInList(int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static int getCommomListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress);
static int readCommonNamedList(char* _pstName, int _iListType, int* _piNbItem, int** _piAddress);
static int fillCommonList(int* _piAddress, int _iListType, int _iNbItem);
static int isKindOfList(int* _piNode);
static int getParentList(int* _piStart, int* _piToFind, int* _piDepth, int** _piParent);
static void closeList(int _iVar, int *_piEnd);
static void updateListOffset(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);
static void updateNamedListOffset(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);
static void updateCommunListOffset(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd);

static int allocCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double **_pdblReal, double **_pdblImg);
static int getCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg);
static int createCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);
static int fillCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg);
static int createCommomMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg);
static int readCommonMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg);
static int allocCommonItemInList(int* _piParent, int _iItemPos, int** _piChildAddr);
static int fillMatrixOfBoolInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool);
static int getCommonxMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static int createCommonMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg);
static int fillCommonMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen);
static int fillCommonMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen);
static int readCommonSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);
static int createCommonSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);
static int createCommonSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg);

int getListItemNumber(int* _piAddress, int* _piNbItem)
{
	switch(getVarType(_piAddress))
	{
	case sci_list :
	case sci_mlist :
	case sci_tlist :
		*_piNbItem = _piAddress[1];
		break;
	default :
		*_piNbItem = -1;
		return 1;
	}
	return 0;
}

int getListItemAddress(int* _piAddress, int _iItemNum, int** _piItemAddress)
{
	int iRet						= 0;
	int iItem						= 0;
	int* piOffset				= NULL;
	int* piItemAddress	= NULL;

	//get item count
	iRet = getListItemNumber(_piAddress, &iItem);
	if(iRet)
	{
		return 1;
	}

	//get offset of item array
	piOffset				=	 _piAddress + 2;
	piItemAddress	= piOffset + iItem  + 1 + !(iItem % 2);
	*_piItemAddress	= piItemAddress + (piOffset[_iItemNum - 1] - 1) * (sizeof(double) / sizeof(int));
	return 0;
}

int createList(int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_iVar, sci_list, _iNbItem, _piAddress);
}

int createMList(int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_iVar, sci_mlist, _iNbItem, _piAddress);
}

int createTList(int _iVar, int _iNbItem, int** _piAddress)
{
	return createCommonList(_iVar, sci_tlist, _iNbItem, _piAddress);
}

int createNamedList(char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pstName, sci_list, _iNbItem, _piAddress);
}

int createNamedTList(char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pstName, sci_tlist, _iNbItem, _piAddress);
}

int createNamedMList(char* _pstName, int _iNbItem, int** _piAddress)
{
	return createCommonNamedList(_pstName, sci_mlist, _iNbItem, _piAddress);
}

static int createCommonNamedList(char* _pstName, int _iListType, int _iNbItem, int** _piAddress)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	fillCommonList(piAddr, _iListType, _iNbItem);

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
	return 0;
}

static int createCommonList(int _iVar, int _iListType, int _iNbItem, int** _piAddress)
{
	int *piAddr			= NULL;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonList(piAddr, _iListType, _iNbItem);
	if(iRet)
	{
		return 1;
	}

	*_piAddress	= piAddr;
	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 2 + _iNbItem + 1 + !(_iNbItem % 2)));
	closeList(iNewPos, piAddr + 2 + _iNbItem + 1 + !(_iNbItem % 2));

	return 0;
}

int fillCommonList(int* _piAddress, int _iListType, int _iNbItem)
{
	int i					= 0;
	int* piOffset = NULL;

	_piAddress[0]	= _iListType;
	_piAddress[1] = _iNbItem;

	piOffset		= _piAddress + 2;
	piOffset[0]	= 1; //always

	for(i = 0 ; i < _iNbItem; i++)
	{//initialize item offset
		piOffset[i + 1] = -1;
	}
	return 0;
}

int readNamedList(char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pstName, sci_list, _piNbItem, _piAddress);
}

int readNamedTList(char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pstName, sci_tlist, _piNbItem, _piAddress);
}

int readNamedMList(char* _pstName, int* _piNbItem, int** _piAddress)
{
	return readCommonNamedList(_pstName, sci_mlist, _piNbItem, _piAddress);
}

static int readCommonNamedList(char* _pstName, int _iListType, int* _piNbItem, int** _piAddress)
{
	int* piAddr		= NULL;
	int iNbItem		= 0;
	int iRet			= 0;

	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 1;
	}

	if(piAddr[0] != _iListType)
	{
		return 1;
	}

	iRet = getListItemNumber(piAddr, &iNbItem);
	if(iRet)
	{
		return 1;
	}

	*_piNbItem = iNbItem;
	*_piAddress = piAddr;

	return 0;
}

int getListInList(int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_piParent, _iItemPos, sci_list, _piAddress);
}

int getTListInList(int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_piParent, _iItemPos, sci_tlist, _piAddress);
}

int getMListInList(int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommonListInList(_piParent, _iItemPos, sci_mlist, _piAddress);
}

int getCommonListInList(int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
	int iRet			= 0;

	iRet = getListItemAddress(_piParent, _iItemPos, _piAddress);
	if(iRet)
	{
		return 1;
	}

	if((*_piAddress)[0] != _iListType)
	{
		return 1;
	}
	return 0;
}

int getListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pstName, _piParent, _iItemPos, sci_list, _piAddress);
}

int getTListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pstName, _piParent, _iItemPos, sci_tlist, _piAddress);
}

int getMListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int** _piAddress)
{
	return getCommomListInNamedList(_pstName, _piParent, _iItemPos, sci_mlist, _piAddress);
}

int getCommomListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iListType, int** _piAddress)
{
	int* piAddr		= NULL;

	getListItemAddress(_piParent, _iItemPos, &piAddr);

	if(piAddr[0] != _iListType)
	{
		return 1;
	}

	*_piAddress = piAddr;
	return 0;
}

/*********************
 * Double functions *
 *********************/

int getMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal)
{
	return getCommonMatrixOfDoubleInList(_iVar, _piParent,_iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

int getComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfDoubleInList(_iVar, _piParent,_iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static int getCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double** _pdblReal, double** _pdblImg)
{
	int iRet			= 0;
	int* piAddr		= NULL;

	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getCommonMatrixOfDouble(piAddr, _iComplex, _piRows, _piCols, _pdblReal, _pdblImg);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

int allocMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal)
{
	return allocCommonMatrixOfDoubleInList(_iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

int allocComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	return allocCommonMatrixOfDoubleInList(_iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

static int allocCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	fillCommonMatrixOfDoubleInList(_iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);

	piEnd = (int*) (*_pdblReal + _iRows * _iCols * (_iComplex + 1));
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}

	return 0;
}

static int fillCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double** _pdblReal, double** _pdblImg)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}


	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfDouble(piChildAddr, _iComplex, _iRows, _iCols, _pdblReal, _pdblImg);
	if(iRet)
	{
		return 1;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + _iRows * _iCols * (_iComplex + 1) + 2;

	return 0;
}


int	allocCommonItemInList(int* _piParent, int _iItemPos, int** _piChildAddr)
{
	int iRet			= 0;
	int* piOffset = NULL;

	//Does previous items was already inserted
	piOffset		= _piParent + 2;
	if(piOffset[_iItemPos - 1] == -1)
	{//Previous items wasn't inserted
		return 1;
	}

	iRet = getListItemAddress(_piParent, _iItemPos, _piChildAddr);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

int createMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommonMatrixOfDoubleInList(_iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

int createComplexMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommonMatrixOfDoubleInList(_iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

int createComplexZMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	int iRet = 0;

	iRet = allocCommonMatrixOfDoubleInList(_iVar, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	return 0;
}

int createCommonMatrixOfDoubleInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	int iRet = 0;

	iRet = allocCommonMatrixOfDoubleInList(_iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}
	if(_pdblReal != NULL)
	{
		memcpy(pdblReal, _pdblReal, _iRows * _iCols * sizeof(double));
	}

	if(_iComplex && _pdblImg != NULL)
	{
		memcpy(pdblImg, _pdblImg, _iRows * _iCols * sizeof(double));
	}
	return 0;
}

int createMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal)
{
	return createCommomMatrixOfDoubleInNamedList(_pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _pdblReal, NULL);
}

int createComplexMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	return createCommomMatrixOfDoubleInNamedList(_pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _pdblReal, _pdblImg);
}

int createComplexZMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, doublecomplex* _pdblData)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfDoubleInList(Top, _piParent, _iItemPos, 1, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	vGetPointerFromDoubleComplex(_pdblData, _iRows * _iCols, pdblReal, pdblImg);

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	piEnd = piChildAddr + 4 + (_iRows * _iCols * 4);//4 -> 2*2 real + img * double
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int createCommomMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, double* _pdblReal, double* _pdblImg)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfDoubleInList(Top, _piParent, _iItemPos, _iComplex, _iRows, _iCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	memcpy(pdblReal, _pdblReal, sizeof(double) * _iRows * _iCols);
	if(_iComplex)
	{
		memcpy(pdblImg, _pdblImg, sizeof(double) * _iRows * _iCols);
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	piEnd = piChildAddr + 4 + (_iRows * _iCols * 2 * (_iComplex + 1));
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int createListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_iVar, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress, 0);
}

int createTListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_iVar, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress, 0);
}

int createMListInList(int _iVar, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInList(_iVar, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress, 0);
}

static int createCommonListInList(int _iVar, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress, int iNamed)
{
	int iRet					= 0;
	int iNewPos				= Top - Rhs + _iVar;
	int iNbItem				= 0;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonList(piChildAddr, _iListType, _iNbItem);
	if(iRet)
	{
		return 1;
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
			updateNamedListOffset(_iVar, _piParent, _iItemPos, piEnd);
		}
		else
		{
			updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
		}
	}

	return 0;
}

int createListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pstName, _piParent, _iItemPos, sci_list, _iNbItem, _piAddress);
}

int createTListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pstName, _piParent, _iItemPos, sci_tlist, _iNbItem, _piAddress);
}

int createMListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iNbItem, int** _piAddress)
{
	return createCommonListInNamedList(_pstName, _piParent, _iItemPos, sci_mlist, _iNbItem, _piAddress);
}

int createCommonListInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iListType, int _iNbItem, int** _piAddress)
{
	int iVarID[nsiz];
	int iRet = 0;
	int iSaveTop = Top;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
	Top = Top + Nbvars + 1;

	iRet = createCommonListInList(Top, _piParent, _iItemPos, _iListType, _iNbItem, _piAddress, 1);

	if(_iNbItem == 0 && _iItemPos == _piParent[1])
	{
		int* piEnd = *_piAddress + 4;
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
	return iRet;
}


int readMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal)
{
	return readCommonMatrixOfDoubleInNamedList(_pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _pdblReal, NULL);
}

int readComplexMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	return readCommonMatrixOfDoubleInNamedList(_pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _pdblReal, _pdblImg);
}

static int readCommonMatrixOfDoubleInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, double* _pdblReal, double* _pdblImg)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getCommonMatrixOfDouble(piAddr, _iComplex, _piRows, _piCols, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return 0;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piRows * *_piCols);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piRows * *_piCols);
	}
	return 0;
}


/*********************
 * Strings functions *
 *********************/

int getMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	int iRet			= 0;
	int* piAddr		= NULL;

	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getMatrixOfString(piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(iRet)
	{
		return 1;
	}
	return 0;
}


int createMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int iTotalLen			= 0;
	int iNewPos				= Top - Rhs + _iVar;

	int* piItemAddr		= NULL;
	int* piEnd				= NULL;

	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}

	iRet = getListItemAddress(_piParent, _iItemPos, &piItemAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfStringInList(_iVar, _piParent, _iItemPos, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(iRet)
	{
		return 1;
	}

	piEnd = piItemAddr + iTotalLen + 5 + _iRows * _iCols + !((iTotalLen + _iRows * _iCols) % 2);
	closeList(iNewPos, piEnd);

	if(_iItemPos == iNbItem)
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}

	return 0;
}

int fillCommonMatrixOfStringInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings, int* _piTotalLen)
{
	int iRet				= 0;
	int iNbItem			= 0;

	int* piAddr			= NULL;
	int* piOffset		= NULL;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillMatrixOfString(piAddr, _iRows, _iCols, _pstStrings, _piTotalLen);
	if(iRet)
	{
		return 1;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + (*_piTotalLen + 5 + _iRows * _iCols + !((*_piTotalLen + _iRows * _iCols) %2)) / 2;

	return 0;
}

int createMatrixOfStringInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pstStrings)
{
	int iVarID[nsiz];
	int iRet					= 0;
	int iTotalLen			= 0;
	int iSaveRhs			= Rhs;
	int iSaveTop			= Top;

	int* piItemAddr		= NULL;
	int* piEnd				= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
	Top = Top + Nbvars + 1;

	iRet = getListItemAddress(_piParent, _iItemPos, &piItemAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfStringInList(Top, _piParent, _iItemPos, _iRows, _iCols, _pstStrings, &iTotalLen);
	if(iRet)
	{
		return 1;
	}

	piEnd = piItemAddr + (iTotalLen + 5 + _iRows * _iCols + !((_iRows * _iCols) %2));
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;


	return 0;
}

int readMatrixOfStringInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	int iRet					= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;

	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getMatrixOfString(piAddr, _piRows, _piCols, _piLength, _pstStrings);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

/*********************
 * boolean functions *
 *********************/

int getMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piBool)
{
	int iRet			= 0;
	int* piAddr		= NULL;

	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getMatrixOfBoolean(piAddr, _piRows, _piCols, _piBool);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

int createMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool)
{
	int *piBool			= NULL;

	int iRet = 0;

	iRet = allocMatrixOfBooleanInList(_iVar, _piParent, _iItemPos, _iRows, _iCols, &piBool);
	if(iRet)
	{
		return 1;
	}
	if(_piBool != NULL)
	{
		memcpy(piBool, _piBool, _iRows * _iCols * sizeof(int));
	}
	return 0;
}

int allocMatrixOfBooleanInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	fillMatrixOfBoolInList(_iVar, _piParent, _iItemPos, _iRows, _iCols, _piBool);

	piEnd = *_piBool + _iRows * _iCols + !((_iRows * _iCols) % 2);
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}
	return 0;
}

static int fillMatrixOfBoolInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piBool)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}


	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillMatrixOfBoolean(piChildAddr, _iRows, _iCols, _piBool);
	if(iRet)
	{
		return 1;
	}

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((3 + _iRows * _iCols + !((_iRows * _iCols) % 2)) / 2);

	return 0;
}

int createMatrixOfBooleanInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piBool)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	int* piBool				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillMatrixOfBoolInList(Top, _piParent, _iItemPos, _iRows, _iCols, &piBool);
	if(iRet)
	{
		return 1;
	}

	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	piEnd = piChildAddr + 4 + (_iRows * _iCols) + ((_iRows * _iCols) % 2);
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int readMatrixOfBooleanInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piBool)
{
	int iRet					= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;
	int* piBool				= NULL;


	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getMatrixOfBoolean(piAddr, _piRows, _piCols, &piBool);
	if(iRet)
	{
		return 1;
	}

	if(_piBool == NULL)
	{
		return 0;
	}

	if(piBool == NULL)
	{
		return 1;
	}

	memcpy(_piBool, piBool, *_piRows * *_piCols * sizeof(int));
	return 0;
}


/*************************
 * polynomials functions *
 *************************/

int getMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return getCommonxMatrixOfPolyInList(_iVar, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

int getComplexMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonxMatrixOfPolyInList(_iVar, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

int getCommonxMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	int iRet			= 0;
	int* piAddr		= NULL;

	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getCommonMatrixOfPoly(piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

int createMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPolyInList(_iVar, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

int createComplexMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPolyInList(_iVar, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

int createCommonMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	int iRet						= 0;
	int* piItemAddr			= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;
	int iTotalLen				= 0;

	iRet = getListItemAddress(_piParent, _iItemPos, &piItemAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfPolyInList(_iVar, _piParent, _iItemPos, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(iRet)
	{
		return 1;
	}

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;
	piEnd								=	piItemAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return 0;
}

static int fillCommonMatrixOfPolyInList(int _iVar, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int iTotalLen			= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	int iItemLen			= 0;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfPoly(piChildAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(iRet)
	{
		return 1;
	}

	piOffset						= _piParent + 2;

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

	*_piTotalLen = iTotalLen;
	return 0;
}


int createMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPolyInNamedList(_pstName, _piParent, _iItemPos, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

int createComplexMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPolyInNamedList(_pstName, _piParent, _iItemPos, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

int createCommonMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;
	int iTotalLen			= 0;
	int iItemLen			= 0;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfPolyInList(Top, _piParent, _iItemPos, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(iRet)
	{
		return 1;
	}

	iItemLen						= 9 + _iRows * _iCols + (9 + _iRows * _iCols)%2;
	iItemLen						+= iTotalLen;

	getListItemAddress(_piParent, _iItemPos, &piChildAddr);
	piEnd								=	piChildAddr + iItemLen;
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}


	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int readMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return readCommonMatrixOfPolyInNamedList(_pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

int readComplexMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return readCommonMatrixOfPolyInNamedList(_pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

int readCommonMatrixOfPolyInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	int iRet					= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int iNbItem				= NULL;

	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getCommonMatrixOfPoly(piAddr, _iComplex, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	if(iRet)
	{
		return 1;
	}
	return 0;
}

/**********************
 * integers functions *
 **********************/

static int fillCommonMatrixOfIntegerInList(int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}


	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonMatrixOfInteger(piChildAddr, _iPrecision, _iRows, _iCols, _pvData);
	if(iRet)
	{
		return 1;
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
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 2 + _iRows * _iCols / (sizeof(double) / (_iPrecision % 10 )) + !!(_iRows * _iCols) % (sizeof(double) / (_iPrecision % 10 ));

	return 0;
}

static int allocCommonMatrixOfIntegerInList(int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void** _pvData)
{
	int iRet					= 0;
	int iNewPos				= Top - Rhs + _iVar;
	int* piEnd				= NULL;

	iRet = fillCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, _pvData);
	if(iRet)
	{
		return 1;
	}

	piEnd = (int*)*_pvData + _iRows * _iCols / (sizeof(int) / (_iPrecision % 10)) + !!(_iRows * _iCols) % (sizeof(int) / (_iPrecision % 10));
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}
	return 0;
}

int allocMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char** _pucData)
{
	return allocCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, (void **)_pucData);
}

int allocMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short** _pusData)
{
	return allocCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, (void**)_pusData);
}

int allocMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int** _puiData)
{
	return allocCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, (void**)_puiData);
}

int allocMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char** _pcData)
{
	return allocCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, (void**)_pcData);
}

int allocMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short** _psData)
{
	return allocCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, (void**)_psData);
}

int allocMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int** _piData)
{
	return allocCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, (void**)_piData);
}

static int createCommomMatrixOfIntegerInList(int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	void *pvData = NULL;

	int iRet = 0;

	iRet = allocCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, &pvData);
	if(iRet)
	{
		return 1;
	}
	if(pvData != NULL)
	{
		memcpy(pvData, _pvData, _iRows * _iCols * (_iPrecision % 10));
	}
	return 0;
}

int createMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData)
{
	return createCommomMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

int createMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData)
{
	return createCommomMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

int createMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData)
{
	return createCommomMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

int createMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData)
{
	return createCommomMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

int createMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData)
{
	return createCommomMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

int createMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData)
{
	return createCommomMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

static int getCommonMatrixOfIntegerInList(int _iVar, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void** _pvData)
{
	int iRet			= 0;
	int* piAddr		= NULL;
	
	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getCommonMatrixOfInteger(piAddr, _iPrecision, _piRows, _piCols, _pvData);
	if(iRet)
	{
		return 1;
	}

	return 0;
}

int getMatrixOfUnsignedInteger8InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char** _pucData)
{
	return getCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, (void**)_pucData);
}

int getMatrixOfUnsignedInteger16InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short** _pusData)
{
	return getCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, (void**)_pusData);
}

int getMatrixOfUnsignedInteger32InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int** _puiData)
{
	return getCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, (void**)_puiData);
}

int getMatrixOfInteger8InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char** _pcData)
{
	return getCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, (void**)_pcData);
}

int getMatrixOfInteger16InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short** _psData)
{
	return getCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, (void**)_psData);
}

int getMatrixOfInteger32InList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int** _piData)
{
	return getCommonMatrixOfIntegerInList(_iVar, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, (void**)_piData);
}

static int createCommonMatrixOfIntegerInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int _iRows, int _iCols, void* _pvData)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = createCommomMatrixOfIntegerInList(Top, _piParent, _iItemPos, _iPrecision, _iRows, _iCols, _pvData);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	//integer : size in int32
	//1st case, 5 * 1 int8  -> 10 5 1 1 (1,2,3,4) (5,x,x,x)						-> 6 : 4 + 5/4 + !!(5%4) -> 4 + 1 + 1 -> 6
	//2nd case, 5 * 1 int16 -> 10 5 1 2   (1,2)     (3,4)   (5,x)			-> 7 : 4 + 5/2 + !!(5%2) -> 4 + 2 + 1 -> 7
	//3th case, 5 * 1 int32 -> 10 5 1 4     1         2       3   4 5	-> 9 : 4 + 5/1 + !!(5%1) -> 4 + 5 + 0 -> 9
	piEnd = piChildAddr + 4 + _iRows * _iCols / (sizeof(int) / (_iPrecision % 10)) + !!(_iRows * _iCols) % ((sizeof(int) / (_iPrecision % 10)));
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int createMatrixOfUnsignedInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned char* _pucData)
{
	return createCommonMatrixOfIntegerInNamedList(_pstName, _piParent, _iItemPos, SCI_UINT8, _iRows, _iCols, _pucData);
}

int createMatrixOfUnsignedInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned short* _pusData)
{
	return createCommonMatrixOfIntegerInNamedList(_pstName, _piParent, _iItemPos, SCI_UINT16, _iRows, _iCols, _pusData);
}

int createMatrixOfUnsignedInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, unsigned int* _puiData)
{
	return createCommonMatrixOfIntegerInNamedList(_pstName, _piParent, _iItemPos, SCI_UINT32, _iRows, _iCols, _puiData);
}

int createMatrixOfInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, char* _pcData)
{
	return createCommonMatrixOfIntegerInNamedList(_pstName, _piParent, _iItemPos, SCI_INT8, _iRows, _iCols, _pcData);
}

int createMatrixOfInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, short* _psData)
{
	return createCommonMatrixOfIntegerInNamedList(_pstName, _piParent, _iItemPos, SCI_INT16, _iRows, _iCols, _psData);
}

int createMatrixOfInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int* _piData)
{
	return createCommonMatrixOfIntegerInNamedList(_pstName, _piParent, _iItemPos, SCI_INT32, _iRows, _iCols, _piData);
}

static int readCommonMatrixOfIntgerInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iPrecision, int* _piRows, int* _piCols, void* _pvData)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	void* pvData			= NULL;

	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getCommonMatrixOfInteger(piAddr, _iPrecision, _piRows, _piCols, &pvData);
	if(iRet)
	{
		return 1;
	}

	if(_pvData == NULL)
	{
		return 0;
	}

	memcpy(_pvData, pvData, (_iPrecision % 10 ) * *_piRows * *_piCols);
	return 0;
}

int readMatrixOfUnsignedInteger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned char* _pucData)
{
	return readCommonMatrixOfIntgerInNamedList(_pstName, _piParent, _iItemPos, SCI_UINT8, _piRows, _piCols, _pucData);
}

int readMatrixOfUnsignedInteger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned short* _pusData)
{
	return readCommonMatrixOfIntgerInNamedList(_pstName, _piParent, _iItemPos, SCI_UINT16, _piRows, _piCols, _pusData);
}

int readMatrixOfUnsignedInteger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, unsigned int* _puiData)
{
	return readCommonMatrixOfIntgerInNamedList(_pstName, _piParent, _iItemPos, SCI_UINT32, _piRows, _piCols, _puiData);
}

int readMatrixOfIntger8InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, char* _pcData)
{
	return readCommonMatrixOfIntgerInNamedList(_pstName, _piParent, _iItemPos, SCI_INT8, _piRows, _piCols, _pcData);
}

int readMatrixOfIntger16InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, short* _psData)
{
	return readCommonMatrixOfIntgerInNamedList(_pstName, _piParent, _iItemPos, SCI_INT16, _piRows, _piCols, _psData);
}

int readMatrixOfIntger32InNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piData)
{
	return readCommonMatrixOfIntgerInNamedList(_pstName, _piParent, _iItemPos, SCI_INT32, _piRows, _piCols, _piData);
}

/*********************
 * sparses functions *
 *********************/

static int fillCommonSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg, int* _piTotalSize)
{
	int iRet					= 0;
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
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonSparseMatrix(piChildAddr, _iComplex, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg, &iTotalLen);
	if(iRet)
	{
		return 1;
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
	return 0;
}

static int createCommonSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	int iRet						= 0;
	int* piAddr					= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;
	int iTotalLen				= 0;

	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillCommonSparseMatrixInList(_iVar, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg, &iTotalLen);
	if(iRet)
	{
		return 1;
	}
	
	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	iItemLen						+= iTotalLen * 2;
	piEnd								=	piAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return 0;
}

int createSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonSparseMatrixInList(_iVar, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

int createComplexSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonSparseMatrixInList(_iVar, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

int createCommonSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iItemLen			= 0;
	int iRet					= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = createCommonSparseMatrixInList(Top, _piParent, _iItemPos, _iComplex, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	iItemLen						+= _iNbItem * (_iComplex + 1) * 2;
	piEnd								=	piChildAddr + iItemLen;
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int createSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return createCommonSparseMatrixInNamedList(_pstName, _piParent, _iItemPos, 0, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

int createComplexSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return createCommonSparseMatrixInNamedList(_pstName, _piParent, _iItemPos, 1, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static int getCommonSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	int iRet			= 0;
	int* piAddr		= NULL;
	
	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getCommonSparseMatrix(piAddr, _iComplex, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
	if(iRet)
	{
		return 1;
	}

	return 0;
}

int getSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal)
{
	return getCommonSparseMatrixInList(_iVar, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

int getComplexSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos, double** _pdblReal, double** _pdblImg)
{
	return getCommonSparseMatrixInList(_iVar, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

static int readCommonSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iComplex, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getCommonSparseMatrix(piAddr, _iComplex, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);
	if(iRet)
	{
		return 1;
	}

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

	if(_pdblReal == NULL || (_iComplex && _pdblImg == NULL))
	{
		return 0;
	}

	memcpy(_pdblReal, pdblReal, sizeof(double) * *_piNbItem);
	if(_iComplex)
	{
		memcpy(_pdblImg, pdblImg, sizeof(double) * *_piNbItem);
	}

	return 0;
}

int readSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal)
{
	return readCommonSparseMatrixInNamedList(_pstName, _piParent, _iItemPos, 0, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, NULL);
}

int readComplexSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos, double* _pdblReal, double* _pdblImg)
{
	return readCommonSparseMatrixInNamedList(_pstName, _piParent, _iItemPos, 1, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos, _pdblReal, _pdblImg);
}

/*****************************
 * boolean sparses functions *
 *****************************/
static int fillBooleanSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;
	int* piChildAddr	= NULL;

	int iItemLen			= 0;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillBooleanSparseMatrix(piChildAddr, _iRows, _iCols, _iNbItem, &piNbItemRow, &piColPos);
	if(iRet)
	{
		return 1;
	}

	piOffset						= _piParent + 2;

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + ((iItemLen + 1) / 2);

	memcpy(piNbItemRow, _piNbItemRow, _iRows * sizeof(int));
	memcpy(piColPos, _piColPos, _iNbItem * sizeof(int));

	return 0;
}

int createBooleanSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	int iRet						= 0;
	int* piAddr					= NULL;
	int *piEnd					= NULL;
	int iItemLen				= 0;

	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillBooleanSparseMatrixInList(_iVar, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(iRet)
	{
		return 1;
	}
	
	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piEnd								=	piAddr + iItemLen;
	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}

	closeList(_iVar, piEnd);

	return 0;
}

int createBooleanSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int _iRows, int _iCols, int _iNbItem, int* _piNbItemRow, int* _piColPos)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iItemLen			= 0;
	int iRet					= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = createBooleanSparseMatrixInList(Top, _piParent, _iItemPos, _iRows, _iCols, _iNbItem, _piNbItemRow, _piColPos);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iItemLen						= 5 + _iRows + _iNbItem + !((_iRows + _iNbItem) % 2);
	piEnd								=	piChildAddr + iItemLen;
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}

int getBooleanSparseMatrixInList(int _iVar, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int** _piNbItemRow, int** _piColPos)
{
	int iRet			= 0;
	int* piAddr		= NULL;
	
	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getBooleanSparseMatrix(piAddr, _piRows, _piCols, _piNbItem, _piNbItemRow, _piColPos);
	if(iRet)
	{
		return 1;
	}

	return 0;
}

int readBooleanSparseMatrixInNamedList(char* _pstName, int* _piParent, int _iItemPos, int* _piRows, int* _piCols, int* _piNbItem, int* _piNbItemRow, int* _piColPos)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;
	int* piNbItemRow	= NULL;
	int* piColPos			= NULL;

	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getBooleanSparseMatrix(piAddr, _piRows, _piCols, _piNbItem, &piNbItemRow, &piColPos);
	if(iRet)
	{
		return 1;
	}

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

/*********************
 * Pointer functions *
 *********************/
int getPointerInList(int _iVar, int* _piParent, int _iItemPos, void** _pvPtr)
{
	int iRet			= 0;
	int* piAddr		= NULL;
	
	iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getPointer(piAddr, _pvPtr);
	if(iRet)
	{
		return 1;
	}

	return 0;
}

int createPointerInList(int _iVar, int* _piParent, int _iItemPos, void* _pvPtr)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piOffset			= NULL;
	int* piChildAddr	= NULL;
	void* pvPtr				= NULL;
	int* piEnd					= NULL;
	int iNewPos				= Top - Rhs + _iVar;

	//Does item can be added in the list
	getListItemNumber(_piParent, &iNbItem);
	if(iNbItem < _iItemPos)
	{
		return 1;
	}


	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = fillPointer(piChildAddr, &pvPtr);
	if(iRet)
	{
		return 1;
	}

	((double*)pvPtr)[0] = (double) ((unsigned long int) _pvPtr);

	piOffset						= _piParent + 2;
	piOffset[_iItemPos] = piOffset[_iItemPos - 1] + 3;//2 for header and 1 for data ( n * 64 bits )

	piEnd = piChildAddr + 6;//4 for header and 2 for data ( n * 32 bits )
	closeList(iNewPos, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateListOffset(_iVar, _piParent, _iItemPos, piEnd);
	}
	return 0;
}

int readPointerInNamedList(char* _pstName, int* _piParent, int _iItemPos, void** _pvPtr)
{
	int iRet					= 0;
	int iNbItem				= 0;
	int* piAddr				= NULL;
	int* piRoot				= NULL;

	if(_piParent == NULL)
	{
		iRet = readNamedList(_pstName, &iNbItem, &piRoot);
		if(iRet)
		{
			return 1;
		}

		iRet = getListItemAddress(piRoot, _iItemPos, &piAddr);
	}
	else
	{
		iRet = getListItemAddress(_piParent, _iItemPos, &piAddr);
	}

	if(iRet)
	{
		return 1;
	}

	iRet = getPointer(piAddr, _pvPtr);
	if(iRet)
	{
		return 1;
	}

	return 0;
}

int createPointerInNamedList(char* _pstName, int* _piParent, int _iItemPos, void* _pvPtr)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;
	int* piEnd				= NULL;
	int* piChildAddr	= NULL;

  C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = createPointerInList(Top, _piParent, _iItemPos, _pvPtr);
	if(iRet)
	{
		return 1;
	}

	iRet = allocCommonItemInList(_piParent, _iItemPos, &piChildAddr);
	if(iRet)
	{
		return 1;
	}

	piEnd = piChildAddr + 6;//4 for header + 2 for data
	closeList(Top, piEnd);

	if(_iItemPos == _piParent[1])
	{
		updateNamedListOffset(Top, _piParent, _iItemPos, piEnd);
		createNamedVariable(iVarID);
	}

	Top = iSaveTop;
  Rhs = iSaveRhs;

	return 0;
}



/********************
 * tools  functions *
 ********************/

static void updateNamedListOffset(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
	updateCommunListOffset(_iVar, _piCurrentNode, _iItemPos, _piEnd);
}

static void updateListOffset(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
	int iNewPos				= Top - Rhs + _iVar;
	updateCommunListOffset(iNewPos, _piCurrentNode, _iItemPos, _piEnd);
}

//internal tool functions
static void updateCommunListOffset(int _iVar, int *_piCurrentNode, int _iItemPos, int *_piEnd)
{
	//find list depth and update list offset for last item
	int i							= 0;
	int *piRoot				= istk(iadr(*Lstk(_iVar)));
	int iDepth				= 1; //we are already in a list
	int iMaxDepth			= 0; //we are already in a list
	int **piParent			= NULL;

	getParentList(piRoot, _piCurrentNode, &iDepth, NULL);
	piParent = (int**)MALLOC(sizeof(int*) * iDepth);
	iMaxDepth = iDepth;
	iDepth = 1;
	piParent[0] = piRoot;
	getParentList(piRoot, _piCurrentNode, &iDepth, piParent);
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

static int getParentList(int* _piStart, int* _piToFind, int* _piDepth, int** _piParent)
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
			getListItemAddress(_piStart, iIndex + 1, &piChild);
			if(piChild == _piToFind)
			{
				if(_piParent != NULL)
				{
					_piParent[*_piDepth - 1] = piChild;
					*_piDepth -= 1;
				}
				return 1;
			}

			iRet = getParentList(piChild, _piToFind, _piDepth, _piParent);
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
