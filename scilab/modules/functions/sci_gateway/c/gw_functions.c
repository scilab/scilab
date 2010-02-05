
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "gw_functions.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "recursionFunction.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{NULL ,""}, //lib
	{NULL, ""}, //deff
	{NULL, ""}, //getf
	{NULL, ""}, //exec
	{NULL, ""}, //execstr
	{sci_librarieslist,"librarieslist"},
	{sci_libraryinfo,"libraryinfo"},
	{sci_whereis,"whereis"}
};
/*--------------------------------------------------------------------------*/
int gw_functions(void)
{  
	Rhs = Max(0, Rhs);

	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
