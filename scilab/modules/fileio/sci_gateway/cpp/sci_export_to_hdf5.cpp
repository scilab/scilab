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
#include <string.h>
#include <stdio.h>
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_variable.h"
#include "h5_fileManagement.h"
#include "h5_writeDataToFile.h"
#include "freeArrayOfString.h"
}

//#define PRINT_DEBUG
int iLevel = 0;

bool export_data(int _iH5File, int *_piVar, char* _pstName);
bool export_list(int _iH5File, int *_piVar, char* _pstName, int _iVarType);
bool export_double(int _iH5File, int *_piVar, char* _pstName);
bool export_poly(int _iH5File, int *_piVar, char* _pstName);
bool export_boolean(int _iH5File, int *_piVar, char* _pstName);
bool export_sparse(int *_piVar, char* _pstName);
bool export_boolean_sparse(int *_piVar, char* _pstName);
bool export_matlab_sparse(int *_piVar, char* _pstName);
bool export_ints(int *_piVar, char* _pstName);
bool export_handles(int *_piVar, char* _pstName);
bool export_strings(int _iH5File, int *_piVar, char* _pstName);
bool export_u_function(int *_piVar, char* _pstName);
bool export_c_function(int *_piVar, char* _pstName);
bool export_lib(int *_piVar, char* _pstName);
bool export_lufact_pointer(int *_piVar, char* _pstName);

void print_type(char* _pstType);
int extractVarNameList(int _iStart, int _iEnd, char** _pstNameList);

#ifdef _MSC_VER
#define strdup _strdup
#endif

