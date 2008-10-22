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
#include "stack-c.h"
#include "system_setproperty.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_system_setproperty)(char *fname,unsigned long fname_len)
{
	Rhs = Max(Rhs,0);
	CheckRhs(2,2);
	CheckLhs(1,1);

	if ( (GetType(1) == sci_strings) && (GetType(2) == sci_strings) )
	{
		static int m1,n1=0,l1=0;
		static int m2,n2=0,l2=0;
		char *propertyName = NULL;
		char *propertyValue = NULL;
		char *previousValue = NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		propertyName = cstk(l1);

		GetRhsVar(2,STRING_DATATYPE,&m2,&n2,&l2);
		propertyValue = cstk(l2);

		previousValue = system_setproperty(propertyName,propertyValue);

		if (previousValue)
		{
			n1=1;
			CreateVarFromPtr(Rhs+1,STRING_DATATYPE,(m1=(int)strlen(propertyValue), &m1),&n1,&previousValue);
		}
		else
		{
			n1 = 0;
			m1 = 0;
			l1 = 0;
			CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,&n1,&m1,&l1);
		}
		LhsVar(1)=Rhs+1;
		C2F(putlhsvar)();
		if (previousValue) {FREE(previousValue); previousValue = NULL;}
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument #%d or #%d: String expected.\n"),fname, 1, 2);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
