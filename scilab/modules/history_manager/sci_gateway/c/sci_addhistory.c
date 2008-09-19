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
#include "gw_history_manager.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "Scierror.h"
#include "localization.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_addhistory)(char *fname,unsigned long fname_len)
{
	static int n1,m1;

	CheckRhs(1,1);
	CheckLhs(0,1);

	if (GetType(1) == sci_strings)
	{
		char **lines=NULL;
		BOOL bOK = FALSE;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&lines);
		bOK = appendLinesToScilabHistory(lines,m1*n1);

		LhsVar(1) = 0;
		C2F(putlhsvar)();

		freeArrayOfString(lines, m1*n1);
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String array expected.\n"),fname,1);
		return 0;
	}

	return 0;
}
/*--------------------------------------------------------------------------*/
