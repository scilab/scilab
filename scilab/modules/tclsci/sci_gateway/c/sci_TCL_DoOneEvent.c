/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2005-2008 - INRIA - Allan CORNET
 *  Copyright (C) 2008-2008 - INRIA - Bruno JOFRET
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_DoOneEvent (char *fname,unsigned long l)
{
	CheckRhs(0,0);
	CheckLhs(1,1);

	/* wait for events and invoke event handlers */
	Tcl_DoOneEvent(TCL_ALL_EVENTS | TCL_DONT_WAIT);

	LhsVar(1) = 0;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
