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

#include "common_api.h"
#include "string_api.h"

#include "CallScilab.h"
#include "stack-c.h"
#include "code2str.h"

//local string functions
static int fillMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress, int* _piTotalLen);


/*******************************/
/*   string matrix functions   */
/*******************************/

int getMatrixOfString(int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	int i = 0;
	int *piOffset = NULL;
	int *piData		= NULL;

	if(	_piAddress == NULL || getVarType(_piAddress) != sci_strings)
	{
		return 1;
	}
	
	getVarDimension(_piAddress, _piRows, _piCols);

	if(_piLength == NULL)
	{
		return 0;
	}

	piOffset = _piAddress + 4;

	//non cummulative length
	for(i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		_piLength[i] = piOffset[i + 1] - piOffset[i];
	}

	if(_pstStrings == NULL)
	{
		return 0;
	}

	piData = piOffset + *_piRows * *_piCols + 1;

	for(i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		if(_pstStrings[i] == NULL)
		{
			return 1;
		}
		code2str(&_pstStrings[i], piData + iArraySum(_piLength, 0, i), _piLength[i]);
		_pstStrings[i][_piLength[i]] = 0;
	}
	return 0;
}

int getMatrixOfStringCumulativeLength(int* _piAddress, int* _piRows, int* _piCols, int* _piLength, char* _pstStrings)
{
	int i = 0;
	int *piOffset = NULL;
	int *piData		= NULL;

	if(	_piAddress == NULL || getVarType(_piAddress) != sci_strings)
	{
		return 1;
	}
	
	getVarDimension(_piAddress, _piRows, _piCols);

	if(_piLength == NULL)
	{
		return 0;
	}

	piOffset = _piAddress + 4;

	//cummulative length
	for(i = 0 ; i < *_piRows * *_piCols + 1; i++)
	{
		_piLength[i] = piOffset[i];
	}

	if(_pstStrings == NULL)
	{
		return 0;
	}

	piData = piOffset + *_piRows * *_piCols + 1;
	code2str(&_pstStrings, piData, iArraySum(_piLength, 0, *_piRows * *_piCols));
	return 0;
}

int createMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress)
{
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);
	int iTotalLen		= 0;

	fillMatrixOfString(iNewPos, _iRows, _iCols, _pstStrings, _piAddress, &iTotalLen);
	updateInterSCI(_iVar, '$', iAddr, iAddr + 5 + _iRows * _iCols);
	updateLstk(iNewPos, iAddr + 5 + _iRows * _iCols, iTotalLen);
	return 0;
}

static int fillMatrixOfString(int _iVar, int _iRows, int _iCols, char** _pstStrings, int** _piAddress, int* _piTotalLen)
{
	int* piOffset = NULL;
	int* piAddr		= NULL;
	int* piData		= NULL;
	int i					= 0;
	int iOffset		= 0;

	int iRet = getVarAddressFromNumber(_iVar, &piAddr);
	if(iRet != 0)
	{
		return 1;
	}

	piAddr[0]	= sci_strings;
	piAddr[1] = _iRows;
	piAddr[2] = _iCols;
	piAddr[3] = 0; //always 0

	piOffset	= piAddr + 4;
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

	*_piAddress		= piAddr;
	*_piTotalLen	= piOffset[_iRows * _iCols - 1];
	return 0;
}

int createNamedMatrixOfString(char* _pstName, int _iNameLen, int _iRows, int _iCols, char** _pstStrings)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int *piAddr				= NULL;

	int iTotalLen	= 0;

  C2F(str2name)(_pstName, iVarID, _iNameLen);
  Top = Top + Nbvars + 1;

	iRet = getVarAddressFromNumber(Top, &piAddr);


	//write matrix information
	fillMatrixOfString(Top, _iRows, _iCols, _pstStrings, &piAddr, &iTotalLen);

	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 5, iTotalLen);

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;
	return 0;
}

int readNamedMatrixOfString(char* _pstName, int _iNameLen, int* _piRows, int* _piCols, int* _piLength, char** _pstStrings)
{
	int iVarID[nsiz];
	int* piAddr				= NULL;

	//get variable id from name
	C2F(str2name)(_pstName, iVarID, _iNameLen);

	//define scope of search
  Fin = -1;
	//search variable 
  C2F(stackg)(iVarID);

	if (Err > 0 || Fin == 0)
	{
		return 1;
	}

	//No idea :(
  if ( *Infstk(Fin) == 2)
		Fin = *istk(iadr(*Lstk(Fin )) + 1 + 1);

	//get variable address
	getVarAddressFromNumber(Fin, &piAddr);
	
	return getMatrixOfString(piAddr, _piRows, _piCols, _piLength, _pstStrings);
}
/*--------------------------------------------------------------------------*/
