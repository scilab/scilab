
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
#include "gw_time.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]={
    {C2F(sci_getdate),"getdate"},
	{C2F(sci_calendar),"Calendar"},
	{C2F(sci_timer),"timer"},
	{C2F(sci_sleep),"sleep"},
	{C2F(sci_xpause),"xpause"},
	{C2F(sci_realtimeinit),"realtimeinit"},
	{C2F(sci_realtime),"realtime"},
};
/*--------------------------------------------------------------------------*/ 
int gw_time(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);
	return 0;
}
/*--------------------------------------------------------------------------*/ 
