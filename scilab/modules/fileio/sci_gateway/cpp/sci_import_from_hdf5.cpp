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
#include "gw_fileio.h"
#include "stack-c.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "variable_api.h"
#include "../../call_scilab/includes/CallScilab.h"
#include "h5_fileManagement.h"
#include "h5_readDataFromFile.h"
}

int iTab = 0;


void prtinf_tree(char* _pstMsg);

int	import_data(int _iDatasetId, int _iItemPos, int* _piAddress);
int import_double(int _iDatasetId, int _iItemPos, int* _piAddress);
int import_string(int _iDatasetId, int _iItemPos, int* _piAddress);
int import_list(int _iDatasetId, int _iVarType, int _iItemPos, int* _piAddress);

int sci_import_from_hdf5(char *fname,unsigned long fname_len)
{
	int i1 = *Lstk(Rhs) - *Lstk(Rhs - 1);
	sciprint("1 -- diff : %d\n", i1);

	//int* piAddr = NULL;
	//double pdblData[] = {1,2,3,4,5,6,7,8,
	//											1,2,3,4,5,6,7,8,
	//											1,2,3,4,5,6,7,8,
	//											1,2,3,4,5,6,7,8,
	//											1,2,3,4,5,6,7,8,
	//											1,2,3,4,5,6,7,8,
	//											1,2,3,4,5,6,7,8,
	//											1,2,3,4,5,6,7,8};

	//createMatrixOfDouble(Rhs + 1, 8, 8, pdblData, &piAddr);

	//int piBool[] = {1,0,1,0,1,0,1,0,
	//								1,0,1,0,1,0,1,0,
	//								1,0,1,0,1,0,1,0,
	//								1,0,1,0,1,0,1,0,
	//								1,0,1,0,1,0,1,0,
	//								1,0,1,0,1,0,1,0,
	//								1,0,1,0,1,0,1,0,
	//								1,0,1,0,1,0,1,0};

	//int* piAddr = NULL;
	//createMatrixOfBoolean(Rhs + 1, 8, 8, piBool, &piAddr);

	//char **pstData = NULL;
	//pstData = (char**)malloc(sizeof(char*) * 8);
	//for(int i = 0 ; i < 8 ; i++)
	//{
	//	pstData[i] = (char*)malloc(sizeof(char) * 9);
	//	strcpy(pstData[i], "12345678");
	//}
	//createMatrixOfString(Rhs + 1, 8, 1, pstData, &piAddr);

	//int* piAddrRoot			= NULL;
	//createList(Rhs + 1, 2, &piAddrRoot);

	//int* piAddr1				= NULL;
	//createListInList(Rhs + 1, piAddrRoot, 1, 1, &piAddr1);

	//int* piAddr11				= NULL;
	//createListInList(Rhs + 1, piAddr1, 1, 1, &piAddr11);

	//int* piAddr111			= NULL;
	//double iVal					= 1;
	//createMatrixOfDoubleInList(Rhs + 1, piAddr11, 1, 1, 1, &iVal);

	//createMatrixOfDoubleInList(Rhs + 1, piAddrRoot, 2, 1, 1, &iVal);

	//int* piAddr					= NULL;

	//int iRows						= 0;
	//int iCols						= 0;

	//int iNbItem					= 0;
	//int* piNbItemRow		= NULL;
	//int* piColPos				= NULL;

	//double* pdblReal		= NULL;

	//getVarAddressFromNumber(1, &piAddr);
	//getSparseMatrix(piAddr, &iRows, &iCols, &iNbItem, &piNbItemRow, &piColPos, &pdblReal);

	//double *pdblReal2  = NULL;
	//pdblReal2 = (double*)malloc(sizeof(double) * iNbItem);

	//for(int i = 0 ; i < iNbItem ; i++)
	//{
	//	pdblReal2[i] = pdblReal[i] * 2;
	//}

	//createSparseMatrix(Rhs + 1, iRows, iCols, iNbItem, piNbItemRow, piColPos, pdblReal2, &piAddr);

	//char** pstA = (char**)malloc(sizeof(char*) * 4);
	//for(int i = 0 ; i < 4 ; i++)
	//{
	//	pstA[i] = (char*)malloc(sizeof(char) * 5);
	//	strcpy(pstA[i], "1234");
	//}

	//double* pdblB = (double*)malloc(sizeof(double) * 4);
	//for(int i = 0 ; i < 4 ; i++)
	//{
	//	pdblB[i] = i + 1;
	//}

	//int* piListRoot				= NULL;
	//createList(Rhs + 1, 3, &piListRoot);

	//	int *piAddr1				= NULL;
	//	createMatrixOfStringInList(Rhs + 1, piListRoot, 1, 2, 2, pstA, &piAddr1);
	//	//createMatrixOfDoubleInList(Rhs + 1, piListRoot, 1, 2, 2, pdblB);

	//	int* piList2				= NULL;
	//	createListInList(Rhs + 1, piListRoot, 2, 2, &piList2);

	//		//createMatrixOfDoubleInList(Rhs + 1, piList2, 1, 2, 2, pdblB);
	//		int *piAddr21			= NULL;
	//		createMatrixOfStringInList(Rhs + 1, piList2, 1, 2, 2, pstA, &piAddr21);
	//		//int *piAddr22			= NULL;
	//		//createMatrixOfStringInList(Rhs + 1, piList2, 2, 2, 2, pstA, &piAddr22);
	//		createMatrixOfDoubleInList(Rhs + 1, piList2, 2, 2, 2, pdblB);

	//	int* piList3				= NULL;
	//	createListInList(Rhs + 1, piListRoot, 3, 1, &piList3);

	//		int* piList31			= NULL;
	//		createListInList(Rhs + 1, piList3, 1, 2, &piList31);

	//			//createMatrixOfDoubleInList(Rhs + 1, piList31, 1, 2, 2, pdblB);
	//			int *piAddr311	= NULL;
	//			createMatrixOfStringInList(Rhs + 1, piList31, 1, 2, 2, pstA, &piAddr311);
	//			//int *piAddr312	= NULL;
	//			//createMatrixOfStringInList(Rhs + 1, piList31, 2, 2, 2, pstA, &piAddr312);
	//			createMatrixOfDoubleInList(Rhs + 1, piList31, 2, 2, 2, pdblB);

	//int i2 = *Lstk(Rhs + 2) - *Lstk(Rhs + 1);
	//sciprint("2 -- diff : %d\n", i2);

	//LhsVar(1) = Rhs + 1;
	//PutLhsVar();
	//return 0;
	//CheckRhs(1,1);
	//CheckLhs(1,1);

	int iRows						= 0;
	int iCols						= 0;
	int iLen						= 0;
	int* piAddr					= NULL;
	char *pstVarName		= NULL;


	getVarAddressFromNumber(1, &piAddr);

	if(getVarType(piAddr) != sci_strings)
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: A string.\n"),fname, 2);
		return 0;
	}

	getVarDimension(piAddr, &iRows, &iCols);
	if(iRows != 1 || iCols != 1)
	{
		Scierror(999,_("%s: Wrong size for input argument #%d: A string expected.\n"),fname,2);
	}

	getMatrixOfString(piAddr, &iRows, &iCols, &iLen, NULL);
	pstVarName = (char*)MALLOC((iRows * iCols + 1) * sizeof(char));
	getMatrixOfString(piAddr, &iRows, &iCols, &iLen, &pstVarName);

	//open hdf5 file
	int iFile = openHDF5File(pstVarName);
	int iDataSetId = getDataSetId(iFile);

	//import all data
	import_data(iDataSetId, 0, NULL);

	//close the file
	closeHDF5File(iFile);

	int i2 = *Lstk(Rhs + 2) - *Lstk(Rhs + 1);
	sciprint("2 -- diff : %d\n", i2);

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int	import_data(int _iDatasetId, int _iItemPos, int* _piAddress)
{
	//get var type
	int iVarType = getScilabTypeFromDataSet(_iDatasetId);
	switch(iVarType)
	{
	case sci_matrix :
		{
			import_double(_iDatasetId, _iItemPos, _piAddress);
			break;
		}
	case sci_strings :
		{
			import_string(_iDatasetId, _iItemPos, _piAddress);
			break;
		}
	case sci_list :
	case sci_tlist :
	case sci_mlist :
		{
			import_list(_iDatasetId, iVarType, _iItemPos, _piAddress);
			break;
		}
	}
	return 0;
}

