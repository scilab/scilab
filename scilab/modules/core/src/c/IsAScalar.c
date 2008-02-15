/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <string.h>
#include "machine.h"
#include "stack-c.h"
#include "IsAScalar.h"
/*--------------------------------------------------------------------------*/ 
int IsAScalar(int RhsNumber)
{

	if ( GetType(RhsNumber) == sci_matrix )
	{
		static int l1,n1,m1;
		GetRhsVar(RhsNumber,MATRIX_OF_DOUBLE_DATATYPE,&m1,&n1,&l1);
		if ( (m1 == 1) && (n1 == 1) )
		{
			return 1;
		}
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
