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
#include "CallScilab.h"
#include "api_common.h"
#include "api_internal_common.h"
#include "api_double.h"
#include "api_int.h"
#include "api_string.h"
#include "stack-c.h"
#include "stackinfo.h"
#include "Scierror.h"
#include "alltypes.hxx"

using namespace types;
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
int getVarDimension(int* _piAddress, int* _piRows, int* _piCols)
{
	if(_piAddress != NULL && isVarMatrixType(_piAddress))
	{
		*_piRows		= ((GenericType*)_piAddress)->rows_get();
		*_piCols		= ((GenericType*)_piAddress)->cols_get();
		return 0;
	}
	else
	{
		*_piRows		= 0;
		*_piCols		= 0;
		return 1;
	}
}
/*--------------------------------------------------------------------------*/
int getNamedVarDimension(char *_pstName, int* _piRows, int* _piCols)
{
	int iRet				= 0;
	int* piAddr				= NULL;
	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 0;
	}
	return getVarDimension(piAddr, _piRows, _piCols);
}
/*--------------------------------------------------------------------------*/
int getVarAddressFromPosition(int _iVar, int** _piAddress, int* _piKey)
{
	GatewayStruct *pStr =  (GatewayStruct*)_piKey;

	if(pStr->m_pin->size() < _iVar)
	{
		return 1;
	}

	*_piAddress = (int*)((*pStr->m_pin)[_iVar - 1]);

	return 0;
}
/*--------------------------------------------------------------------------*/
int getVarNameFromPosition(int _iVar, char* _pstName)
{
	int iNameLen				= 0;
	int iJob1						= 1;
	CvNameL(&vstk_.idstk[(_iVar - 1) * 6], _pstName, &iJob1, &iNameLen);
	if(iNameLen == 0)
	{
		return 1;
	}

	_pstName[iNameLen]	= '\0';
	return 0;
}
/*--------------------------------------------------------------------------*/
int getNewVarAddressFromPosition(int _iVar, int** _piAddress)
{
	int iAddr			= iadr(*Lstk(_iVar));
	*_piAddress		= istk(iAddr);
	//intersci_.ntypes[_iVar - 1] = '$' ;

	return 0;
}
/*--------------------------------------------------------------------------*/
int getVarAddressFromName(char* _pstName, int** _piAddress)
{
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
		return 1;
	}

	//No idea :(
  if ( *Infstk(Fin) == 2)
		Fin = *istk(iadr(*Lstk(Fin )) + 1 + 1);

	//get variable address
	//WARNING check in VarType can be negative
	getNewVarAddressFromPosition(Fin, &piAddr);

	*_piAddress = piAddr;
	return 0;
}
/*--------------------------------------------------------------------------*/
int getVarType(int* _piAddress)
{
	if(_piAddress == NULL)
	{
		return 0;
	}

	InternalType* pIT = (types::InternalType*)_piAddress;
	InternalType::RealType iType = pIT->getType();

	switch(iType)
	{
	case InternalType::RealDouble : 
		return sci_matrix;
	case InternalType::RealPoly : 
		return sci_poly;
	case InternalType::RealBool : 
		return sci_boolean;
	case InternalType::RealInt : 
		return sci_ints;
	case InternalType::RealString : 
		return sci_strings;
	case InternalType::RealList :
	  return sci_list;
	default :
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
int getNamedVarType(char* _pstName)
{
	int iRet				= 0;
	int* piAddr				= NULL;
	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet)
	{
		return 0;
	}
	return getVarType(piAddr);
}
/*--------------------------------------------------------------------------*/
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
		iComplex = ((InternalType*)_piAddress)->getAsDouble()->isComplex() ? 1 : 0;
		break;
	case sci_poly :
		iComplex = ((InternalType*)_piAddress)->getAsPoly()->isComplex() ? 1 : 0;
		break;
	case sci_sparse :
		iComplex = _piAddress[3];
		break;
	default:
		iComplex = 0;
	}
	return iComplex;
}
/*--------------------------------------------------------------------------*/
int isNamedVarComplex(char *_pstName)
{
	int iRet				= 0;
	int* piAddr				= NULL;
	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet) return 0;
	return isVarComplex(piAddr);
}
/*--------------------------------------------------------------------------*/
void createNamedVariable(int *_piVarID)
{
	int iOne				= 1;
	//it seems this part setting up the output format but "stackp" print anything
//	int iSaveLct		= C2F(iop).lct[3];
//  C2F(iop).lct[3] = -1;
  C2F(stackp)(_piVarID, &iOne);
//  C2F(iop).lct[3] = iSaveLct;
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
int isVarMatrixType(int* _piAddress)
{
	if(_piAddress != NULL)
	{
		int iType = getVarType(_piAddress);

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
int isNamedVarMatrixType(char *_pstName)
{
	int iRet				= 0;
	int* piAddr				= NULL;
	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet) return 0;
    return isVarMatrixType(piAddr);
}
/*--------------------------------------------------------------------------*/
int getProcessMode(int* _piAddCheck, int* _piAddRef, int *_piMode)
{
	int iRet				= 0;
	int iRows1			= 0;
	int iCols1			= 0;
	int iRows2			= 0;
	int iCols2			= 0;
	int iMode				= 0;
	int* piAddr2		= NULL;

	iRet = getVarDimension(_piAddRef, &iRows1, &iCols1);
	if(iRet)
	{
		return 1;
	}

	if(getVarType(_piAddCheck) == sci_matrix && !isVarComplex(_piAddCheck))
	{
		double *pdblReal2 = NULL;
		iRet = getMatrixOfDouble(_piAddCheck, &iRows2, &iCols2, &pdblReal2);
		if(iRet)
		{
			return 1;
		}

		if(iRows2 != 1 || iCols2 != 1)
		{
			Error(89);
			return 1;
		}

		iMode = (int)pdblReal2[0];
	}
	else if(getVarType(_piAddCheck) == sci_strings)
	{
		int iLen					= 0;
		char *pstMode[1]	= {""};

		iRet = getVarDimension(_piAddCheck, &iRows2, &iCols2);
		if(iRet)
		{
			return 1;
		}

		if(iRows2 != 1 || iCols2 != 1)
		{
			Error(89);
			return 1;
		}

		iRet = getMatrixOfString(_piAddCheck, &iRows2, &iCols2, &iLen, NULL);
		if(iRet)
		{
			return 1;
		}

		pstMode[0] = (char*)malloc(sizeof(char) * (iLen + 1)); //+1 for null termination
		iRet = getMatrixOfString(_piAddCheck, &iRows2, &iCols2, &iLen, pstMode);
		if(iRet)
		{
			return 1;
		}

		iMode = (int)pstMode[0][0];
	}
	else
	{
		Error(44);
		return 2;
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
		Error(44);
		return 2;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int getDimFromVar(int* _piAddress, int* _piVal)
{
	int iRet					= 0;
	int iType					= 0;
	int iRows					= 0;
	int iCols					= 0;
	double *pdblReal	= NULL;
	int *piRealData		= NULL;

	iType = getVarType(_piAddress);

	if(iType == sci_matrix)
	{
		if(isVarComplex(_piAddress))
		{
			Error(89);
			return 1;
		}

		iRet = getMatrixOfDouble(_piAddress, &iRows, &iCols, &pdblReal);
		if(iRet)
		{
			return 1;
		}
		*_piVal = (int)Max(pdblReal[0], 0);
	}
	else if(iType == sci_ints)
	{
		int iPrec			= 0;
		int iYType		= 4;
		int iXInc			= 1;
		int iYInc			= 1;

		iRet = getVarDimension(_piAddress, &iRows, &iCols);
		if(iRet)
		{
			return 1;
		}

		if(iRows != 1 || iCols != 1)
		{
			return 1;
		}

		iRet = getMatrixOfIntegerPrecision(_piAddress, &iPrec);
		if(iRet)
		{
			return 1;
		}

		switch(iPrec)
		{
		case SCI_INT8 :
			{
				char* pcData		= NULL;
				iRet = getMatrixOfInteger8(_piAddress, &iRows, &iCols, &pcData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = pcData[0];
			}
			break;
		case SCI_UINT8 :
			{
				unsigned char* pucData		= NULL;
				iRet = getMatrixOfUnsignedInteger8(_piAddress, &iRows, &iCols, &pucData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = pucData[0];
			}
			break;
		case SCI_INT16 :
			{
				short* psData		= NULL;
				iRet = getMatrixOfInteger16(_piAddress, &iRows, &iCols, &psData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = psData[0];
			}
			break;
		case SCI_UINT16 :
			{
				unsigned short* pusData		= NULL;
				iRet = getMatrixOfUnsignedInteger16(_piAddress, &iRows, &iCols, &pusData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = pusData[0];
			}
			break;
		case SCI_INT32 :
			{
				int* piData		= NULL;
				iRet = getMatrixOfInteger32(_piAddress, &iRows, &iCols, &piData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = piData[0];
			}
			break;
		case SCI_UINT32 :
			{
				unsigned int* puiData		= NULL;
				iRet = getMatrixOfUnsignedInteger32(_piAddress, &iRows, &iCols, &puiData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = puiData[0];
			}
			break;
		case SCI_INT64 :
			{
				long long* pllData	= NULL;
				iRet = getMatrixOfInteger64(_piAddress, &iRows, &iCols, &pllData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = (int)pllData[0];
			}
			break;
		case SCI_UINT64 :
			{
				unsigned long long* pullData		= NULL;
				iRet = getMatrixOfUnsignedInteger64(_piAddress, &iRows, &iCols, &pullData);
				if(iRet)
				{
					return 1;
				}
				*_piVal = (int)pullData[0];
			}
			break;
		}
	}
	else
	{
		Error(89);
		return 1;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
int getDimFromNamedVar(char* _pstName, int* _piVal)
{
	int iRet				= 0;
	int* piAddr				= NULL;
	iRet = getVarAddressFromName(_pstName, &piAddr);
	if(iRet) return iRet;
	return getDimFromVar(piAddr, _piVal);
}
/*--------------------------------------------------------------------------*/
