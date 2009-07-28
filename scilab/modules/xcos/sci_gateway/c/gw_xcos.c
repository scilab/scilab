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
#include "gw_xcos.h"
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
#define XCOS_TAB_SIZE 1
static gw_generic_table Tab[XCOS_TAB_SIZE]={
  {sci_Xcos, "Xcos"}
};

/*--------------------------------------------------------------------------*/
int gw_xcos(void)
{
	Rhs = Max(0, Rhs);

	if ( getScilabMode() == SCILAB_NWNI)
	{
		Scierror(999,_("Scilab '%s' module disabled in -nogui or -nwni mode."), "xcos");
		return 0;
	}

	if (!loadedDep) 
	{
		loadOnUseClassPath("XCos");
		loadedDep = TRUE;
	}
	callFunctionFromGateway(Tab,XCOS_TAB_SIZE);
	return 0;
}
/*--------------------------------------------------------------------------*/
