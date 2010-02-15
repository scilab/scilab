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
#include "api_internal_poly.h"
#include "api_internal_common.h"
#include "stack-c.h"
#include "api_oldstack.h"
#include "localization.h"
#include "MALLOC.h"
#include "context.hxx"

using namespace std;
using namespace types;

SciErr getPolyVariableName(void* _pvCtx, int* _piAddress, char* _pstVarName, int* _piVarNameLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	char *pstVarName	= NULL;

	if(_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getPolyVariableName");
		return sciErr;
	}

	if(!((InternalType*)_piAddress)->isPoly())
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), "getPolyVariableName", _("polynomial matrix"));
		return sciErr;
	}

	if(_pstVarName == NULL || *_piVarNameLen == 0)
	{
		*_piVarNameLen = (int)((InternalType*)_piAddress)->getAsPoly()->var_get().size();
		return sciErr; //No error
	}

	strcpy(_pstVarName, ((InternalType*)_piAddress)->getAsPoly()->var_get().c_str());
	return sciErr;
}

SciErr getMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return getCommonMatrixOfPoly(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr getComplexMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonMatrixOfPoly(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr getCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType					= 0;
	int iSize					= 0;
	int *piOffset			= NULL;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	if(_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly");
		return sciErr;
	}

	sciErr = getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}
	
	if(iType != sci_poly)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_TYPE, _("%s: Invalid argument type, %s excepted"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", _("polynomial matrix"));
		return sciErr;
	}

	if(isVarComplex(_pvCtx, _piAddress) != _iComplex)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_COMPLEXITY, _("%s: Bad call to get a non complex matrix"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly");
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, _piAddress, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getComplexMatrixOfPoly" : "getMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
		return sciErr;
	}

	iSize	= *_piRows * *_piCols;

	if(_piNbCoef == NULL)
	{
		return sciErr;
	}

	MatrixPoly *pMP = ((InternalType*)_piAddress)->getAsPoly();
	pMP->rank_get(_piNbCoef);

	if(_pdblReal == NULL)
	{
		return sciErr;
	}

	for(int i = 0 ; i < iSize ; i++)
	{
		memcpy(_pdblReal[i], pMP->poly_get(i)->coef_real_get(), sizeof(double) * pMP->poly_get(i)->rank_get());
	}

	if(_iComplex == 1)
	{
		for(int i = 0 ; i < iSize ; i++)
		{
			memcpy(_pdblImg[i], pMP->poly_get(i)->coef_img_get(), sizeof(double) * _piNbCoef[i]);
		}
	}
	return sciErr;
}

SciErr createMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonMatrixOfPoly(_pvCtx, _iVar, 0, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createComplexMatrixOfPoly(void* _pvCtx, int _iVar, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonMatrixOfPoly(_pvCtx, _iVar, 1, _pstVarName, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createCommonMatrixOfPoly(void* _pvCtx, int _iVar, int _iComplex, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	if(_pvCtx == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), _iComplex ? "createComplexMatrixOfPoly" : "createMatrixOfPoly");
		return sciErr;
	}

	GatewayStruct* pStr = (GatewayStruct*)_pvCtx;
  InternalType** out = pStr->m_pOut;

	MatrixPoly* pP = new MatrixPoly(_pstVarName, _iRows, _iCols, _piNbCoef);
	if(pP == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_NO_MORE_MEMORY, _("%s: No more memory to allocated variable"), _iComplex ? "createComplexMatrixOfPoly" : "createMatrixOfPoly");
		return sciErr;
	}

	if(_iComplex)
	{
		pP->complex_set(true);
	}

	int rhs = _iVar - api_Rhs((int*)_pvCtx);
	out[rhs - 1] = pP;

	for(int i = 0 ; i < pP->size_get() ; i++)
	{
		Double* pD = new Double(_piNbCoef[i], 1, _iComplex == 1);
		pD->real_set(_pdblReal[i]);
		if(_iComplex)
		{
			pD->img_set(_pdblImg[i]);
		}
		pP->poly_set(i, pD);
	}

	return sciErr;
}

