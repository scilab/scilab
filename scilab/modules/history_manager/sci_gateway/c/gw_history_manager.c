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
static gw_generic_table Tab[] =
{
	{sci_loadhistory,"loadhistory"},
	{sci_savehistory,"savehistory"},
	{sci_gethistory,"gethistory"},
	{sci_resethistory,"resethistory"},
	{sci_displayhistory,"displayhistory"},
	{sci_addhistory,"addhistory"},
	{sci_saveconsecutivecommands,"saveconsecutivecommands"},
	{sci_saveafterncommands,"saveafterncommands"},
	{sci_gethistoryfile,"gethistoryfile"},
	{sci_sethistoryfile,"sethistoryfile"},
	{sci_removelinehistory,"removelinehistory"},
	{sci_historysize,"historysize"},
	{sci_historymanager,"historymanager"}
};
/*--------------------------------------------------------------------------*/
int gw_history_manager(void)
{
	Rhs = Max(0, Rhs);
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
