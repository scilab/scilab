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

double *Foo;
int iRows					= 0;
int iCols					= 0;

int sci_foo_set_matrix(char *fname,unsigned long fname_len)
{
	//for input variable 1
	int* piAddrVar1			= NULL;
	double *pdblReal		= NULL;

	/************************
	 * Get input arguments *
	 ************************/

	//check number of input arguments
	CheckRhs(1,1);

	//get variable 1 address
	getVarAddressFromPosition(1, &piAddrVar1);

	//get variable 1 dimensions
	getVarDimension(piAddrVar1, &iRows, &iCols);

	//check type, dimension and if it is a real number
	if(getVarType(piAddrVar1) != sci_matrix || isVarComplex(piAddrVar1))
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: Real matrix expected.\n"), fname, 1);
		}

	//get the variable
	getMatrixOfDouble(piAddrVar1, &iRows, &iCols, &pdblReal);
	if (Foo!=NULL){
		free(Foo);
	}
	Foo = (double *) malloc(iRows * iCols * sizeof(double));

	//extract the first value
	memcpy(Foo,pdblReal,iRows * iCols * sizeof(double));
  
  return 0;
}


int sci_foo_get_matrix(char *fname,unsigned long fname_len)
{
	int iVarOut			= Rhs + 1;

	//check number of output arguments
	CheckLhs(1,1);

	createMatrixOfDouble(iVarOut, iRows, iCols, Foo);
	LhsVar(1)				= iVarOut;

  return 0;
}
