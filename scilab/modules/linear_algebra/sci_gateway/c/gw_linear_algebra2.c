
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET <allan.cornet@inria.fr>
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
#include "gw_linear_algebra.h"
#include "gw_linear_algebra2.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/ 
#define LINEAR_ALGEBRA2_TAB_SIZE 15
static gw_generic_table Tab[LINEAR_ALGEBRA2_TAB_SIZE]={
	{NULL, ""}, //qr
	{NULL, ""}, //svd
	{NULL, ""}, //lsq
	{NULL, ""}, //spec
	{NULL, ""}, //inv
	{NULL, ""}, //rcond
	{NULL, ""}, //chol
	{NULL, ""}, //lu
	{C2F(intslash),"slash"},
	{C2F(intbackslash),"backslash"},
	{C2F(intschur),"schur"},
	{NULL, ""}, //hess
	{NULL, ""}, //det
	{NULL, ""} //balanc
};
/*--------------------------------------------------------------------------*/ 
int gw_linear_algebra2(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	if (Err <= 0 && C2F(errgst).err1 <= 0) PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/ 
