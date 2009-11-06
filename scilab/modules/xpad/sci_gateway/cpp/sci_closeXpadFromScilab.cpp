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
/*--------------------------------------------------------------------------*/
#include "Xpad.hxx"

extern "C"
{
#include "gw_xpad.h"
#include "stack-c.h"
#include "api_scilab.h"
#include "getScilabJavaVM.h"
}
/*--------------------------------------------------------------------------*/
using namespace org_scilab_modules_xpad;
/*--------------------------------------------------------------------------*/
int sci_closeXpadFromScilab(char *fname,unsigned long fname_len)
{
	CheckRhs(0,0);
	CheckLhs(0,1);

	Xpad::closeXpadFromScilab(getScilabJavaVM());

	LhsVar(1) = 0;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
