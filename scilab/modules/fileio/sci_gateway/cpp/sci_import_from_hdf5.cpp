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
	#include "variable_api.h"
	#include "../../call_scilab/includes/CallScilab.h"
}

int	import_data(int _iFile, int* _piAddress);
int import_double(int _iFile, int* _piAddress);
int import_string(int _iFile, int* _piAddress);
int import_list(int _iFile, int _iVarType, int* _piAddress);

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
	int iFile = 0;
	
	import_data(iFile, NULL);

	LhsVar(1) = Rhs + 1;
	PutLhsVar();
	return 0;
}

int	import_data(int _iFile, int* _piAddress)
{
	//get var type
	int iVarType = 0;
	switch(iVarType)
	{
	case sci_matrix :
		{
			import_double(_iFile, _piAddress);
			break;
		}
	case sci_strings :
		{
			import_string(_iFile, _piAddress);
			break;
		}
	case sci_list :
	case sci_tlist :
	case sci_mlist :
		{
			import_list(_iFile, iVarType, _piAddress);
			break;
		}
	}
	return 0;
}

int import_double(int _iFile, int* _piAddress)
{
	
	if(_piAddress == NULL)
	{
		//createMatrixOfDouble
	}
	else //if not null this variable is in a list
	{
	}
	return 0;
}

int import_string(int _iFile, int* _piAddress)
{
	return 0;
}

int import_list(int _iFile, int _iVarType, int* _piAddress)
{
	return 0;
}

/*--------------------------------------------------------------------------*/
