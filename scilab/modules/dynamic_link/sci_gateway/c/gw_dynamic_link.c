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
#include <string.h>
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{sci_getdynlibext,"getdynlibext"},
	{sci_addinter,"addinter"},
	{sci_fort,"fort"},
	{sci_call,"call"},
	{sci_link,"link"},
	{sci_ulink,"ulink"},
	{sci_c_link,"c_link"},
	{sci_ilib_verbose,"ilib_verbose"}
};
/*--------------------------------------------------------------------------*/
int gw_dynamic_link(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
