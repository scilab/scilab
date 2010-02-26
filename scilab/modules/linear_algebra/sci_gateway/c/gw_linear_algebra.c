
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
#include "stack-c.h"
#include "gw_linear_algebra.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/ 
static int C2F(intvoid)(char *fname, unsigned long fname_len)
{
	return 0;
}
/*--------------------------------------------------------------------------*/ 
#define LINEAR_ALGEBRA_TAB_SIZE 6
static gw_generic_table Tab[LINEAR_ALGEBRA_TAB_SIZE]={
  {NULL, ""}, //hess
  {C2F(intschur),"schur"},
  {NULL, ""}, //spec
  {NULL, ""}, //bdiag
  {C2F(intvoid),"xxxx"},
  {NULL, ""} //balanc
};
/*--------------------------------------------------------------------------*/ 
int gw_linear_algebra(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/ 
