/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2009 - DIGITEO - Allan CORNET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "gw_xpad.h"
#include "stack-c.h"
#include "callFunctionFromGateway.h"
#include "BOOL.h"
#include "scilabmode.h"
#include "localization.h"
#include "Scierror.h"
#include "BOOL.h"
#include "loadOnUseClassPath.h"
/*--------------------------------------------------------------------------*/
static BOOL loadedDep = FALSE;
/*--------------------------------------------------------------------------*/
static gw_generic_table Tab[]=
{
  {sci_xpad, "xpad"},
  {sci_xpad_highlightline, "xpad_highlightline"},
  {sci_xpad, "editor"},
  {sci_xpad_highlightline, "editor_highlightline"}
};
/*--------------------------------------------------------------------------*/
int gw_xpad(void)
{
	Rhs = Max(0, Rhs);

	if ( getScilabMode() == SCILAB_NWNI)
	{
		Scierror(999,_("Scilab '%s' module disabled in -nogui or -nwni mode."), "xpad");
		return 0;
	}

	if (!loadedDep) 
	{
		loadOnUseClassPath("Xpad");
		loadedDep = TRUE;
	}
	callFunctionFromGateway(Tab, SIZE_CURRENT_GENERIC_TABLE(Tab));
	return 0;
}
/*--------------------------------------------------------------------------*/
