
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
/*--------------------------------------------------------------------------*/
#include "gw_cacsd0.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{C2F(sci_gschur),"gschur"},
	{C2F(sci_gspec),"gspec"},
	{C2F(sci_ereduc),"ereduc"},
	{C2F(sci_fstair),"fstair"}
};
/*--------------------------------------------------------------------------*/
int gw_cacsd0(void)
{  
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
