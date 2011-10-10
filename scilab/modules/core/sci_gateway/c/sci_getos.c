/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "MALLOC.h"
#include "getos.h"
#include "localization.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getos)(char *fname,unsigned long fname_len)
{
	static int n1 = 0, m1 = 0;
	char *OperatingSystem = getOSFullName();

	Rhs = Max(0,Rhs);
	CheckRhs(0,0);
	CheckLhs(1,2);

	if (OperatingSystem)
	{
		n1 = 1;
		CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(OperatingSystem), &m1),&n1,&OperatingSystem);
		if (OperatingSystem) {FREE(OperatingSystem); OperatingSystem = NULL;}
		LhsVar(1)=Rhs+1;

		if (Lhs == 2)
		{
			char *Release = getOSRelease();

			if (Release)
			{
				n1 = 1;
				CreateVarFromPtr(Rhs+ 2,STRING_DATATYPE,(m1=(int)strlen(Release), &m1),&n1,&Release);
				if (Release) {FREE(Release); Release = NULL;}
				LhsVar(2) = Rhs + 2;
			}
			else
			{
				Scierror(999,_("%s: No more memory.\n"),fname);
				return 0;
			}
		}
		PutLhsVar();
	}
	else
	{
		Scierror(999,_("%s: No more memory.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
