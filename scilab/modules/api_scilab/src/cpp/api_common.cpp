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
#include "api_common.h"
#include "api_internal_common.h"
#include "api_double.h"
#include "api_int.h"
#include "api_string.h"
#include "stack-c.h"
#include "stackinfo.h"
#include "Scierror.h"
#include "localization.h"

/*Global structure for scilab 5.x*/
extern "C"
{
	StrCtx* pvApiCtx = NULL;
}

/*--------------------------------------------------------------------------*/
/* Defined in SCI/modules/core/src/fortran/cvname.f */
extern "C" {
extern int C2F(cvnamel)(int *id,char *str,int *jobptr,int *str_len); 
/* *jobptr==0: Get Scilab codes from C-string */
/* *jobptr==1: Get C-string from Scilab codes */

extern int C2F(stackp)(int *,int *);
};
/*--------------------------------------------------------------------------*/
#define idstk(x,y) (C2F(vstk).idstk+(x-1)+(y-1)*nsiz)
#define CvNameL(id,str,jobptr,str_len) C2F(cvnamel)(id,str,jobptr,str_len);
/*--------------------------------------------------------------------------*/

StrErr getVarDimension(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	if(_piAddress != NULL && isVarMatrixType(_pvCtx, _piAddress))
	{
		*_piRows		= _piAddress[1];
		*_piCols		= _piAddress[2];
	}
	else
	{
		*_piRows		= 0;
		*_piCols		= 0;
		if(_piAddress == NULL)
		{
			addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getVarDimension");
		}
		else
		{
			addErrorMessage(&strErr, API_ERROR_NOT_MATRIX_TYPE, _("%s: matrix argument excepted"), "getVarDimension");
		}
	}
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getNamedVarDimension(void* _pvCtx, char *_pstName, int* _piRows, int* _piCols)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;
	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_NAMED_VARDIM, _("%s: Unable to get dimension of variable \"%s\""), "getNamedVarDimension", _pstName);
		return strErr;
	}

	strErr = getVarDimension(_pvCtx, piAddr, _piRows, _piCols);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_NAMED_VARDIM, _("%s: Unable to get dimension of variable \"%s\""), "getNamedVarDimension", _pstName);
		return strErr;
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iAddr			= 0;
	int iValType	= 0;
	/* we accept a call to getVarAddressFromPosition after a create... call */
	if(_iVar > Rhs && _iVar > Nbvars)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POSITION, _("%s: bad call to %s! (1rst argument).\n"), ((StrCtx*)_pvCtx)->pstName, "getVarAddressFromPosition");
		return strErr;
	}

	iAddr = iadr(*Lstk(Top - Rhs + _iVar));
	iValType	= *istk(iAddr);
	if(iValType < 0)
	{
		iAddr				= iadr(*istk(iAddr + 1));
	}

	*_piAddress		= istk(iAddr);
	intersci_.ntypes[_iVar - 1] = '$' ;
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getVarNameFromPosition(void* _pvCtx, int _iVar, char* _pstName)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iNameLen				= 0;
	int iJob1						= 1;
	CvNameL(&vstk_.idstk[(_iVar - 1) * 6], _pstName, &iJob1, &iNameLen);
	if(iNameLen == 0)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_NAME, _("%s: Unable to get name of argument #%d"), "getVarNameFromPosition", _iVar);
		return strErr;
	}

	_pstName[iNameLen]	= '\0';
	return strErr;
}
/*--------------------------------------------------------------------------*/
int getNewVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
{
	int iAddr			= iadr(*Lstk(_iVar));
	*_piAddress		= istk(iAddr);
	return 0;
}
/*--------------------------------------------------------------------------*/
StrErr getVarAddressFromName(void* _pvCtx, char* _pstName, int** _piAddress)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iVarID[nsiz];
	int* piAddr				= NULL;

	//get variable id from name
	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));

	//define scope of search
  Fin = -1;
	//search variable
  C2F(stackg)(iVarID);

	if (Err > 0 || Fin == 0)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_NAME, _("%s: Unable to get address of variable \"%s\""), "getVarAddressFromName", _pstName);
		return strErr;
	}

	//No idea :(
  if ( *Infstk(Fin) == 2)
		Fin = *istk(iadr(*Lstk(Fin )) + 1 + 1);

	//get variable address
	getNewVarAddressFromPosition(_pvCtx, Fin, &piAddr);

	*_piAddress = piAddr;
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getVarType(void* _pvCtx, int* _piAddress, int* _piType)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		addErrorMessage(&strErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getVarType");
		return strErr;
	}

	*_piType = _piAddress[0];
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getNamedVarType(void* _pvCtx, char* _pstName, int* _piType)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_NAMED_TYPE, _("%s: Unable to get type of variable \"%s\""), "getNamedVarType", _pstName);
		return strErr;
	}
	
	strErr = getVarType(_pvCtx, piAddr, _piType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_NAMED_TYPE, _("%s: Unable to get type of variable \"%s\""), "getNamedVarType", _pstName);
		return strErr;
	}
	return strErr;
}
/*--------------------------------------------------------------------------*/
int isVarComplex(void* _pvCtx, int* _piAddress)
{
	int iType			= 0;
	int iComplex	= 0;

	if(_piAddress == NULL)
	{
		return 0;
	}

	getVarType(_pvCtx, _piAddress, &iType);
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
/*--------------------------------------------------------------------------*/
int isNamedVarComplex(void* _pvCtx, char *_pstName)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		return 0;
	}
	return isVarComplex(_pvCtx, piAddr);
}
/*--------------------------------------------------------------------------*/
void createNamedVariable(int *_piVarID)
{
	int iOne				= 1;
  C2F(stackp)(_piVarID, &iOne);
}
/*--------------------------------------------------------------------------*/
int updateInterSCI(int _iVar, char _cType, int _iSCIAddress, int _iSCIDataAddress)
{
	intersci_.ntypes[_iVar - 1]	= _cType;
	intersci_.iwhere[_iVar - 1]	= _iSCIAddress;
	intersci_.lad[_iVar - 1]		= _iSCIDataAddress;
	return 0;
}
/*--------------------------------------------------------------------------*/
int updateLstk(int _iNewpos, int _iSCIDataAddress, int _iVarSize)
{
	*Lstk(_iNewpos + 1) = _iSCIDataAddress + _iVarSize;
	return 0;
}
/*--------------------------------------------------------------------------*/
int isVarMatrixType(void* _pvCtx, int* _piAddress)
{
	if(_piAddress != NULL)
	{
		int iType = 0;
		getVarType(_pvCtx, _piAddress, &iType);

		switch(iType)
		{
		case sci_matrix :
		case sci_poly : 
		case sci_boolean : 
		case sci_sparse : 
		case sci_boolean_sparse : 
		case sci_matlab_sparse : 
		case sci_ints : 
		case sci_handles : 
		case sci_strings : 
			return 1;
		default :
			return 0;
		}
	}
	else
	{
		return 0;
	}
	return 1;
}
/*--------------------------------------------------------------------------*/
int isNamedVarMatrixType(void* _pvCtx, char *_pstName)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr				= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		return 0;
	}
	return isVarMatrixType(_pvCtx, piAddr);
}
/*--------------------------------------------------------------------------*/
StrErr getProcessMode(void* _pvCtx, int _iPos, int* _piAddRef, int *_piMode)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iRows1			= 0;
	int iCols1			= 0;
	int iRows2			= 0;
	int iCols2			= 0;
	int iType2			= 0;
	int iMode				= 0;
	int* piAddr2		= NULL;

	strErr = getVarDimension(_pvCtx, _piAddRef, &iRows1, &iCols1);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
		return strErr;
	}

	strErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr2);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get variable address"), "getProcessMode");
		return strErr;
	}

	strErr = getVarType(_pvCtx, piAddr2, &iType2);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument type"), "getProcessMode");
		return strErr;
	}
		
	if(iType2 == sci_matrix && !isVarComplex(_pvCtx, piAddr2))
	{
		double *pdblReal2 = NULL;
		strErr = getMatrixOfDouble(_pvCtx, piAddr2, &iRows2, &iCols2, &pdblReal2);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
			return strErr;
		}

		if(iRows2 != 1 || iCols2 != 1)
		{
			addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1, 1);
			return strErr;
		}

		iMode = (int)pdblReal2[0];
	}
	else if(iType2 == sci_strings)
	{
		int iLen					= 0;
		char *pstMode[1]	= {""};

		strErr = getVarDimension(_pvCtx, piAddr2, &iRows2, &iCols2);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
			return strErr;
		}

		if(iRows2 != 1 || iCols2 != 1)
		{
			addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1, 1);
			return strErr;
		}

		strErr = getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, NULL);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
			return strErr;
		}

		pstMode[1] = (char*)malloc(sizeof(char) * (iLen + 1)); //+1 for null termination
		strErr = getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, pstMode);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
			return strErr;
		}

		iMode = (int)pstMode[0][0];
	}
	else
	{
		addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"), "getProcessMode", _iPos);
		return strErr;
	}

	if(iMode == ROW_LETTER || iMode == BY_ROWS)
		*_piMode = BY_ROWS;
	else if(iMode == COL_LETTER || iMode == BY_COLS)
		*_piMode = BY_COLS;
	else if(iMode == STAR_LETTER || iMode == BY_ALL)
		*_piMode = BY_ALL;
	else if(iMode == MTLB_LETTER || iMode == BY_MTLB)
	{
		*_piMode = 0;
		if(iRows1 > 1)
			*_piMode = 1;
		else if(iCols1 > 1)
			*_piMode = 2;
	}
	else
	{
		addErrorMessage(&strErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "getProcessMode", _iPos, "'*', 'r', 'c', 'm', '0', '1', '2'", "-1");
		return strErr;
	}
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getDimFromVar(void* _pvCtx, int* _piAddress, int* _piVal)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int iType					= 0;
	int iRows					= 0;
	int iCols					= 0;
	double *pdblReal	= NULL;

	strErr = getVarType(_pvCtx, _piAddress, &iType);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument type"), "getDimFromVar");
		return strErr;
	}

	if(iType == sci_matrix)
	{
		if(isVarComplex(_pvCtx, _piAddress))
		{
			addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "getDimFromVar", getRhsFromAddress(_pvCtx, _piAddress));
			return strErr;
		}

		strErr = getMatrixOfDouble(_pvCtx, _piAddress, &iRows, &iCols, &pdblReal);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
			return strErr;
		}

		*_piVal = (int)Max(pdblReal[0], 0);
	}
	else if(iType == sci_ints)
	{
		int iPrec			= 0;

		strErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument dimension"), "getDimFromVar");
			return strErr;
		}

		if(iRows != 1 || iCols != 1)
		{
			addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", getRhsFromAddress(_pvCtx, _piAddress), 1, 1);
			return strErr;
		}

		strErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddress, &iPrec);
		if(strErr.iErr)
		{
			addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument precision"), "getDimFromVar");
			return strErr;
		}

		switch(iPrec)
		{
		case SCI_INT8 :
			{
				char* pcData		= NULL;
				strErr = getMatrixOfInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pcData);
				if(strErr.iErr)
				{
					addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return strErr;
				}
				*_piVal = pcData[0];
			}
			break;
		case SCI_UINT8 :
			{
				unsigned char* pucData		= NULL;
				strErr = getMatrixOfUnsignedInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pucData);
				if(strErr.iErr)
				{
					addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return strErr;
				}
				*_piVal = pucData[0];
			}
			break;
		case SCI_INT16 :
			{
				short* psData		= NULL;
				strErr = getMatrixOfInteger16(_pvCtx, _piAddress, &iRows, &iCols, &psData);
				if(strErr.iErr)
				{
					addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return strErr;
				}
				*_piVal = psData[0];
			}
			break;
		case SCI_UINT16 :
			{
				unsigned short* pusData		= NULL;
				strErr = getMatrixOfUnsignedInteger16(_pvCtx, _piAddress, &iRows, &iCols, &pusData);
				if(strErr.iErr)
				{
					addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return strErr;
				}
				*_piVal = pusData[0];
			}
			break;
		case SCI_INT32 :
			{
				int* piData		= NULL;
				strErr = getMatrixOfInteger32(_pvCtx, _piAddress, &iRows, &iCols, &piData);
				if(strErr.iErr)
				{
					addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return strErr;
				}
				*_piVal = piData[0];
			}
			break;
		case SCI_UINT32 :
			{
				unsigned int* puiData		= NULL;
				strErr = getMatrixOfUnsignedInteger32(_pvCtx, _piAddress, &iRows, &iCols, &puiData);
				if(strErr.iErr)
				{
					addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return strErr;
				}
				*_piVal = puiData[0];
			}
			break;
		}
	}
	else
	{
			addErrorMessage(&strErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for input argument #%d: A real scalar or a integer scalar expected.\n"), "getDimFromVar", getRhsFromAddress(_pvCtx, _piAddress));
			return strErr;
	}
	return strErr;
}
/*--------------------------------------------------------------------------*/
StrErr getDimFromNamedVar(void* _pvCtx, char* _pstName, int* _piVal)
{
	StrErr strErr; strErr.iErr = 0; strErr.iMsgCount = 0;
	int* piAddr		= NULL;

	strErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_NAMED_DIMFROMVAR, _("%s: Unable to get dimension from variable \"%s\""), "getDimFromNamedVar", _pstName);
		return strErr;
	}

	strErr = getDimFromVar(_pvCtx, piAddr, _piVal);
	if(strErr.iErr)
	{
		addErrorMessage(&strErr, API_ERROR_GET_NAMED_DIMFROMVAR, _("%s: Unable to get dimension from variable \"%s\""), "getDimFromNamedVar", _pstName);
		return strErr;
	}

	return strErr;
}
/*--------------------------------------------------------------------------*/
int getRhsFromAddress(void* _pvCtx, int* _piAddress)
{
	int i = 0;
	int* piAddr = NULL;
	for(i = 0 ; i < Rhs ; i++)
	{
		getVarAddressFromPosition(_pvCtx, i + 1, &piAddr);
		if(_piAddress == piAddr)
		{
			return i + 1;
		}
	}
	return 0;
}

/*--------------------------------------------------------------------------*/