SciErr fillCommonMatrixOfPoly(void* _pvCtx, int* _piAddress, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg, int* _piTotalLen)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piOffset			= NULL;
	int *piVarName		= NULL;
	int iSize					= _iRows * _iCols;

	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	//header
	_piAddress[0] = sci_poly;
	_piAddress[1] = _iRows;
	_piAddress[2] = _iCols;
	_piAddress[3] = _iComplex;


	//4 for header
	piVarName = _piAddress + 4;//4 for header
	if(strlen(_pstVarName) > 4)//4 characters max
	{
		addErrorMessage(&sciErr, API_ERROR_TOO_LONG_VAR, _("%s: Formal variable name of polynomial can't exceed 4 characters"));
		return sciErr;
	}

	//fill variable name with space ( 40 sergescii )
	piVarName[0] = 40;
	piVarName[1] = 40;
	piVarName[2] = 40;
	piVarName[3] = 40;
	//str2code(piVarName, &_pstVarName);

	piOffset = _piAddress + 8; //4 for header and 4 for variable name
	piOffset[0] = 1;
	for(int i = 0 ; i < iSize ; i++)
	{
		piOffset[i + 1] = piOffset[i] + _piNbCoef[i];
	}

	pdblReal = (double*)(piOffset + iSize + 1 + ((iSize + 1) % 2 == 0 ? 0 : 1 ));

	for(int i = 0 ; i < iSize ; i++)
	{
		memcpy(pdblReal + piOffset[i] - 1, _pdblReal[i], _piNbCoef[i] * sizeof(double));
	}

	if(_iComplex == 1)
	{
		pdblImg = pdblReal + piOffset[iSize] - 1;
		for(int i = 0 ; i < iSize ; i++)
		{
			memcpy(pdblImg + piOffset[i] - 1, _pdblImg[i], _piNbCoef[i] * sizeof(double));
		}
	}

	*_piTotalLen = (piOffset[iSize] - 1) * (_iComplex + 1) * 2;
	return sciErr;
}

SciErr createNamedMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	return createCommonNamedMatrixOfPoly(_pvCtx, _pstName, _pstVarName, 0, _iRows, _iCols, _piNbCoef, _pdblReal, NULL);
}

SciErr createNamedComplexMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return createCommonNamedMatrixOfPoly(_pvCtx, _pstName, _pstVarName, 1, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr createCommonNamedMatrixOfPoly(void* _pvCtx, char* _pstName, char* _pstVarName, int _iComplex, int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
  int iSaveRhs			= api_Rhs((int*)_pvCtx);
	int iSaveTop			= api_Top((int*)_pvCtx);
	int *piAddr				= NULL;
	int iTotalLen			= 0;

	C2F(str2name)(_pstName, iVarID, (unsigned long)strlen(_pstName));
  Top = Top + Nbvars + 1;

	getNewVarAddressFromPosition(_pvCtx, Top, &piAddr);

	//write matrix information
	sciErr = fillCommonMatrixOfPoly(_pvCtx, piAddr, _pstVarName, _iComplex, _iRows, _iCols, _piNbCoef, _pdblReal, _pdblImg, &iTotalLen);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_CREATE_NAMED_POLY, _("%s: Unable to create %s named \"%s\""), _iComplex ? "createNamedComplexMatrixOfPoly" : "createNamedMatrixOfPoly", _("matrix of double"), _pstName);
		return sciErr;
	}


	//update "variable index"
	updateLstk(Top, *Lstk(Top) + 4, iTotalLen);

	//Rhs = 0;
	//Add name in stack reference list
	createNamedVariable(iVarID);

	//Top = iSaveTop;
  //Rhs = iSaveRhs;

	return sciErr;
}

