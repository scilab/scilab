/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include <math.h>
#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_arnoldi.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]={
  {C2F(intdsaupd),"dsaupd"},
  {C2F(intdnaupd),"dnaupd"},
  {C2F(intznaupd),"znaupd"},
  {C2F(intdseupd),"dseupd"},
  {C2F(intdneupd),"dneupd"},
  {C2F(intzneupd),"zneupd"},
};
/*--------------------------------------------------------------------------*/
int gw_arnoldi(void)
{
	Rhs = Max(0, Rhs);

	callFunctionFromGateway(Tab);

	return 0;
}
/*--------------------------------------------------------------------------*/


