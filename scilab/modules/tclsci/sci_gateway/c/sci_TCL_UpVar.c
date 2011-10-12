/*
*  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
*  Copyright (C) 2005-2008 - INRIA - Allan CORNET
*  Copyright (C) 2005-2008 - INRIA - Bruno JOFRET
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
#include "TCL_Global.h"
#include "gw_tclsci.h"
#include "Scierror.h"
#include "localization.h"
#include "GlobalTclInterp.h"
/*--------------------------------------------------------------------------*/
int sci_TCL_UpVar (char *fname,unsigned long l)
{
	CheckRhs(2,3);
	CheckLhs(0,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		int m1 = 0, n1 = 0, l1 = 0;
		int m2 = 0, n2 = 0, l2 = 0;

		Tcl_Interp *TCLinterpreter = NULL;
		char *sourceName = NULL, *destName = NULL;
		int *paramoutINT = (int*)MALLOC(sizeof(int));

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		sourceName = cstk(l1);

		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		destName = cstk(l2);

		if (getTclInterp() == NULL)
		{
			releaseTclInterp();
			Scierror(999,_("%s: Error main TCL interpreter not initialized.\n"),fname);
			return 0;
		}
		releaseTclInterp();

		if (Rhs == 3)
		{
			int m3 = 0, n3 = 0, l3 = 0;
			/* three arguments given - get a pointer on the slave interpreter */
			if (GetType(3) == sci_strings)
			{
				GetRhsVar(3,STRING_DATATYPE,&m3,&n3,&l3);
				TCLinterpreter = Tcl_GetSlave(getTclInterp() ,cstk(l3));
				releaseTclInterp();
				if (TCLinterpreter == NULL)
				{
					Scierror(999,_("%s: No such slave interpreter.\n"),fname);
					return 0;
				}
			}
			else
			{
				Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname, 3);
				return 0;
			}
		}
		else
		{
			/* only two arguments given - use the main interpreter */
			TCLinterpreter = getTclInterp();
			releaseTclInterp();
		}

		if ( Tcl_GetVar(TCLinterpreter, sourceName, TCL_GLOBAL_ONLY) )
		{
			if ( Tcl_UpVar(TCLinterpreter,"#0", sourceName, destName, TCL_GLOBAL_ONLY) == TCL_ERROR )
			{
				*paramoutINT = (int)(FALSE);
			}
			else
			{
				*paramoutINT = (int)(TRUE);
			}
		}
		else
		{
			*paramoutINT = (int)(FALSE);
		}

		n1 = 1;
		CreateVarFromPtr(Rhs + 1, MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
		LhsVar(1) = Rhs+1;
        if (paramoutINT) {FREE(paramoutINT); paramoutINT = NULL;}
		PutLhsVar();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d or #%d: String expected.\n"),fname, 1, 2);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
