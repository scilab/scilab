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

#include "CallScilab.h"
#include "stack-c.h"


/********************************/
/*   boolean matrix functions   */
/********************************/

int getMatrixOfBoolean(int* _piAddress, int* _piRows, int* _piCols, int** _piBool)
{
	if(	_piAddress == NULL || getVarType(_piAddress) != sci_boolean)
	{
		return 1;
	}
	
	getVarDimension(_piAddress, _piRows, _piCols);

	if(_piBool)
	{
		*_piBool = _piAddress + 3;
	}
	return 0;
}

int allocMatrixOfBoolean(int _iVar, int _iRows, int _iCols, int** _piBool)
{
	int *piAddr	= NULL;
	int iNewPos			= Top - Rhs + _iVar;
	int iAddr				= *Lstk(iNewPos);

	int iRet = getNewVarAddressFromPosition(iNewPos, &piAddr);
	if(iRet != 0)
	{
		return 1;
	}

	fillMatrixOfBoolean(piAddr, _iRows, _iCols, _piBool);

	updateInterSCI(_iVar, '$', iAddr, sadr(iadr(iAddr) + 3));
	updateLstk(iNewPos, sadr(iadr(iAddr) + 3), (_iRows * _iCols) / (sizeof(double) / sizeof(int)));

	return 0;
}

int fillMatrixOfBoolean(int* _piAddress, int _iRows, int _iCols, int** _piBool)
{
	_piAddress[0]	= sci_boolean;
	_piAddress[1] = Min(_iRows, _iRows * _iCols);
	_piAddress[2] = Min(_iCols, _iRows * _iCols);

	*_piBool		= _piAddress + 3;
	return 0;
}

int createMatrixOfBoolean(int _iVar, int _iRows, int _iCols, int* _piBool)
{
	int* piBool		= NULL;
	int iRet = allocMatrixOfBoolean(_iVar, _iRows, _iCols, &piBool);
	if(iRet)
	{
		return 1;
	}

	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);
	return 0;
}

int createNamedMatrixOfBoolean(char* _pstName, int _iRows, int _iCols, int* _piBool)
{
	int iVarID[nsiz];
  int iSaveRhs			= Rhs;
	int iSaveTop			= Top;
	int iRet					= 0;
	int* piBool				= NULL;
	int *piAddr				= NULL;

	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));
  Top = Top + Nbvars + 1;

	iRet = getNewVarAddressFromPosition(Top, &piAddr);

	//write matrix information
	fillMatrixOfBoolean(piAddr, _iRows, _iCols, &piBool);
	//copy data in stack
	memcpy(piBool, _piBool, sizeof(int) * _iRows * _iCols);

	updateLstk(Top, *Lstk(Top) + sadr(3), (_iRows * _iCols) / (sizeof(double)/sizeof(int)));

	Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	Top = iSaveTop;
  Rhs = iSaveRhs;


	return 0;
}

int readNamedMatrixOfBoolean(char* _pstName, int* _piRows, int* _piCols, int* _piBool)
{
	int* piAddr	= NULL;
	int* piBool	= NULL;
	int iRet		= 0;

	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 1;
	}

	iRet = getMatrixOfBoolean(piAddr, _piRows, _piCols, &piBool);
	if(iRet)
	{
		return 1;
	}
	
	if(_piBool)
	{
		memcpy(_piBool, piBool, sizeof(int) * *_piRows * *_piCols);
	}

	return 0;
}
/*--------------------------------------------------------------------------*/

