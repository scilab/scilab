/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
 /*--------------------------------------------------------------------------*/
#include "gw_renderer.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
#define RENDERER_TAB_SIZE 0
static gw_generic_table Tab[RENDERER_TAB_SIZE]=
{

};
/*--------------------------------------------------------------------------*/
/* This gateway is not used and it should be not built */
int gw_renderer(void)
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab,RENDERER_TAB_SIZE);
	return 0;
}
/*--------------------------------------------------------------------------*/
