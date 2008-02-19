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
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
{C2F(sci_winopen),"winopen"},
{C2F(sci_winqueryreg),"winqueryreg"},
{C2F(sci_findfileassociation),"findfileassociation"},
{C2F(sci_dos),"dos"},
{C2F(sci_mcisendstring),"mcisendstring"},
{C2F(sci_oemtochar),"oemtochar"},
{C2F(sci_chartooem),"chartooem"},
{C2F(sci_consolebox),"consolebox"},
{C2F(sci_printsetupbox),"printsetupbox"},
{C2F(sci_toprint),"toprint"},
};
/*--------------------------------------------------------------------------*/
int gw_windows_tools(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	
	return 0;
}
/*--------------------------------------------------------------------------*/
