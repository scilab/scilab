/*
* Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
* Copyright (C) INRIA - Allan CORNET
* 
* This file must be used under the terms of the CeCILL.
* This source file is licensed as described in the file COPYING, which
* you should have received as part of this distribution.  The terms
* are also available at    
* http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
*
*/

/*--------------------------------------------------------------------------*/
#include <string.h>
#include "gw_windows_tools.h"
#include "MALLOC.h" /* MALLOC */
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int sci_win64(char *fname,unsigned long l)
{
	int n1 = 0,m1 = 0;
	int *Status = NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Status = (int*)MALLOC(sizeof(int));

#ifdef _WIN64
	*Status = TRUE;
#else
	*Status = FALSE;
#endif

	m1 = 1;n1 = 1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
	LhsVar(1)=Rhs+1;

	C2F(putlhsvar)();
	if (Status) {FREE(Status);Status=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/

