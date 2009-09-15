/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_localization.h"
#include "setgetlanguage.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int sci_getlanguage(char *fname,unsigned long fname_len)
{
	static int n1,m1;
	char *Output=NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Output=getlanguage();

	n1=1;
	CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
	LhsVar(1) = Rhs+1;
	C2F(putlhsvar)();

	return 0;
}
/*--------------------------------------------------------------------------*/
