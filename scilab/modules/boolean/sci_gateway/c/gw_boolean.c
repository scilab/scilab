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

#include <string.h>
/*--------------------------------------------------------------------------*/
#include "gw_boolean.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
#define BOOLEAN_TAB_SIZE 4
static gw_generic_table Tab[BOOLEAN_TAB_SIZE]=
{
	{C2F(sci_find),"find"},
	{C2F(sci_bool2s),"bool2s"},
	{C2F(intor),"or"},
	{C2F(intand),"and"}
};
/*--------------------------------------------------------------------------*/
int gw_boolean(void)
{  
	callFunctionFromGateway(Tab,BOOLEAN_TAB_SIZE);
	return 0;
}
/*--------------------------------------------------------------------------*/
