/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "stack-c.h"
#include "getcommandlineargs.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_sciargs)(char *fname,unsigned long fname_len)
{
	char **arguments = NULL;
	int nb_arguments = 0;
	
	int ncol = 0;
	int nrow = 0;

	arguments = getCommandLineArgs(&nb_arguments);

	nrow = 1;
	ncol = nb_arguments;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &nrow, &ncol,arguments);
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();

	freeArrayOfString(arguments,nb_arguments);

	return 0;
}
/*--------------------------------------------------------------------------*/
