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
*/

extern "C"
{
#include <hdf5.h>
#include <string.h>
#include "gw_hdf5.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_scilab.h"
#include "../../../call_scilab/includes/call_scilab.h"
#include "h5_fileManagement.h"
#include "h5_readDataFromFile.h"
#include "intmacr2tree.h"
#include "stack-def.h"
}


//#define PRINT_DEBUG
int iTab = 0;


void print_tree(char* _pstMsg);

static bool import_data(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_double(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_string(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_boolean(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_integer(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_sparse(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_boolean_sparse(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_poly(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname);
static bool import_list(int _iDatasetId, int _iVarType, int _iItemPos, int* _piAddress, char* _pstVarname);

static char fname[] = "import_from_hdf5";

int sci_import_from_hdf5(char *fname,unsigned long fname_len)
{
	CheckRhs(1,2);
	CheckLhs(1,1);

	int iRows						= 0;
	int iCols						= 0;
	int iLen						= 0;
	int iType						= 0;
	int* piAddr					= NULL;
	char *pstVarName		= NULL;
	bool bImport				= false;
	StrErr strErr;

	strErr = getVarAddressFromPosition(pvApiCtx, 1, &piAddr);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	strErr = getVarDimension(pvApiCtx, piAddr, &iRows, &iCols);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	if(iRows != 1 || iCols != 1)
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
	}

	strErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, NULL);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	pstVarName = (char*)MALLOC((iLen + 1) * sizeof(char));
	strErr = getMatrixOfString(pvApiCtx, piAddr, &iRows, &iCols, &iLen, &pstVarName);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	//open hdf5 file
	int iFile = openHDF5File(pstVarName);
	if(iFile < 0)
	{
		Scierror(999, "Unable to open file: %s", pstVarName);
		return 0;
	}

	int iNbItem = 0;
	iNbItem = getVariableNames(iFile, NULL);
	char** pstVarNameList = (char**)MALLOC(sizeof(char*) * iNbItem);
	iNbItem = getVariableNames(iFile, pstVarNameList);

	//import all data
	for(int i = 0 ; i < iNbItem ; i++)
	{
		int iDataSetId = getDataSetIdFromName(iFile, pstVarNameList[i]);
		if(iDataSetId == 0)
		{
			return 0;
		}

		bImport = import_data(iDataSetId, 0, NULL, pstVarNameList[i]);
		if(bImport == false)
		{
			break;
		}

	}

	//close the file
	closeHDF5File(iFile);

	FREE(pstVarName);

	int *piReturn = NULL;
	strErr = allocMatrixOfBoolean(pvApiCtx, Rhs + 1, 1, 1, &piReturn);
	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return 0;
	}

	if(bImport == true)
	{
		piReturn[0] = 1;
	}
	else
	{
		piReturn[0] = 0;
	}

	LhsVar(1) = Rhs + 1;
	PutLhsVar();

	return 0;
}

static bool import_data(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	bool bRet = false;
	//get var type
	int iVarType = getScilabTypeFromDataSet(_iDatasetId);
	switch(iVarType)
	{
	case sci_matrix :
		{
			bRet = import_double(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	case sci_strings :
		{
			bRet = import_string(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	case sci_list :
	case sci_tlist :
	case sci_mlist :
		{
			bRet = import_list(_iDatasetId, iVarType, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	case sci_boolean :
		{
			bRet = import_boolean(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	case sci_poly :
		{
			bRet = import_poly(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	case sci_ints:
		{
			bRet = import_integer(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	case sci_sparse :
		{
			bRet = import_sparse(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	case sci_boolean_sparse :
		{
			bRet = import_boolean_sparse(_iDatasetId, _iItemPos, _piAddress, _pstVarname);
			break;
		}
	default : 
		{
			char pstMsg[512];
			sprintf(pstMsg, "Unknown type : %d", iVarType);
			print_tree(pstMsg);
		}
	}

	return bRet;
}

static bool import_double(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet						= 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	int	iRows						= 0;
	int iCols						= 0;
	int iComplex				= 0;
	StrErr strErr;

	iRet				= getDataSetDims(_iDatasetId, &iRows, &iCols);
	iComplex		= isComplexData(_iDatasetId);
	if(iRet)
	{
		return false;
	}

	if(iRows * iCols != 0)
	{
		if(iComplex)
		{
			pdblReal	= (double *) MALLOC(iRows * iCols * sizeof(double));
			pdblImg		= (double *) MALLOC(iRows * iCols * sizeof(double));
			iRet			= readDoubleComplexMatrix(_iDatasetId, iRows, iCols, pdblReal, pdblImg);
		}
		else
		{
			pdblReal	= (double *) MALLOC(iRows * iCols * sizeof(double));
			iRet			= readDoubleMatrix(_iDatasetId, iRows, iCols, pdblReal);
		}
		if(iRet)
		{
			return false;
		}
	}

	if(_piAddress == NULL)
	{
		if(iComplex)
		{
			strErr		= createNamedComplexMatrixOfDouble(pvApiCtx, _pstVarname, iRows, iCols, pdblReal, pdblImg);
		}
		else
		{
			strErr		= createNamedMatrixOfDouble(pvApiCtx, _pstVarname, iRows, iCols, pdblReal);
		}
	}
	else //if not null this variable is in a list
	{
		if(iComplex)
		{
			strErr		= createComplexMatrixOfDoubleInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pdblReal, pdblImg);
		}
		else
		{
			strErr		= createMatrixOfDoubleInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pdblReal);
		}
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	char pstMsg[512];
	sprintf(pstMsg, "double (%d x %d)", iRows, iCols);
	print_tree(pstMsg);

	if(pdblReal)
	{
		FREE(pdblReal);
	}

	if(pdblImg)
	{
		FREE(pdblImg);
	}

	if(iRet)
	{
		return false;
	}

	return true;
}

static bool import_string(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet						= 0;
	int i								= 0;
	int	iRows						= 0;
	int iCols						= 0;
	char **pstData			= NULL;
	StrErr strErr;

	iRet = getDataSetDims(_iDatasetId, &iRows, &iCols);
	if(iRet)
	{
		return false;
	}

	pstData = (char **) MALLOC(iRows * iCols * sizeof(char*));
	iRet = readStringMatrix(_iDatasetId, iRows, iCols, pstData);
	if(iRet)
	{
		return false;
	}

	if(_piAddress == NULL)
	{
		strErr = createNamedMatrixOfString(pvApiCtx, _pstVarname, iRows, iCols, pstData);
	}
	else //if not null this variable is in a list
	{
		strErr = createMatrixOfStringInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pstData);
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	char pstMsg[512];
	sprintf(pstMsg, "string (%d x %d)", iRows, iCols);
	print_tree(pstMsg);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		FREE(pstData[i]);
	}
	FREE(pstData);

	if(iRet)
	{
		return false;
	}

	return true;
}

static bool import_integer(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet						= 0;
	int	iRows						= 0;
	int iCols						= 0;
	int iPrec						= 0;
	StrErr strErr;

	iRet								= getDataSetDims(_iDatasetId, &iRows, &iCols);
	if(iRet)
	{
		return false;
	}

	iRet								= getDatasetPrecision(_iDatasetId, &iPrec);
	if(iRet)
	{
		return false;
	}

	switch(iPrec)
	{
	case SCI_INT8 : 
		{
			char* pcData	= NULL;
			pcData = (char*)MALLOC(sizeof(char) * iRows * iCols);
			iRet = readInterger8Matrix(_iDatasetId, iRows, iCols, pcData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfInteger8(pvApiCtx, _pstVarname, iRows, iCols, pcData);
			}
			else
			{
				strErr = createMatrixOfInteger8InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pcData);
			}
		}
		break;
	case SCI_UINT8 : 
		{
			unsigned char* pucData	= NULL;
			pucData = (unsigned char*)MALLOC(sizeof(unsigned char) * iRows * iCols);
			iRet = readUnsignedInterger8Matrix(_iDatasetId, iRows, iCols, pucData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfUnsignedInteger8(pvApiCtx, _pstVarname, iRows, iCols, pucData);
			}
			else
			{
				strErr = createMatrixOfUnsignedInteger8InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pucData);
			}
		}
		break;
	case SCI_INT16 : 
		{
			short* psData	= NULL;
			psData = (short*)MALLOC(sizeof(short) * iRows * iCols);
			iRet = readInterger16Matrix(_iDatasetId, iRows, iCols, psData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfInteger16(pvApiCtx, _pstVarname, iRows, iCols, psData);
			}
			else
			{
				strErr = createMatrixOfInteger16InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, psData);
			}
		}
		break;
	case SCI_UINT16 : 
		{
			unsigned short* pusData	= NULL;
			pusData = (unsigned short*)MALLOC(sizeof(unsigned short) * iRows * iCols);
			iRet = readUnsignedInterger16Matrix(_iDatasetId, iRows, iCols, pusData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfUnsignedInteger16(pvApiCtx, _pstVarname, iRows, iCols, pusData);
			}
			else
			{
				strErr = createMatrixOfUnsignedInteger16InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, pusData);
			}
		}
		break;
	case SCI_INT32 : 
		{
			int* piData	= NULL;
			piData = (int*)MALLOC(sizeof(int) * iRows * iCols);
			iRet = readInterger32Matrix(_iDatasetId, iRows, iCols, piData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfInteger32(pvApiCtx, _pstVarname, iRows, iCols, piData);
			}
			else
			{
				strErr = createMatrixOfInteger32InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, piData);
			}
		}
		break;
	case SCI_UINT32 : 
		{
			unsigned int* puiData	= NULL;
			puiData = (unsigned int*)MALLOC(sizeof(unsigned int) * iRows * iCols);
			iRet = readUnsignedInterger32Matrix(_iDatasetId, iRows, iCols, puiData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfUnsignedInteger32(pvApiCtx, _pstVarname, iRows, iCols, puiData);
			}
			else
			{
				strErr = createMatrixOfUnsignedInteger32InNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, puiData);
			}
		}
		break;
	case SCI_INT64 : 
		{
#ifdef __SCILAB_INT64__
			long long* pllData	= NULL;
			pllData = (long long*)MALLOC(sizeof(long long) * iRows * iCols);
			iRet = readInterger64Matrix(_iDatasetId, iRows, iCols, pllData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfInteger64( _pstVarname, iRows, iCols, pllData);
			}
			else
			{
				strErr = createMatrixOfInteger64InNamedList(_pstVarname, _piAddress, _iItemPos, iRows, iCols, pllData);
			}
#else
			return false;
#endif
		}
		break;
	case SCI_UINT64 : 
		{
#ifdef __SCILAB_INT64__
			unsigned long long* pullData	= NULL;
			pullData = (unsigned long long*)MALLOC(sizeof(unsigned long long) * iRows * iCols);
			iRet = readUnsignedInterger64Matrix(_iDatasetId, iRows, iCols, pullData);
			if(iRet)
			{
				return false;
			}

			if(_piAddress == NULL)
			{
				strErr = createNamedMatrixOfUnsignedInteger64( _pstVarname, iRows, iCols, pullData);
			}
			else
			{
				strErr = createMatrixOfUnsignedInteger64InNamedList(_pstVarname, _piAddress, _iItemPos, iRows, iCols, pullData);
			}
#else
			return false;
#endif
		}
		break;
	default :
		return false;
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	return true;
}

static bool import_boolean(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet						= 0;
	int* piData					= NULL;
	int	iRows						= 0;
	int iCols						= 0;
	StrErr strErr;

	iRet				= getDataSetDims(_iDatasetId, &iRows, &iCols);
	if(iRet)
	{
		return false;
	}

	if(iRows * iCols != 0)
	{
		piData		= (int *) MALLOC(iRows * iCols * sizeof(int));
		iRet			= readBooleanMatrix(_iDatasetId, iRows, iCols, piData);
		if(iRet)
		{
			return false;
		}
	}

	if(_piAddress == NULL)
	{
		strErr = createNamedMatrixOfBoolean(pvApiCtx, _pstVarname, iRows, iCols, piData);
	}
	else //if not null this variable is in a list
	{
		strErr = createMatrixOfBooleanInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, piData);
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	char pstMsg[512];
	sprintf(pstMsg, "boolean (%d x %d)", iRows, iCols);
	print_tree(pstMsg);

	if(piData)
	{
		FREE(piData);
	}

	if(iRet)
	{
		return false;
	}

	return true;
}

static bool import_poly(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet						= 0;
	int i								= 0;
	int	iRows						= 0;
	int iCols						= 0;
	int iComplex				= 0;
	char pstVarName[64]	= {0};
	double **pdblReal		= NULL;
	double **pdblImg		= NULL;
	int *piNbCoef				= NULL;
	StrErr strErr;

	iRet				= getDataSetDims(_iDatasetId, &iRows, &iCols);
	if(iRet)
	{
		return false;
	}

	iComplex		= isComplexData(_iDatasetId);


	if(iComplex)
	{
		piNbCoef		= (int*)MALLOC(iRows * iCols * sizeof(int));
		pdblReal		= (double**)MALLOC(iRows * iCols * sizeof(double*));
		pdblImg			= (double**)MALLOC(iRows * iCols * sizeof(double*));
		iRet				= readPolyComplexMatrix(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
	}
	else
	{
		piNbCoef		= (int*)MALLOC(iRows * iCols * sizeof(int));
		pdblReal		= (double**)MALLOC(iRows * iCols * sizeof(double*));
		iRet				= readPolyMatrix(_iDatasetId, pstVarName, iRows, iCols, piNbCoef, pdblReal);
	}

	if(iRet)
	{
		return false;
	}

	if(_piAddress == NULL)
	{
		if(iComplex)
		{
			strErr = createNamedComplexMatrixOfPoly(pvApiCtx, _pstVarname, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
		}
		else
		{
			strErr = createNamedMatrixOfPoly(pvApiCtx, _pstVarname, pstVarName, iRows, iCols, piNbCoef, pdblReal);
		}
	}
	else //if not null this variable is in a list
	{
		if(iComplex)
		{
			strErr = createComplexMatrixOfPolyInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, iRows, iCols, piNbCoef, pdblReal, pdblImg);
		}
		else
		{
			strErr = createMatrixOfPolyInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, pstVarName, iRows, iCols, piNbCoef, pdblReal);
		}
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	char pstMsg[512];
	sprintf(pstMsg, "string (%d x %d)", iRows, iCols);
	print_tree(pstMsg);

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		FREE(pdblReal[i]);
	}
	FREE(pdblReal);
	FREE(piNbCoef);

	if(iRet)
	{
		return false;
	}

	return true;
}

static bool import_sparse(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet						= 0;
	int i								= 0;
	int	iRows						= 0;
	int iCols						= 0;
	int iComplex				= 0;
	double *pdblReal		= NULL;
	double *pdblImg			= NULL;
	int iNbItem					= 0;
	int* piNbItemRow		= NULL;
	int* piColPos				= NULL;
	StrErr strErr;

	iRet								= getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
	if(iRet)
	{
		return false;
	}

	iComplex						= isComplexData(_iDatasetId);


	if(iComplex)
	{
		piNbItemRow				= (int*)MALLOC(iRows * sizeof(int));
		piColPos					= (int*)MALLOC(iNbItem * sizeof(int));
		pdblReal					= (double*)MALLOC(iNbItem * sizeof(double));
		pdblImg						= (double*)MALLOC(iNbItem * sizeof(double));
		iRet							= readSparseComplexMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
	}
	else
	{
		piNbItemRow				= (int*)MALLOC(iRows * sizeof(int));
		piColPos					= (int*)MALLOC(iNbItem * sizeof(int));
		pdblReal					= (double*)MALLOC(iNbItem * sizeof(double));
		iRet							= readSparseMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
	}

	if(iRet)
	{
		return false;
	}

	if(_piAddress == NULL)
	{
		if(iComplex)
		{
			strErr = createNamedComplexSparseMatrix(pvApiCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
		}
		else
		{
			strErr = createNamedSparseMatrix(pvApiCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
		}
	}
	else //if not null this variable is in a list
	{
		if(iComplex)
		{
			strErr = createComplexSparseMatrixInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal, pdblImg);
		}
		else
		{
			strErr = createSparseMatrixInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal);
		}
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	char pstMsg[512];
	sprintf(pstMsg, "sparse (%d x %d)", iRows, iCols);
	print_tree(pstMsg);

	FREE(piNbItemRow);
	FREE(piColPos);
	FREE(pdblReal);
	if(iComplex)
	{
		FREE(pdblImg);
	}


	if(iRet)
	{
		return false;
	}

	return true;
}

static bool import_boolean_sparse(int _iDatasetId, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet						= 0;
	int i								= 0;
	int	iRows						= 0;
	int iCols						= 0;
	int iNbItem					= 0;
	int* piNbItemRow		= NULL;
	int* piColPos				= NULL;
	StrErr strErr;

	iRet								= getSparseDimension(_iDatasetId, &iRows, &iCols, &iNbItem);
	if(iRet)
	{
		return false;
	}


	piNbItemRow				= (int*)MALLOC(iRows * sizeof(int));
	piColPos					= (int*)MALLOC(iNbItem * sizeof(int));
	iRet							= readBooleanSparseMatrix(_iDatasetId, iRows, iCols, iNbItem, piNbItemRow, piColPos);
	if(iRet)
	{
		return false;
	}

	if(_piAddress == NULL)
	{
		strErr					= createNamedBooleanSparseMatrix(pvApiCtx, _pstVarname, iRows, iCols, iNbItem, piNbItemRow, piColPos);
	}
	else //if not null this variable is in a list
	{
		strErr					= createBooleanSparseMatrixInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows, iCols, iNbItem, piNbItemRow, piColPos);
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	char pstMsg[512];
	sprintf(pstMsg, "boolean sparse (%d x %d)", iRows, iCols);
	print_tree(pstMsg);

	FREE(piNbItemRow);
	FREE(piColPos);


	if(iRet)
	{
		return false;
	}

	return true;
}

static bool import_list(int _iDatasetId, int _iVarType, int _iItemPos, int* _piAddress, char* _pstVarname)
{
	int iRet								= 0;
	int i										= 0;
	int	iRows								= 0;
	int iCols								= 0;
	int* piListAddr					= NULL;
	hobj_ref_t* piItemRef		= NULL;
	StrErr strErr;

	iRet				= getDataSetDims(_iDatasetId, &iRows, &iCols);
	if(iRet)
	{
		return false;
	}


	if(iRows * iCols == 0)
	{//special case for empty list
		iRows = 0;
		iCols = 0;
	}
	else
	{
		iRet			= getListItemReferences(_iDatasetId, &piItemRef);
		if(iRet)
		{
			return false;
		}
	}

	char pstMsg[512];
	sprintf(pstMsg, "list (%d x %d)", iRows, iCols);
	print_tree(pstMsg);

	if(_piAddress == 0)
	{
		switch(_iVarType)
		{
		case sci_list :
			strErr	= createNamedList(pvApiCtx, _pstVarname, iRows * iCols, &piListAddr);
			break;
		case sci_tlist :
			strErr	= createNamedTList(pvApiCtx, _pstVarname, iRows * iCols, &piListAddr);
			break;
		case sci_mlist :
			strErr	= createNamedMList(pvApiCtx, _pstVarname, iRows * iCols, &piListAddr);
			break;
		default :
			return false;
		}
	}
	else //if not null this variable is in a list
	{
		switch(_iVarType)
		{
		case sci_list :
			strErr	= createListInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows * iCols, &piListAddr);
			break;
		case sci_tlist :
			strErr	= createTListInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows * iCols, &piListAddr);
			break;
		case sci_mlist :
			strErr	= createMListInNamedList(pvApiCtx, _pstVarname, _piAddress, _iItemPos, iRows * iCols, &piListAddr);
			break;
		default :
			return false;
		}
	}

	if(strErr.iErr)
	{
			printError(&strErr, 0);
			return false;
	}

	iTab++;
	for(i = 0 ; i < iRows * iCols ; i++)
	{
		int iItemDataset = 0;
		iRet			= getListItemDataset(_iDatasetId, piItemRef, i, &iItemDataset);
		if(iRet || iItemDataset == 0)
		{
			return false;
		}

		bool bRet = import_data(iItemDataset, i + 1, piListAddr, _pstVarname);
		if(bRet == false)
		{
			return false;
		}
	}
	iTab--;

	iRet				= deleteListItemReferences(_iDatasetId, piItemRef);
	if(iRet)
	{
		return false;
	}

	return true;
}

void print_tree(char* _pstMsg)
{
#ifdef PRINT_DEBUG
	for(int i = 0 ; i < iTab ; i++)
	{
		printf("\t");
	}
	printf("%s\n", _pstMsg);
#endif
}
/*--------------------------------------------------------------------------*/
