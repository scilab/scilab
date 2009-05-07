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
	#include "gw_io.h"
	#include "stack-c.h"
	#include "MALLOC.h"
	#include "Scierror.h"
	#include "localization.h"
	#include "sciprint.h"
	#include "variable_api.h"
	#include "../../call_scilab/includes/CallScilab.h"
}

int iGetDoubleFromPointer(int* _piAddr, int *_piRows, int *_piCols, double** _pdblReal);
int iGetComplexDoubleFromPointer(int* _piAddr, int *_piRows, int *_piCols, double** _pdblReal, double** _pdblImg);
int iGetPolyFromPointer(int* _piAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, double** _pdblReal);
int iGetComplexPolyFromPointer(int* _piAddr, int** _piVarName, int* _piRows, int* _piCols, int* _piPow, double** _pdblReal, double** _pdblImg);
int iGetBooleanFromPointer(int* _piAddr, int *_piRows, int *_piCols, int** _piBool);//1 -> %t, 0 -> %f
int iGetSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal);
int iGetComplexSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal, double **_pdblImg);
int iGetBooleanSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, int** _piBool);
int iGetMatlabSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal);
int iGetComplexMatlabSparseFromPointer(int* _piAddr, int *_piRows, int *_piCols, int* _piTotalElem, int* _piElemByRow, double **_pdblReal, double **_pdblImg);
int iGetIntFromPointer(int* _piAddr, int *_piRows, int *_piCols, int *_piPrecision, int** _piInt);
int iGetHandleFromPointer(int* _piAddr, int *_piRows, int *_piCols, int** _piHandle);

bool export_data(int *_piVar);
bool export_list(int *_piVar);
bool export_double(int *_piVar);
bool export_poly(int *_piVar);
bool export_boolean(int *_piVar);
bool export_sparse(int *_piVar);
bool export_boolean_sparse(int *_piVar);
bool export_matlab_sparse(int *_piVar);
bool export_ints(int *_piVar);
bool export_handles(int *_piVar);
bool export_strings(int *_piVar);
bool export_u_function(int *_piVar);
bool export_c_function(int *_piVar);
bool export_lib(int *_piVar);
bool export_lufact_pointer(int *_piVar);

int iListGetNode(int _iVar, int *_piParent, int _iItem, int** _piNode);
int iListGetItemType(int *_piItem);
int iGetVarAddress(int _iVar, int **_piAddress);
int GetVarTypeFromAddress(int* _piVar);

