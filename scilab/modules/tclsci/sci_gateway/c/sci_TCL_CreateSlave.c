/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005-2008 - INRIA - Allan CORNET
 * Copyright (C) 2007-2008 - INRIA - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_CreateSlave(char *fname,unsigned long l)
{
	int TypeVar1=0;
	CheckRhs(1,1);
	CheckLhs(1,1);

	TypeVar1=GetType(1);

	if (TypeVar1 == sci_strings)
	{
		static int l2,n2,m2;
		Tcl_Interp *TCLinterpreter=NULL;

		if (!existsGlobalInterp())
		{
		  Scierror(999,_("%s: Error main TCL interpreter not initialized.\n"),fname);
		  return 0;
		}

		GetRhsVar(1,STRING_DATATYPE,&m2,&n2,&l2);

		if (!existsSlaveInterp(cstk(l2)))
		{
			TCLinterpreter=Tcl_CreateSlave(getTclInterp(),cstk(l2), 1);
			releaseTclInterp();
			if (TCLinterpreter==NULL)
			{
				Scierror(999,_("%s: Could not create a slave interpreter.\n"),fname);
				return 0;
			}
		}
		else
		{
			Scierror(999,_("%s: %s interpreter already exists.\n"),fname,cstk(l2));
			return 0;
		}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname, 1);
		return 0;
	}

	LhsVar(1)=0;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
