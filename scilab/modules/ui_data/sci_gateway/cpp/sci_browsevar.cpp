/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "BrowseVar.hxx"

extern "C"
{
#include "gw_ui_data.h"
#include "stack-c.h"
#include "getScilabJavaVM.h"
}
using namespace org_scilab_modules_ui_data;
/*--------------------------------------------------------------------------*/
int sci_browsevar(char *fname,unsigned long fname_len)
{
	CheckRhs(0,0);
	CheckLhs(0,1);	
	
    BrowseVar::openVariableBrowser(getScilabJavaVM());

	LhsVar(1) = 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
