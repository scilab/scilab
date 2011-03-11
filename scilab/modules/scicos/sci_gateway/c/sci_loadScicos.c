/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) 2011 - DIGITEO - Clément DAVID
*
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

#include "gw_scicos.h"
#include "stack-c.h"
#include "api_common.h"

/*
 * Stub function to load the gateway
 */
int sci_loadScicos(char *fname, unsigned long fname_len) {
	CheckRhs(0,1);
	LhsVar(1) = 0;
	PutLhsVar();
	return 0;
}

