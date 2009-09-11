/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 * Copyright (C) 2009 - Digiteo - Vincent LIARD
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 */

#include "gw_statistics.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"

#define GWTABLE_LENGTH(t) (sizeof(Tab) / sizeof(gw_generic_table))

static gw_generic_table Tab[] = { 
  {cdfbetI, "cdfbet"},
  {cdfbinI, "cdfbin"},
  {cdfchiI, "cdfchi"},
  {cdfchnI, "cdfchn"},
  {cdffI  , "cdff"  },
  {cdffncI, "cdffnc"},
  {cdfgamI, "cdfgam"},
  {cdfnbnI, "cdfnbn"},
  {cdfnorI, "cdfnor"},
  {cdfpoiI, "cdfpoi"},
  {cdftI  , "cdft"  }};

int
gw_statistics(void)
{  
  Rhs = Max(0, Rhs);
  callFunctionFromGateway(Tab, GWTABLE_LENGTH(Tab));
  return 0;
}
