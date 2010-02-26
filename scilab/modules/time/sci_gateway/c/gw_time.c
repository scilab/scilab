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
#define TIME_TAB_SIZE 7
static gw_generic_table Tab[TIME_TAB_SIZE]={
	{NULL, ""}, //getdate
	{NULL, ""}, //Calendar
	{NULL, ""}, //timer
	{NULL, ""}, //sleep
	{NULL, ""}, //xpause
	{NULL, ""}, //realtimeinit
	{NULL, ""}, //realtime
};
/*--------------------------------------------------------------------------*/ 
int gw_time(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/ 
