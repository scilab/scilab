/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - INRIA - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 * Please note that piece of code will be rewrited for the Scilab 6 family
 */

#include <string.h>
#include "stack-c.h"

#include "common_api.h"

int getVarDimension(int* _piAddress, int* _piRows, int* _piCols)
{
	if(_piAddress != NULL && isVarMatrixType(_piAddress))
	{
		*_piRows		= _piAddress[1];
		*_piCols		= _piAddress[2];
		return 0;
	}
	else
	{
		*_piRows		= 0;
		*_piCols		= 0;
		return 1;
	}
}

int getVarAddressFromNumber(int _iVar, int** _piAddress)
{
	int iAddr = iadr(*Lstk(_iVar));
	*_piAddress = istk(iAddr);
	return 0;
}

int getVarType(int* _piAddress)
{
	if(_piAddress == NULL)
	{
		return 0;
	}
	return _piAddress[0];
}

int isVarComplex(int* _piAddress)
{
	int iType			= 0;
	int iComplex	= 0;

	if(_piAddress == NULL)
	{
		return 0;
	}

	iType = getVarType(_piAddress);
	switch(iType)
	{
	case sci_matrix :
	case sci_poly :
	case sci_sparse :
		iComplex = _piAddress[3];
		break;
	default:
		iComplex = 0;
	}
	return iComplex;
}

void createNamedVariable(int *_piVarID)
{
	int iZero				= 0;
	//it seems this part setting up the output format but "stackp" print anything
//	int iSaveLct		= C2F(iop).lct[3];
//  C2F(iop).lct[3] = -1;
  C2F(stackp)(_piVarID, &iZero);
//  C2F(iop).lct[3] = iSaveLct;
}

int updateInterSCI(int _iVar, char _cType, int _iSCIAddress, int _iSCIDataAddress)
{

	intersci_.ntypes[_iVar - 1]	= _cType;
	intersci_.iwhere[_iVar - 1]	= _iSCIAddress;
	intersci_.lad[_iVar - 1]		= _iSCIDataAddress;
	return 0;
}

int updateLstk(int _iNewpos, int _iSCIDataAddress, int _iVarSize)
{
	*Lstk(_iNewpos + 1) = _iSCIDataAddress + _iVarSize;
	return 0;
}

int isVarMatrixType(int* _piAddress)
{
	if(_piAddress != NULL)
	{
		int iType = getVarType(_piAddress);
		
		if(	iType == sci_matrix ||
				iType == sci_poly ||
				iType == sci_boolean ||
				iType == sci_sparse ||
				iType == sci_boolean_sparse ||
				iType == sci_matlab_sparse ||
				iType == sci_ints ||
				iType == sci_handles ||
				iType == sci_strings)
				return 1;
		else
		{
			return 0;
		}
	}
	return 1;
}
/*--------------------------------------------------------------------------*/
