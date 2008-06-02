/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007-2008 - INRIA - Allan Cornet
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
#include "machine.h"
#include "stack-c.h"
#include "HistoryManager.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_gethistoryfile)(char *fname,unsigned long fname_len)
{
	char *filename = NULL;
	int m1 = 0, n1 = 0;

	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	filename = getFilenameScilabHistory();

	if (filename)
	{
		n1=1;
		CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,(m1=(int)strlen(filename), &m1),&n1,&filename);
		if (filename) {FREE(filename);filename=NULL;}
		LhsVar(1) = Rhs+1;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: An error occurred: %s\n"),fname,_("filename not defined."));
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