/*--------------------------------------------------------------------------*/
int sci_export_to_hdf5(char *fname,unsigned long fname_len)
{
	CheckRhs(2,1000000);//input parameters
	CheckLhs(1,1);//output parameter

	int iRet						= 0;
	int iNbVar					= 0;
	int iLen						= 0;

	int** piAddrList		= NULL;
	char* pstFilename		= NULL;
	char** pstNameList	= NULL;

	/*get input data*/
	pstNameList = (char**)MALLOC(sizeof(char*) * Rhs);
	iNbVar = extractVarNameList(1, Rhs, pstNameList);

	piAddrList = (int**)MALLOC(sizeof(int*) * (iNbVar - 1));
	for(int i = 0 ; i < Rhs - 1 ; i++)
	{
		iRet = getVarAddressFromName(pstNameList[i + 1], (int)strlen(pstNameList[i + 1]), &piAddrList[i]);
		if(iRet)
		{
			Scierror(999,_("%s: Wrong value for input argument #%d: Defined variable expected.\n"), fname, i + 1);
			return 0;
		}
	}

	iLevel = 0;
	//open hdf5 file
	int iH5File = createHDF5File(pstNameList[0]); 

	// export data
	bool bExport = false;
	for(int i = 0 ; i < Rhs - 1; i++)
	{
		bExport = export_data(iH5File, piAddrList[i], pstNameList[i + 1]);
		if(bExport == false)
		{
			break;
		}
	}

	//close hdf5 file
	closeHDF5File(iH5File);

	int *piReturn = NULL;
	allocMatrixOfBoolean(Rhs + 1, 1, 1, &piReturn);
	if(bExport == true)
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

bool export_data(int _iH5File, int* _piVar, char* _pstName)
{
	bool bReturn = false;
	int iType = getVarType(_piVar);
	switch(iType)
	{
	case sci_matrix :
		{
			bReturn = export_double(_iH5File, _piVar, _pstName);
			break;
		}
	case sci_poly :
		{
			bReturn = export_poly(_iH5File, _piVar, _pstName);
			break;
		}
	case sci_boolean :
		{
			bReturn = export_boolean(_iH5File, _piVar, _pstName);
			break;
		}
	case sci_sparse :
		{
			bReturn = export_sparse(_piVar, _pstName);
			break;
		}
	case sci_boolean_sparse :
		{
			bReturn = export_boolean_sparse(_piVar, _pstName);
			break;
		}
	case sci_matlab_sparse :
		{
			bReturn = export_matlab_sparse(_piVar, _pstName);
			break;
		}
	case sci_ints :
		{
			bReturn = export_ints(_piVar, _pstName);
			break;
		}
	case sci_handles :
		{
			bReturn = export_handles(_piVar, _pstName);
			break;
		}
	case sci_strings :
		{
			bReturn = export_strings(_iH5File, _piVar, _pstName);
			break;
		}
	case sci_u_function :
		{
			bReturn = export_u_function(_piVar, _pstName);
			break;
		}
	case sci_c_function :
		{
			bReturn = export_c_function(_piVar, _pstName);
			break;
		}
	case sci_lib :
		{
			bReturn = export_lib(_piVar, _pstName);
			break;
		}
	case sci_list :
	case sci_tlist :
	case sci_mlist :
		{
			bReturn = export_list(_iH5File, _piVar, _pstName, iType);
			break;
		}
	case sci_lufact_pointer :
		{
			bReturn = export_lufact_pointer(_piVar, _pstName);
			break;
		}
	}
	return bReturn;
}

bool export_list(int _iH5File, int *_piVar, char* _pstName, int _iVarType)
{
	bool bReturn = false;
	int iItemNumber = 0;
	getListItemNumber(_piVar, &iItemNumber);

	//create groupe name
	char* pstGroupName	= strdup(_pstName);
	char* pstSlash			= strstr(pstGroupName, "/");
	if(pstSlash != NULL)
	{
		pstSlash[0]					= '_';
	}

	char pstMsg[256];
	sprintf(pstMsg, "list (%d)", iItemNumber);
	print_type(pstMsg);

	iLevel++;
	//open list
	void *pvList = openList(_iH5File, pstGroupName, iItemNumber);
	for(int i = 0 ; i < iItemNumber ; i++)
	{
		int *piNewVar = NULL;
		getListItemAddress(_piVar, i + 1, &piNewVar);//1 indexed
		if(piNewVar == NULL)
		{
			Scierror(999,"Item not found");
			return 0;
		}

		char* pstName	= NULL;
		pstName				= (char*)MALLOC(((int)log10((double)(i+1)) + 4) * sizeof(char)); //1 for null termination, 1 for round value, 2 for '#' characters
		sprintf(pstName, "#%d#", i);

		char *pstPathName	= NULL;
		pstPathName				= (char*)MALLOC((strlen(pstGroupName) + strlen(pstName) + 4) * sizeof(char)); //1 for null termination, 1 for separator, 2 for '#' characters

		sprintf(pstPathName, "#%s#/%s", pstGroupName, pstName);
		bReturn				= export_data(_iH5File, piNewVar, pstPathName);
		addItemInList(_iH5File, pvList, i, pstPathName);

		FREE(pstName);
		FREE(pstPathName);
		if(bReturn == false)
			return false;
	}
	iLevel--;
	closeList(_iH5File, pvList, _pstName, iItemNumber, _iVarType);
	free(pstGroupName);
	//close list
	return true;
}

bool export_double(int _iH5File, int *_piVar, char* _pstName)
{
	int iType = getVarType(_piVar);
	if(iType != sci_matrix)
	{
		return false;
	}

	int iComplex			= isVarComplex(_piVar);
	int iRows					= 0;
	int iCols					= 0;
	double *pdblReal	= NULL;
	double *pdblImg		= NULL;

	if(iComplex)
	{
		getComplexMatrixOfDouble(_piVar, &iRows, &iCols, &pdblReal, &pdblImg);
	}
	else
	{
		getMatrixOfDouble(_piVar, &iRows, &iCols, &pdblReal);
		writeDoubleMatrix(_iH5File, _pstName, pdblReal, iRows, iCols);
	}

	char pstMsg[512];
	sprintf(pstMsg, "double (%d x %d)", iRows, iCols);
	print_type(pstMsg);
	return true;
}

bool export_poly(int _iH5File, int *_piVar, char* _pstName)
{
	int iRows						= 0;
	int iCols						= 0;
	int* piNbCoef				= NULL;
	double** pdblData		= NULL;
	char pstVarName[64]	= {0};
	int iVarNameLen			= 0;

	getPolyVariableName(_piVar, pstVarName, &iVarNameLen);

	getMatrixOfPoly(_piVar, &iRows, &iCols, NULL, NULL);
	piNbCoef = (int*)MALLOC(iRows * iCols * sizeof(int));
	getMatrixOfPoly(_piVar, &iRows, &iCols, piNbCoef, NULL);
	pdblData = (double**)MALLOC(sizeof(double*) * iRows * iCols);
	for(int i = 0 ; i < iRows * iCols ; i++)
	{
		pdblData[i] = (double*)MALLOC(sizeof(double) * piNbCoef[i]);// for null termination
	}
	getMatrixOfPoly(_piVar, &iRows, &iCols, piNbCoef, pdblData);

	writePolyMatrix(_iH5File, _pstName, pstVarName, iRows, iCols, piNbCoef, pdblData);

	char pstMsg[512];
	sprintf(pstMsg, "poly (%d x %d)", iRows, iCols);
	print_type(pstMsg);

	for(int i = 0 ; i < iRows * iCols ; i++)
	{
		FREE(pdblData[i]);
	}

	FREE(pdblData);
	FREE(piNbCoef);
	return true;
}

bool export_boolean(int _iH5File, int *_piVar, char* _pstName)
{
	int iRows					= 0;
	int iCols					= 0;
	int *piData				= NULL;

	getMatrixOfBoolean(_piVar, &iRows, &iCols, &piData);
	writeBooleanMatrix(_iH5File, _pstName, piData, iRows, iCols);

	char pstMsg[512];
	sprintf(pstMsg, "bool (%d x %d)", iRows, iCols);
	print_type(pstMsg);
	return true;
}

bool export_sparse(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_boolean_sparse(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_matlab_sparse(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_ints(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_handles(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_strings(int _iH5File, int *_piVar, char* _pstName)
{
	int iRows				= 0;
	int iCols				= 0;
	int* piLen			= NULL;
	char** pstData	= NULL;


	getMatrixOfString(_piVar, &iRows, &iCols, NULL, NULL);
	piLen = (int*)MALLOC(iRows * iCols * sizeof(int));
	getMatrixOfString(_piVar, &iRows, &iCols, piLen, NULL);
	pstData = (char**)MALLOC(sizeof(char*) * iRows * iCols);
	for(int i = 0 ; i < iRows * iCols ; i++)
	{
		pstData[i] = (char*)MALLOC(sizeof(char) * (piLen[i] + 1));// for null termination
	}
	getMatrixOfString(_piVar, &iRows, &iCols, piLen, pstData);

	writeStringMatrix(_iH5File, _pstName, pstData, iRows, iCols);

	char pstMsg[512];
	sprintf(pstMsg, "string (%d x %d)", iRows, iCols);
	print_type(pstMsg);

	freeArrayOfString(pstData, iRows * iCols);
	return true;
}

bool export_u_function(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_c_function(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_lib(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_lufact_pointer(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

void print_type(char* _pstType)
{
#ifdef PRINT_DEBUG
	for(int i = 0 ; i < iLevel ; i++)
	{
		sciprint("\t");
	}
	sciprint("%s\n", _pstType);
#endif
}

int extractVarNameList(int _iStart, int _iEnd, char** _pstNameList)
{
	int iCount = 0;

	for(int i = _iStart ; i <= _iEnd ; i++)
	{
		int iRows					= 0;
		int iCols					= 0;
		int iLen					= 0;
		int* piAddr				= NULL;

		getVarAddressFromPosition(i, &piAddr);
		//get filename
		if(getVarType(piAddr) != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: A string.\n"), "export_to_hdf5", i);
			return 0;
		}

		getVarDimension(piAddr, &iRows, &iCols);
		if(iRows != 1 || iCols != 1)
		{
			Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"), "export_to_hdf5", i);
			return 0;
		}
		
		getMatrixOfString(piAddr, &iRows, &iCols, &iLen, NULL);
		_pstNameList[iCount] = (char*)MALLOC((iRows * iCols + 1) * sizeof(char));//1 for null termination
		getMatrixOfString(piAddr, &iRows, &iCols, &iLen, &_pstNameList[iCount]);
		iCount++;
	}
	return iCount;
}

/*--------------------------------------------------------------------------*/
