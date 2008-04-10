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
#include "gw_windows_tools.h"
#include "stack-c.h"
#include "httpdownloadfile.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/
int sci_winhttpdownload(char *fname,unsigned long l)
{
	int m1 = 0,n1 = 0,l1 = 0;
	int m2 = 0,n2 = 0,l2 = 0;
	int *Status = NULL;

	CheckRhs(2,2);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) ) 

	GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
	GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);

	Status = (int*)MALLOC(sizeof(int));

	*Status = (BOOL) httpDownloadFile(cstk(l1),cstk(l2));
	
	m1 = 1;n1 = 1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &Status);
	LhsVar(1)=Rhs+1;

	C2F(putlhsvar)();
	if (Status) {FREE(Status);Status=NULL;}

	return 0;
}
/*--------------------------------------------------------------------------*/
