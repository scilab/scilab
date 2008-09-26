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
static gw_generic_table Tab[]=
{
	{C2F(sci_getdynlibext),"getdynlibext"},
	{C2F(sci_addinter),"addinter"},
	{C2F(sci_fort),"fort"},
	{C2F(sci_call),"call"},
	{C2F(sci_link),"link"},
	{C2F(sci_ulink),"ulink"},
	{C2F(sci_c_link),"c_link"},
};
/*--------------------------------------------------------------------------*/
int gw_dynamic_link(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/
