/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2009 - DIGITEO - Antoine ELIAS
* Copyright (C) 2009 - DIGITEO - Sylvestre LEDRU
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "sciprint.h"
#include "api_variable.h"

double Foo;

int sci_foo_set_scalar(char *fname,unsigned long fname_len)
{
	//for input variable 1
	int* piAddrVar1			= NULL;
	int iRows1					= 0;
	int iCols1					= 0;
	double *pdblReal1		= NULL;

	//check number of input arguments
	CheckRhs(1,1);

	//get variable 1 address
	getVarAddressFromPosition(1, &piAddrVar1);
	//get variable 1 dimensions
	getVarDimension(piAddrVar1, &iRows1, &iCols1);
	//check type, dimension and if it is a real number
	if(getVarType(piAddrVar1) != sci_matrix || iRows1 != 1 || iCols1 != 1 || isVarComplex(piAddrVar1))
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Real scalar expected.\n"), fname, 1);
	}

	//get variable 1 data
	getMatrixOfDouble(piAddrVar1, &iRows1, &iCols1, &pdblReal1);
	//extract the first value
	Foo			= pdblReal1[0];
  
	//	PutLhsVar();
  return 0;
}


int sci_foo_get_scalar(char *fname,unsigned long fname_len)
{
	int iRowsOut		= 1;
	int iColsOut		= 1;
	int iVarOut			= Rhs + 1;

	//check number of output arguments
	CheckLhs(1,1);

	createMatrixOfDouble(iVarOut, iRowsOut, iColsOut, &Foo);
	LhsVar(1)				= iVarOut;

	return 0;
}