SciErr readNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal)
{
	return readCommonNamedMatrixOfPoly(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}

SciErr readNamedComplexMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return readCommonNamedMatrixOfPoly(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}

SciErr readCommonNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int _iComplex, int* _piRows, int* _piCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_POLY, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfPoly" :"readNamedMatrixOfPoly", _pstName);
		return sciErr;
	}
	
	if(_iComplex == 1)
	{
		sciErr = getComplexMatrixOfPoly(_pvCtx, piAddr, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
	}
	else
	{
		sciErr = getMatrixOfPoly(_pvCtx, piAddr, _piRows, _piCols, _piNbCoef, _pdblReal);
	}

	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_READ_NAMED_POLY, _("%s: Unable to get variable \"%s\""), _iComplex ? "readNamedComplexMatrixOfPoly" :"readNamedMatrixOfPoly", _pstName);
		return sciErr;
	}

	return sciErr;
}

/*shortcut functions */

/*--------------------------------------------------------------------------*/
int isPolyType(void* _pvCtx, int* _piAddress)
{
	return checkVarType(_pvCtx, _piAddress, sci_poly);
}
/*--------------------------------------------------------------------------*/
int isNamedPolyType(void* _pvCtx, char* _pstName)
{
	return checkNamedVarType(_pvCtx, _pstName, sci_poly);
}
/*--------------------------------------------------------------------------*/
int getAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal)
{
	return getCommonAllocatedSinglePoly(_pvCtx, _piAddress, 0, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedSingleComplexPoly(void* _pvCtx, int* _piAddress, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonAllocatedSinglePoly(_pvCtx, _piAddress, 1, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedSinglePoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	double* pdblReal = NULL;
	double* pdblImg	 = NULL;

	if(isScalar(_pvCtx, _piAddress) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Wrong type for input argument #%d: A scalar expected.\n"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, _piNbCoef, NULL, NULL);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pdblReal = (double*)MALLOC(sizeof(double) * *_piNbCoef);

	if(_iComplex)
	{
		*_pdblImg	= (double*)MALLOC(sizeof(double) * *_piNbCoef);
	}

	sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, &iRows, &iCols, _piNbCoef, _pdblReal, _pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_SINGLE_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedSinglePoly(void* _pvCtx, char* _pstName, int* _piNbCoef, double** _pdblReal)
{
	return getCommonAllocatedNamedSinglePoly(_pvCtx, _pstName, 0, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedSingleComplexPoly(void* _pvCtx, char* _pstName, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	return getCommonAllocatedNamedSinglePoly(_pvCtx, _pstName, 1, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedNamedSinglePoly(void* _pvCtx, char* _pstName, int _iComplex, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	double* pdblReal = NULL;
	double* pdblImg	 = NULL;

	if(isNamedScalar(_pvCtx, _pstName) == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_POLY, _("%s: Wrong type for input argument \"%s\": A scalar expected.\n"), _iComplex ? "getAllocatedNamedSingleComplexPoly" : "getAllocatedNamedSinglePoly", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, &iRows, &iCols, _piNbCoef, &pdblReal, &pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_SINGLE_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedSingleComplexPoly" : "getAllocatedNamedSinglePoly", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pdblReal = (double*)MALLOC(sizeof(double) * *_piNbCoef);
	memcpy(*_pdblReal, pdblReal, sizeof(double) * *_piNbCoef);

	if(_iComplex)
	{
		*_pdblImg	= (double*)MALLOC(sizeof(double) * *_piNbCoef);
		memcpy(*_pdblImg, pdblImg, sizeof(double) * *_piNbCoef);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal)
{
	return getCommonAllocatedMatrixOfPoly(_pvCtx, _piAddress, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedMatrixOfComplexPoly(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
	return getCommonAllocatedMatrixOfPoly(_pvCtx, _piAddress, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedMatrixOfPoly(void* _pvCtx, int* _piAddress, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
	SciErr sciErr;

	double* pdblReal	= NULL;
	double* pdblImg		= NULL;

	sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, NULL, NULL, NULL);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedMatrixOfComplexPoly" : "getAllocatedMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_piNbCoef = (int*)MALLOC(sizeof(int) * *_piRows * *_piCols);

	sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, *_piNbCoef, NULL, NULL);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedMatrixOfComplexPoly" : "getAllocatedMatrixOfPoly", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pdblReal = (double**)MALLOC(sizeof(double*) * *_piRows * *_piCols);
	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		(*_pdblReal)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
	}

	if(_iComplex)
	{
		*_pdblImg	= (double**)MALLOC(sizeof(double*) * *_piRows * *_piCols);
		for(int i = 0 ; i < *_piRows * *_piCols ; i++)
		{
			(*_pdblImg)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
		}
	}

	sciErr = getCommonMatrixOfPoly(_pvCtx, _piAddress, _iComplex, _piRows, _piCols, *_piNbCoef, *_pdblReal, _pdblImg == NULL ? NULL : *_pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_MATRIX_POLY, _("%s: Unable to get argument #%d"), _iComplex ? "getAllocatedSingleComplexPoly" : "getAllocatedSinglePoly", getRhsFromAddress(_pvCtx, _piAddress));
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal)
{
	return getCommonAllocatedNamedMatrixOfPoly(_pvCtx, _pstName, 0, _piRows, _piCols, _piNbCoef, _pdblReal, NULL);
}
/*--------------------------------------------------------------------------*/
int getAllocatedNamedMatrixOfComplexPoly(void* _pvCtx, char* _pstName, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
	return getCommonAllocatedNamedMatrixOfPoly(_pvCtx, _pstName, 1, _piRows, _piCols, _piNbCoef, _pdblReal, _pdblImg);
}
/*--------------------------------------------------------------------------*/
static int getCommonAllocatedNamedMatrixOfPoly(void* _pvCtx, char* _pstName, int _iComplex, int* _piRows, int* _piCols, int** _piNbCoef, double*** _pdblReal, double*** _pdblImg)
{
	SciErr sciErr;
	int iRows	= 0;
	int iCols	= 0;

	double* pdblReal = NULL;
	double* pdblImg	 = NULL;


	sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, _piRows, _piCols, NULL, NULL, NULL);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_MATRIX_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedMatrixOfComplexPoly" : "getAllocatedNamedMatrixOfPoly", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_piNbCoef = (int*)MALLOC(sizeof(int) * *_piRows * *_piCols);

	sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, _piRows, _piCols, *_piNbCoef, NULL, NULL);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_MATRIX_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedMatrixOfComplexPoly" : "getAllocatedNamedMatrixOfPoly", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	*_pdblReal = (double**)MALLOC(sizeof(double*) * *_piRows * *_piCols);
	for(int i = 0 ; i < *_piRows * *_piCols ; i++)
	{
		(*_pdblReal)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
	}

	if(_iComplex)
	{
		*_pdblImg	= (double**)MALLOC(sizeof(double*) * *_piRows * *_piCols);
		for(int i = 0 ; i < *_piRows * *_piCols ; i++)
		{
			(*_pdblImg)[i] = (double*)MALLOC(sizeof(double) * (*_piNbCoef)[i]);
		}
	}

	sciErr = readCommonNamedMatrixOfPoly(_pvCtx, _pstName, _iComplex, _piRows, _piCols, *_piNbCoef, *_pdblReal, _pdblImg == NULL ? NULL : *_pdblImg);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_ALLOC_NAMED_MATRIX_POLY, _("%s: Unable to get argument \"%s\""), _iComplex ? "getAllocatedNamedMatrixOfComplexPoly" : "getAllocatedNamedMatrixOfPoly", _pstName);
		printError(&sciErr, 0);
		return sciErr.iErr;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
void freeAllocatedSinglePoly(double* _pdblReal)
{
	FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedSingleComplexPoly(double* _pdblReal, double* _pdblImg)
{
	freeAllocatedSinglePoly(_pdblReal);
	FREE(_pdblImg);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedMatrixOfPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal)
{
	FREE(_piNbCoef);

	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		FREE(_pdblReal[i]);
	}
	FREE(_pdblReal);
}
/*--------------------------------------------------------------------------*/
void freeAllocatedMatrixOfComplexPoly(int _iRows, int _iCols, int* _piNbCoef, double** _pdblReal, double** _pdblImg)
{
	freeAllocatedMatrixOfPoly(_iRows, _iCols, _piNbCoef, _pdblReal);

	for(int i = 0 ; i < _iRows * _iCols ; i++)
	{
		FREE(_pdblImg[i]);
	}
	FREE(_pdblImg);
}
