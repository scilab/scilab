/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) ????-2008 - INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_special_functions1.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = 
{ 
  {C2F(sci_oldbesseli),"oldbesseli"},
  {C2F(sci_oldbesselj), "oldbesselj"},
  {C2F(sci_oldbesselk),"oldbesselk"},
  {C2F(sci_oldbessely),"oldbessely"},
  {C2F(sci_gamma),"gamma"},
  {C2F(sci_lgamma),"gammaln"},
  {C2F(sci_dlgamma),"dlgamma"},
  {C2F(sci_calerf),"calerf"}
};
/*--------------------------------------------------------------------------*/
int gw_special_functions1(void)
{
	Rhs = Max(0, Rhs);

	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
