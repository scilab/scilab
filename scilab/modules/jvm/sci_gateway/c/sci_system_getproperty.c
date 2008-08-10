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
#include "gw_jvm.h"
#include "MALLOC.h"
#include "machine.h"
#include "stack-c.h"
#include "system_getproperty.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_system_getproperty)(char *fname,unsigned long fname_len)
{
	Rhs = Max(Rhs,0);
	CheckRhs(1,1);
	CheckLhs(0,1);

	if ( GetType(1) == sci_strings )
	{
		static int m1,n1=0,l1=0;
		char *propertyName = NULL;
		char *propertyValue = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		propertyName=cstk(l1);
		propertyValue=system_getproperty(propertyName,"unknown");

		n1=1;
		CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(propertyValue), &m1),&n1,&propertyValue);
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		if (propertyValue) {FREE(propertyValue);propertyValue=NULL;}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d: String expected.\n"),fname, 1);
		return 0;
	}
	return 0;
}

/*--------------------------------------------------------------------------*/

