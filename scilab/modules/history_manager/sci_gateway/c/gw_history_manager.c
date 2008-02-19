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
#include <string.h>
#include "gw_history_manager.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
	{C2F(sci_loadhistory),"loadhistory"},
	{C2F(sci_savehistory),"savehistory"},
	{C2F(sci_gethistory),"gethistory"},
	{C2F(sci_resethistory),"resethistory"},
	{C2F(sci_displayhistory),"displayhistory"},
	{C2F(sci_addhistory),"addhistory"},
	{C2F(sci_saveconsecutivecommands),"saveconsecutivecommands"},
	{C2F(sci_saveafterncommands),"saveafterncommands"},
	{C2F(sci_gethistoryfile),"gethistoryfile"},
	{C2F(sci_sethistoryfile),"sethistoryfile"},
	{C2F(sci_removelinehistory),"removelinehistory"},
	{C2F(sci_historysize),"historysize"},
	{C2F(sci_historymanager),"historymanager"}
};
/*--------------------------------------------------------------------------*/
int gw_history_manager(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab);

	return 0;
}
/*--------------------------------------------------------------------------*/
