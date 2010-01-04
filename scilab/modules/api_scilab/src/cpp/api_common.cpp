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

SciErr getVarDimension(void* _pvCtx, int* _piAddress, int* _piRows, int* _piCols)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
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
			addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getVarDimension");
		}
		else
		{
			addErrorMessage(&sciErr, API_ERROR_NOT_MATRIX_TYPE, _("%s: matrix argument excepted"), "getVarDimension");
		}
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getNamedVarDimension(void* _pvCtx, char *_pstName, int* _piRows, int* _piCols)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;
	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_NAMED_VARDIM, _("%s: Unable to get dimension of variable \"%s\""), "getNamedVarDimension", _pstName);
		return sciErr;
	}

	sciErr = getVarDimension(_pvCtx, piAddr, _piRows, _piCols);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_NAMED_VARDIM, _("%s: Unable to get dimension of variable \"%s\""), "getNamedVarDimension", _pstName);
		return sciErr;
	}

	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iAddr			= 0;
	int iValType	= 0;
	/* we accept a call to getVarAddressFromPosition after a create... call */
	if(_iVar > Rhs && _iVar > Nbvars)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POSITION, _("%s: bad call to %s! (1rst argument).\n"), ((StrCtx*)_pvCtx)->pstName, "getVarAddressFromPosition");
		return sciErr;
	}

	iAddr = iadr(*Lstk(Top - Rhs + _iVar));
	iValType	= *istk(iAddr);
	if(iValType < 0)
	{
		iAddr				= iadr(*istk(iAddr + 1));
	}

	*_piAddress		= istk(iAddr);
	intersci_.ntypes[_iVar - 1] = '$' ;
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getVarNameFromPosition(void* _pvCtx, int _iVar, char* _pstName)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iNameLen				= 0;
	int iJob1						= 1;
	CvNameL(&vstk_.idstk[(_iVar - 1) * 6], _pstName, &iJob1, &iNameLen);
	if(iNameLen == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Unable to get name of argument #%d"), "getVarNameFromPosition", _iVar);
		return sciErr;
	}

	_pstName[iNameLen]	= '\0';
	return sciErr;
}
/*--------------------------------------------------------------------------*/
int getNewVarAddressFromPosition(void* _pvCtx, int _iVar, int** _piAddress)
{
	int iAddr			= iadr(*Lstk(_iVar));
	*_piAddress		= istk(iAddr);
	return 0;
}
/*--------------------------------------------------------------------------*/
SciErr getVarAddressFromName(void* _pvCtx, char* _pstName, int** _piAddress)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iVarID[nsiz];
	int* piAddr				= NULL;

	//get variable id from name
	C2F(str2name)(_pstName, iVarID, (int)strlen(_pstName));

	//define scope of search
  Fin = -1;
	Err = 0;
	//search variable
  C2F(stackg)(iVarID);

	//No idea :(
  if ( *Infstk(Fin) == 2)
		Fin = *istk(iadr(*Lstk(Fin )) + 1 + 1);

	if (Err > 0 || Fin == 0)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_NAME, _("%s: Unable to get address of variable \"%s\""), "getVarAddressFromName", _pstName);
		return sciErr;
	}


	//get variable address
	getNewVarAddressFromPosition(_pvCtx, Fin, &piAddr);

	*_piAddress = piAddr;
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getVarType(void* _pvCtx, int* _piAddress, int* _piType)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	if(_piAddress == NULL)
	{
		addErrorMessage(&sciErr, API_ERROR_INVALID_POINTER, _("%s: Invalid argument address"), "getVarType");
		return sciErr;
	}

	*_piType = _piAddress[0];
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getNamedVarType(void* _pvCtx, char* _pstName, int* _piType)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_NAMED_TYPE, _("%s: Unable to get type of variable \"%s\""), "getNamedVarType", _pstName);
		return sciErr;
	}
	
	sciErr = getVarType(_pvCtx, piAddr, _piType);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_NAMED_TYPE, _("%s: Unable to get type of variable \"%s\""), "getNamedVarType", _pstName);
		return sciErr;
	}
	return sciErr;
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
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
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
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr				= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		return 0;
	}
	return isVarMatrixType(_pvCtx, piAddr);
}
/*--------------------------------------------------------------------------*/
SciErr getProcessMode(void* _pvCtx, int _iPos, int* _piAddRef, int *_piMode)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iRows1			= 0;
	int iCols1			= 0;
	int iRows2			= 0;
	int iCols2			= 0;
	int iType2			= 0;
	int iMode				= 0;
	int* piAddr2		= NULL;

	sciErr = getVarDimension(_pvCtx, _piAddRef, &iRows1, &iCols1);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
		return sciErr;
	}

	sciErr = getVarAddressFromPosition(_pvCtx, _iPos, &piAddr2);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get variable address"), "getProcessMode");
		return sciErr;
	}

	sciErr = getVarType(_pvCtx, piAddr2, &iType2);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument type"), "getProcessMode");
		return sciErr;
	}
		
	if(iType2 == sci_matrix && !isVarComplex(_pvCtx, piAddr2))
	{
		double *pdblReal2 = NULL;
		sciErr = getMatrixOfDouble(_pvCtx, piAddr2, &iRows2, &iCols2, &pdblReal2);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
			return sciErr;
		}

		if(iRows2 != 1 || iCols2 != 1)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1, 1);
			return sciErr;
		}

		iMode = (int)pdblReal2[0];
	}
	else if(iType2 == sci_strings)
	{
		int iLen					= 0;
		char *pstMode[1]	= {""};

		sciErr = getVarDimension(_pvCtx, piAddr2, &iRows2, &iCols2);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument dimension"), "getProcessMode");
			return sciErr;
		}

		if(iRows2 != 1 || iCols2 != 1)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", _iPos, 1, 1);
			return sciErr;
		}

		sciErr = getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, NULL);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
			return sciErr;
		}

		pstMode[1] = (char*)malloc(sizeof(char) * (iLen + 1)); //+1 for null termination
		sciErr = getMatrixOfString(_pvCtx, piAddr2, &iRows2, &iCols2, &iLen, pstMode);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Unable to get argument data"), "getProcessMode");
			return sciErr;
		}

		iMode = (int)pstMode[0][0];
	}
	else
	{
		addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong type for input argument #%d: A string or a scalar expected.\n"), "getProcessMode", _iPos);
		return sciErr;
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
		addErrorMessage(&sciErr, API_ERROR_GET_PROCESSMODE, _("%s: Wrong value for input argument #%d: ''%s'' or ''%s'' expected.\n"), "getProcessMode", _iPos, "'*', 'r', 'c', 'm', '0', '1', '2'", "-1");
		return sciErr;
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getDimFromVar(void* _pvCtx, int* _piAddress, int* _piVal)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int iType					= 0;
	int iRows					= 0;
	int iCols					= 0;
	double *pdblReal	= NULL;

	sciErr = getVarType(_pvCtx, _piAddress, &iType);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument type"), "getDimFromVar");
		return sciErr;
	}

	if(iType == sci_matrix)
	{
		if(isVarComplex(_pvCtx, _piAddress))
		{
			addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for argument %d: Real matrix expected.\n"), "getDimFromVar", getRhsFromAddress(_pvCtx, _piAddress));
			return sciErr;
		}

		sciErr = getMatrixOfDouble(_pvCtx, _piAddress, &iRows, &iCols, &pdblReal);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
			return sciErr;
		}

		*_piVal = (int)Max(pdblReal[0], 0);
	}
	else if(iType == sci_ints)
	{
		int iPrec			= 0;

		sciErr = getVarDimension(_pvCtx, _piAddress, &iRows, &iCols);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument dimension"), "getDimFromVar");
			return sciErr;
		}

		if(iRows != 1 || iCols != 1)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong size for argument %d: (%d,%d) expected.\n"), "getProcessMode", getRhsFromAddress(_pvCtx, _piAddress), 1, 1);
			return sciErr;
		}

		sciErr = getMatrixOfIntegerPrecision(_pvCtx, _piAddress, &iPrec);
		if(sciErr.iErr)
		{
			addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument precision"), "getDimFromVar");
			return sciErr;
		}

		switch(iPrec)
		{
		case SCI_INT8 :
			{
				char* pcData		= NULL;
				sciErr = getMatrixOfInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pcData);
				if(sciErr.iErr)
				{
					addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return sciErr;
				}
				*_piVal = pcData[0];
			}
			break;
		case SCI_UINT8 :
			{
				unsigned char* pucData		= NULL;
				sciErr = getMatrixOfUnsignedInteger8(_pvCtx, _piAddress, &iRows, &iCols, &pucData);
				if(sciErr.iErr)
				{
					addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return sciErr;
				}
				*_piVal = pucData[0];
			}
			break;
		case SCI_INT16 :
			{
				short* psData		= NULL;
				sciErr = getMatrixOfInteger16(_pvCtx, _piAddress, &iRows, &iCols, &psData);
				if(sciErr.iErr)
				{
					addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return sciErr;
				}
				*_piVal = psData[0];
			}
			break;
		case SCI_UINT16 :
			{
				unsigned short* pusData		= NULL;
				sciErr = getMatrixOfUnsignedInteger16(_pvCtx, _piAddress, &iRows, &iCols, &pusData);
				if(sciErr.iErr)
				{
					addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return sciErr;
				}
				*_piVal = pusData[0];
			}
			break;
		case SCI_INT32 :
			{
				int* piData		= NULL;
				sciErr = getMatrixOfInteger32(_pvCtx, _piAddress, &iRows, &iCols, &piData);
				if(sciErr.iErr)
				{
					addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return sciErr;
				}
				*_piVal = piData[0];
			}
			break;
		case SCI_UINT32 :
			{
				unsigned int* puiData		= NULL;
				sciErr = getMatrixOfUnsignedInteger32(_pvCtx, _piAddress, &iRows, &iCols, &puiData);
				if(sciErr.iErr)
				{
					addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Unable to get argument data"), "getDimFromVar");
					return sciErr;
				}
				*_piVal = puiData[0];
			}
			break;
		}
	}
	else
	{
			addErrorMessage(&sciErr, API_ERROR_GET_DIMFROMVAR, _("%s: Wrong type for input argument #%d: A real scalar or a integer scalar expected.\n"), "getDimFromVar", getRhsFromAddress(_pvCtx, _piAddress));
			return sciErr;
	}
	return sciErr;
}
/*--------------------------------------------------------------------------*/
SciErr getDimFromNamedVar(void* _pvCtx, char* _pstName, int* _piVal)
{
	SciErr sciErr; sciErr.iErr = 0; sciErr.iMsgCount = 0;
	int* piAddr		= NULL;

	sciErr = getVarAddressFromName(_pvCtx, _pstName, &piAddr);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_DIMFROMVAR, _("%s: Unable to get dimension from variable \"%s\""), "getDimFromNamedVar", _pstName);
		return sciErr;
	}

	sciErr = getDimFromVar(_pvCtx, piAddr, _piVal);
	if(sciErr.iErr)
	{
		addErrorMessage(&sciErr, API_ERROR_GET_NAMED_DIMFROMVAR, _("%s: Unable to get dimension from variable \"%s\""), "getDimFromNamedVar", _pstName);
		return sciErr;
	}

	return sciErr;
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
