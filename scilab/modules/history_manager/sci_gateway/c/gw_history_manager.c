/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/**
 * gateway source file which defines links between the
 * native function (C, fortran, whatever) and the function offered into Scilab
 */
/*--------------------------------------------------------------------------*/
#include "gw_history_manager.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[] =
{
	{NULL,""}, // loadhistory
	{NULL,""}, // savehistory
	{NULL,""}, // gethistory
	{NULL,""}, // resethistory
	{NULL,""}, // displayhistory
	{NULL,""}, // addhistory
	{NULL,""}, // saveconsecutivecommands
	{NULL,""}, // saveafterncommands
	{NULL,""}, // gethistoryfile
	{NULL,""}, // sethistoryfile
	{NULL,""}, // removelinehistory
	{NULL,""}, // historysize
	{NULL,""}  // historymanager
};
/*--------------------------------------------------------------------------*/
int gw_history_manager(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
