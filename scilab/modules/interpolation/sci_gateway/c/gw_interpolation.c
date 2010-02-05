/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_interpolation.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] = 
{ 
  {intsplin,           "splin"},
  {intlsq_splin,       "lsq_splin"},
  {intinterp1,          "interp"},
  {intlinear_interpn,  "linear_interpn"},
  {intsplin2d,         "splin2d"},
  {intinterp2d,        "interp2d"},
  {intcshep2d,         "cshep2d"},
  {inteval_cshep2d,    "eval_cshep2d" },
  {intsplin3d,         "splin3d"},
  {intinterp3d,        "interp3d"},
  {intbsplin3val,      "bsplin3val"}
};
/*--------------------------------------------------------------------------*/
int gw_interpolation(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab,SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
