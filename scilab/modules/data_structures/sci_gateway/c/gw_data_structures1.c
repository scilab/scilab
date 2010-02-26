/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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
#include "gw_data_structures1.h"
#include "callFunctionFromGateway.h"
#include "stack-c.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_scilist),"list"},
	{C2F(sci_scitlist),"tlist"},
	{C2F(sci_rlist),"rlist"},
	{C2F(sci_lsslist),"lsslist"},
	{C2F(sci_glist),"glist"},
	{C2F(sci_lstcat),"lstcat"},
	{C2F(sci_scimlist),"mlist"},
	{C2F(sci_definedfields),"definedfields"},
	{C2F(sci_lstsize),"lstsize"}
};
/*--------------------------------------------------------------------------*/
int gw_data_structures1(void)
{
	Rhs=Max(Rhs,0);
	if (Top - Rhs + Lhs + 1 >= Bot)
	{
		static int codeerror = 18;
		Error(codeerror);
		return 0;
	}

	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(gwdatastructures1)(void)
{
	return gw_data_structures1();
}
/*--------------------------------------------------------------------------*/
