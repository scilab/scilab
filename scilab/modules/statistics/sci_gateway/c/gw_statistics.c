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
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{ 
	{NULL, ""}, //cdfbet
	{NULL, ""}, //cdfbin
	{NULL, ""}, //cdfchi
	{NULL, ""}, //cdfchn
	{NULL, ""}, //cdff
	{NULL, ""}, //cdffnc
	{NULL, ""}, //cdfgam
	{NULL , ""}, //cdfnbn
	{NULL, ""}, //cdfnor
	{NULL, ""}, //cdfpoi
	{NULL, ""} //cdft
};
/*--------------------------------------------------------------------------*/
int gw_statistics()
{  
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
