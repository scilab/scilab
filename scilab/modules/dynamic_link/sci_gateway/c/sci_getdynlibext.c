/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "MALLOC.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "api_oldstack.h"
#include "gw_dynamic_link.h"
/*--------------------------------------------------------------------------*/
int sci_getdynlibext(char *fname, int* _piKey)
{
	CheckRhs(0,0);
	CheckLhs(1,1);

    createSingleString(_piKey, Rhs + 1, SHARED_LIB_EXT);

	LhsVar(1) = Rhs+1;
	PutLhsVar();

	return 0;
}
/*--------------------------------------------------------------------------*/
