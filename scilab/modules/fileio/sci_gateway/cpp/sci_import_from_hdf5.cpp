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

int	import_data(int _iDatasetId, int _iItemPos, int* _piAddress);
int import_double(int _iDatasetId, int _iItemPos, int* _piAddress);
int import_string(int _iDatasetId, int _iItemPos, int* _piAddress);
int import_list(int _iDatasetId, int _iVarType, int _iItemPos, int* _piAddress);

int sci_import_from_hdf5(char *fname,unsigned long fname_len)
{
	CheckRhs(1,1);
	CheckLhs(1,1);

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

	void* piItemRef		= NULL;

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
		createList(Rhs + 1, iRows * iCols, &piListAddr);
	}
	else //if not null this variable is in a list
	{
		createListInList(Rhs + 1, _piAddress, _iItemPos, iRows * iCols, &piListAddr);
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

	return 0;
}

/*--------------------------------------------------------------------------*/