int import_double(int _iDatasetId, int _iItemPos, int* _piAddress)
{
	int	iRows, iCols;
	getDataSetDims(_iDatasetId, &iRows, &iCols);

	double *pdblData = (double *) malloc(iRows * iCols * sizeof(double));
	readDoubleMatrix(_iDatasetId, pdblData, iRows, iCols);

	if(_piAddress == NULL)
	{
		int *piAddr;
		//createMatrixOfDouble
		createMatrixOfDouble(Rhs + 1, iRows, iCols, pdblData, &piAddr);
	}
	else //if not null this variable is in a list
	{
		createMatrixOfDoubleInList(Rhs + 1, _piAddress, _iItemPos, iRows, iCols, pdblData);
	}

	free(pdblData);
	return 0;
}

int import_string(int _iDatasetId, int _iItemPos, int* _piAddress)
{
	int i = 0;
	int	iRows, iCols;
	getDataSetDims(_iDatasetId, &iRows, &iCols);

	char **pstData = (char **) malloc(iRows * iCols * sizeof(char*));

	readStringMatrix(_iDatasetId, pstData, iRows, iCols);

	if(_piAddress == NULL)
	{
		int *piAddr; 
		createMatrixOfString(Rhs + 1, iRows, iCols, pstData, &piAddr);
	}
	else //if not null this variable is in a list
	{
		int *piAddr; 
		createMatrixOfStringInList(Rhs + 1, _piAddress, _iItemPos, iRows, iCols, pstData, &piAddr);
	}

	for(i = 0 ; i < iRows * iCols ; i++)
	{
		free(pstData[i]);
	}
	free(pstData);
	return 0;
}

