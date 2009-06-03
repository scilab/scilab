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
	#include "variable_api.h"
	#include "h5_fileManagement.h"
	#include "h5_writeDataToFile.h"
}

bool export_data(int _iH5File, int *_piVar, char* _pstName);
bool export_list(int *_piVar, char* _pstName);
bool export_double(int _iH5File, int *_piVar, char* _pstName);
bool export_poly(int *_piVar, char* _pstName);
bool export_boolean(int *_piVar, char* _pstName);
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
int iTab;
/*--------------------------------------------------------------------------*/
int sci_export_to_hdf5(char *fname,unsigned long fname_len)
{
	//Real code
	CheckRhs(3,3);//two input parameters
	CheckLhs(1,1);//one output parameter

	int iLen						= 0;
	int iRows						= 0;
	int iCols						= 0;

	int* piAddr2				= NULL;
	int* piAddr3				= NULL;
	int* piAddrReturn		= NULL;
	char *pstVarName		= NULL;
	char *pstFilename		= NULL;

	int *piVar					= NULL;

	/*get input data*/
	getVarAddressFromNumber(1, &piVar);
	getVarAddressFromNumber(2, &piAddr2);
	getVarAddressFromNumber(3, &piAddr3);

	if(getVarType(piAddr2) != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string.\n"),fname, 2);
		return 0;
	}

	if(getVarType(piAddr3) != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string.\n"),fname, 2);
		return 0;
	}

	//get variable name
	getVarDimension(piAddr2, &iRows, &iCols);
	if(iRows != 1 || iCols != 1)
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
	}

	getMatrixOfString(piAddr2, &iRows, &iCols, &iLen, NULL);
	pstVarName = (char*)MALLOC((iRows * iCols + 1) * sizeof(char));
	getMatrixOfString(piAddr2, &iRows, &iCols, &iLen, &pstVarName);

	//get filename
	getVarDimension(piAddr3, &iRows, &iCols);
	if(iRows != 1 || iCols != 1)
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
	}

	getMatrixOfString(piAddr3, &iRows, &iCols, &iLen, NULL);
	pstFilename = (char*)MALLOC((iRows * iCols + 1) * sizeof(char));//1 for null termination
	getMatrixOfString(piAddr3, &iRows, &iCols, &iLen, &pstFilename);

	iTab = 0;

	//open hdf5 file
	int iH5File = openHDF5File(pstFilename); 

	// export data
	bool bExport = export_data(iH5File, piVar, pstVarName);

	//close hdf5 file
	closeHDF5File(iH5File);

	int *piReturn = NULL;
	allocMatrixOfBoolean(Rhs + 1, 1, 1, &piReturn, &piAddrReturn);
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
			bReturn = export_poly(_piVar, _pstName);
			break;
		}
	case sci_boolean :
		{
			bReturn = export_boolean(_piVar, _pstName);
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
			bReturn = export_list(_piVar, _pstName);
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

bool export_list(int *_piVar, char* _pstName)
{
	bool bReturn = false;
	int iItemNumber = 0;
	getListItemNumber(_piVar, &iItemNumber);

	//open list
	//create groupe
	char* pstGroupName	= strdup(_pstName);
	char* pstSlash			= strstr(pstGroupName, "/");
	if(pstSlash != NULL)
	{
		pstSlash[0]					= '_';
	}

	char pstMsg[256];
	sprintf(pstMsg, "%s (list of %d items)", pstGroupName, iItemNumber);
	print_type(pstMsg);

	iTab++;
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
		pstName				= (char*)MALLOC((
			(int)log10((double)(i+1)) + 4) * sizeof(char)); //1 for null termination, 1 for round value, 2 for '#' characters
		sprintf(pstName, "#%d#", i);

		char *pstPathName	= NULL;
		pstPathName				= (char*)MALLOC((strlen(pstGroupName) + strlen(pstName) + 4) * sizeof(char)); //1 for null termination, 1 for separator, 2 for '#' characters

		sprintf(pstPathName, "#%s#/%s", pstGroupName, pstName);
		//bReturn				= export_data(piNewVar, pstPathName);

//		print_type(pstGroupName);
		FREE(pstName);
		FREE(pstPathName);
		//create ref
		//add ref in list
		if(bReturn == false)
			return false;
	}
	free(pstGroupName);
	//close list
	iTab--;
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

	char pstMsg[256];
	sprintf(pstMsg, "%s (double matrix : %d x %d)", _pstName, iRows, iCols);
	print_type(pstMsg);
	return true;
}

bool export_poly(int *_piVar, char* _pstName)
{
	print_type(_pstName);
	return true;
}

bool export_boolean(int *_piVar, char* _pstName)
{
	print_type(_pstName);
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

	char pstMsg[256];
	sprintf(pstMsg, "%s (string matrix : %d x %d)", _pstName, iRows, iCols);
	print_type(pstMsg);

	for(int i = 0 ; i < iRows * iCols ; i++)
	{
		FREE(pstData[i]);
	}
	FREE(pstData);
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
	char szOut[128] = {0};
	for(int i = 0 ; i < iTab ; i++)
	{
		strcat(szOut, "\t");
	}
	strcat(szOut, _pstType);
	sciprint("%s\n", szOut);
}

/*--------------------------------------------------------------------------*/