void print_type(char* _pstType);
int iTab;
/*--------------------------------------------------------------------------*/
int sci_export_to_hdf5(char *fname,unsigned long fname_len)
{
	int i								= 0;
	int *piAddr					= NULL;
	int iRows						= 0;
	int iCols						= 0;

	int iTotalItem			= 0;
	int* piNbItemRow		= NULL;
	int* piColPos				= NULL;


	double *pdblReal		= NULL;
	double *pdblImg			= NULL;

	Rhs = Max(Rhs,0);

	getVarAddressFromNumber(1, &piAddr);

	getComplexSparseMatrix(piAddr, &iRows, &iCols, &iTotalItem, &piNbItemRow, &piColPos, &pdblReal, &pdblImg);


	//LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

	//Real code
	//CheckRhs(2,2);//two input parameters
	//CheckLhs(1,1);//one output parameter

	//char *pstData = NULL;
	//int *piLen		= NULL;
	//int iRows			= 0;
	//int iCols			= 0;
	//int iLen			= 0;
	//if(GetType(2) != sci_strings)
	//{
	//	Scierror(999,_("%s: Wrong type for input argument #%d: A string.\n"),fname, 2);
	//	return 0;
	//}

	////get filename
	//GetRhsStringVar(2, &iRows, &iCols, piLen, pstData);
	//piLen = (int*)MALLOC(sizeof(int) * iRows * iCols);
	//GetRhsStringVar(2, &iRows, &iCols, piLen, pstData);
	//iLen = iArraySum(piLen, 0, iRows * iCols) + 1; // +1 for null terminated
	//pstData = (char*)MALLOC(sizeof(char) * iLen);
	//GetRhsStringVar(2, &iRows, &iCols, piLen, pstData);

	//int *piVar = NULL;
	//getVarAddressFromNumber(1, &piVar);
	//iTab = 0;
	//bool bExport = export_data(piVar);

	//int *piReturn = NULL;
	//iAllocMatrixOfBoolean(Rhs + 1, 1, 1, &piReturn);
	//if(bExport == true)
	//{
	//	piReturn[0] = 1;
	//}
	//else
	//{
	//	piReturn[0] = 0;
	//}

	//LhsVar(1) = Rhs + 1;
	//PutLhsVar();

bool export_data(int* _piVar)
{
	bool bReturn = false;
	int iType = GetVarTypeFromAddress(_piVar);
	switch(iType)
	{
	case sci_matrix :
		{
			bReturn = export_double(_piVar);
			break;
		}
	case sci_poly :
		{
			bReturn = export_poly(_piVar);
			break;
		}
	case sci_boolean :
		{
			bReturn = export_boolean(_piVar);
			break;
		}
	case sci_sparse :
		{
			bReturn = export_sparse(_piVar);
			break;
		}
	case sci_boolean_sparse :
		{
			bReturn = export_boolean_sparse(_piVar);
			break;
		}
	case sci_matlab_sparse :
		{
			bReturn = export_matlab_sparse(_piVar);
			break;
		}
	case sci_ints :
		{
			bReturn = export_ints(_piVar);
			break;
		}
	case sci_handles :
		{
			bReturn = export_handles(_piVar);
			break;
		}
	case sci_strings :
		{
			bReturn = export_strings(_piVar);
			break;
		}
	case sci_u_function :
		{
			bReturn = export_u_function(_piVar);
			break;
		}
	case sci_c_function :
		{
			bReturn = export_c_function(_piVar);
			break;
		}
	case sci_lib :
		{
			bReturn = export_lib(_piVar);
			break;
		}
	case sci_list :
	case sci_tlist :
	case sci_mlist :
		{
			bReturn = export_list(_piVar);
			break;
		}
	case sci_lufact_pointer :
		{
			bReturn = export_lufact_pointer(_piVar);
			break;
		}
	}
	return bReturn;
}

bool export_list(int *_piVar)
{
	bool bReturn = false;
	int iItemNumber = 0;
	iGetListItemType(1, _piVar, &iItemNumber, NULL);

	print_type("sci_list");
	iTab++;
	for(int i = 0 ; i < iItemNumber ; i++)
	{
		int *piNewVar = NULL;
		piNewVar = iGetAddressFromItemPos(_piVar, i);
		if(piNewVar == NULL)
		{
			Scierror(999,"Item not found");
			return 0;
		}
		bReturn = export_data(piNewVar);
		if(bReturn == false)
			return false;
	}
	iTab--;
	return true;
}


int iListGetNode(int _iVar, int *_piParent, int _iItem, int** _piNode)
{
	if(_piParent == NULL)
	{//Root
		int iAddrBase	=	iadr(*Lstk(Top - Rhs + _iVar));
		int iValType	= *istk(iAddrBase);

		if(iValType < 0)
		{
			iAddrBase		=	iadr(*istk(iAddrBase + 1));
		}

		*_piNode	= istk(iAddrBase);		
	}
	else
	{//iItem
		if(_iItem == 0)
		{
			*_piNode = _piParent;
		}
		else
		{
			*_piNode = iGetAddressFromItemPos(_piParent, _iItem);
		}
	}
	return 0;
}

int iGetVarAddress(int _iVar, int **_piAddress)
{
	int iAddress = iadr(*Lstk(Top - Rhs + _iVar));
	if (*istk(iAddress ) < 0)
		iAddress = iadr(*istk(iAddress +1));

	*_piAddress = istk(iAddress);
	return 0;
}

int GetVarTypeFromAddress(int* _piVar)
{
	if(_piVar == NULL)
	{
		return 0;
	}
	return _piVar[0];
}

int iListGetItemType(int *_piItem)
{
	if(_piItem == NULL)
	{
		return 0;
	}
	return _piItem[0];
}

bool export_double(int *_piVar)
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
	}
 
	char sz[32];
	sprintf(sz,"sci_matrix (%d,%d)", iRows, iCols); 
	print_type(sz);
	return true;
}

bool export_poly(int *_piVar)
{
	print_type("sci_poly");
	return true;
}

bool export_boolean(int *_piVar)
{
	print_type("sci_boolean");
	return true;
}

bool export_sparse(int *_piVar)
{
	print_type("sci_sparse");
	return true;
}

bool export_boolean_sparse(int *_piVar)
{
	print_type("sci_boolean_sparse");
	return true;
}

bool export_matlab_sparse(int *_piVar)
{
	print_type("sci_matlab_sparse");
	return true;
}

bool export_ints(int *_piVar)
{
	print_type("sci_ints");
	return true;
}

bool export_handles(int *_piVar)
{
	print_type("sci_handles");
	return true;
}

bool export_strings(int *_piVar)
{
	print_type("sci_strings");
	return true;
}

bool export_u_function(int *_piVar)
{
	print_type("sci_u_function");
	return true;
}

bool export_c_function(int *_piVar)
{
	print_type("sci_c_function");
	return true;
}

bool export_lib(int *_piVar)
{
	print_type("sci_lib");
	return true;
}

bool export_lufact_pointer(int *_piVar)
{
	print_type("sci_lufact_pointer");
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
