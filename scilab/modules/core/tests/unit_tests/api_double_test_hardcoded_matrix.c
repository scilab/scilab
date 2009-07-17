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

int sci_foo_get_hardcoded_matrix(char *fname,unsigned long fname_len)
{
	/*                  
	 * Foo=[1 4 2 3; 
	 *      3 9 8 2 ]
	 */
	int iVarOut			= Rhs + 1;
	double Foo[] = {1,3,4,9,2,8,3,2};   /* Declare the matrix */
	int iRows = 2, iCols = 4; /* Size of the matrix */

	//check number of output arguments
	CheckLhs(1,1);

	createMatrixOfDouble(iVarOut, iRows, iCols, Foo);
	LhsVar(1)				= iVarOut;

  return 0;
}
