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
/*--------------------------------------------------------------------------*/
int C2F(sci_sethistoryfile)(char *fname,unsigned long fname_len)
{
	CheckRhs(0,1) ;
	CheckLhs(0,1) ;

	if (Rhs == 0)
	{
		setDefaultFilenameScilabHistory();
	}
	else
	{
		int l1 = 0, m1 = 0, n1 = 0;
		char  *filename = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		filename=cstk(l1);
		setFilenameScilabHistory(filename);
	}
	LhsVar(1) = 0;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