int import_list(int _iDatasetId, int _iVarType, int _iItemPos, int* _piAddress)
{
	int i							= 0;
	int	iRows					= 0;
	int iCols					= 0;
	int status				= 0;
	int* piListAddr		= NULL;

	hobj_ref_t* piItemRef		= NULL;

	status = getDataSetDims(_iDatasetId, &iRows, &iCols);
	if(status)
	{
		return 1;
	}

	status = getListItemReferences(_iDatasetId, &piItemRef);
	if(status)
	{
		return 1;
	}

	if(_piAddress == 0)
	{
		switch(_iVarType)
		{
		case sci_list : 
			createList(Rhs + 1, iRows * iCols, &piListAddr);
			break;
		case sci_tlist :
			createTList(Rhs + 1, iRows * iCols, &piListAddr);
			break;
		case sci_mlist :
			createMList(Rhs + 1, iRows * iCols, &piListAddr);
			break;
		default :
			return 1;
		}
	}
	else //if not null this variable is in a list
	{
		switch(_iVarType)
		{
		case sci_list : 
			createListInList(Rhs + 1, _piAddress, _iItemPos, iRows * iCols, &piListAddr);
			break;
		case sci_tlist :
			createTListInList(Rhs + 1, _piAddress, _iItemPos, iRows * iCols, &piListAddr);
			break;
		case sci_mlist :
			createMListInList(Rhs + 1, _piAddress, _iItemPos, iRows * iCols, &piListAddr);
			break;
		default :
			return 1;
		}
	}


	for(i = 0 ; i < iRows * iCols ; i++)
	{
		int iItemDataset = 0;
		getListItemDataset(_iDatasetId, piItemRef, i, &iItemDataset);
		if(iItemDataset == 0)
		{
			return 1;
		}
		import_data(iItemDataset, i + 1, piListAddr);
	}

	free(piItemRef);
	return 0;
}

void prtinf_tree(char* _pstMsg)
{
#ifdef PRINT_DEBUG
	char pstMsg[515];
	for(int i = 0 ; i < iTab ; i++)
	{
		sprintf(pstMsg + i, "\t");
	}
	sciprint("%s%s\n",pstMsg, _pstMsg);
#endif
}
/*--------------------------------------------------------------------------*/
