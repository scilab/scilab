/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * ...
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
#include "gw_fileio.h"
#include "stack-c.h"
#include "getlongpathname.h"
#include "localization.h"
#include "MALLOC.h"
#include "Scierror.h"
#include "charEncoding.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_getlongpathname)(char *fname,unsigned long l)
{
	static int l1,n1,m1;

	CheckRhs(1,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		char szTemp[bsiz];
		int bOK=FALSE;
		char *LongName=NULL;
		char *ShortName=NULL;
    
		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);

		/* bug 3089 */
		ShortName = UTFToLocale(cstk(l1), szTemp);

		LongName = getlongpathname(ShortName,&bOK);

		if (LongName) 
		{
		  char *LongNameUTF=localeToUTF(LongName, szTemp);
			m1 = (int)strlen(LongNameUTF);
			n1 = 1;
			CreateVarFromPtr(Rhs+ 1,STRING_DATATYPE,&m1,&n1,&LongNameUTF);
		}
		else 
		{
			m1 = 0;
			n1 = 0;
			l1 = 0;
			CreateVar(Rhs+1,STRING_DATATYPE,  &m1, &n1, &l1);
		}
		LhsVar(1)=Rhs+1;

		if (LongName) {FREE(LongName);LongName=NULL;}

		if (Lhs != 1) /* Lhs == 2 */
		{
			int *bOkOutINT=(int*)MALLOC(sizeof(int));

			*bOkOutINT=bOK;
			n1=1;
			CreateVarFromPtr(Rhs+2,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &bOkOutINT);
			LhsVar(2)=Rhs+2;
			if (bOkOutINT) {FREE(bOkOutINT);bOkOutINT=NULL;}
		}
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
