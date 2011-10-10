/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * Copyright (C) 2009 - DIGITEO - Allan CORNET
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
#include "freeArrayOfString.h"
#include "BOOL.h"
/*--------------------------------------------------------------------------*/
int sci_getlongpathname(char *fname,unsigned long l)
{
	CheckRhs(0,1);
	CheckLhs(1,2);

	if (GetType(1) == sci_strings)
	{
		int n1 = 0,m1 = 0, m1n1 = 0;
		char **LongNames = NULL;
		char **ShortNames = NULL;
		BOOL *bOK = NULL;
		int i = 0;

		GetRhsVar(1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,&ShortNames);
		m1n1 = m1*n1;

		if (m1n1 > 0)
		{
			LongNames = (char**)MALLOC(sizeof(char*)* (m1n1));
			bOK =(BOOL*)MALLOC(sizeof(BOOL)*(m1n1));

			if ( (LongNames == NULL) || (bOK == NULL) )
			{
				freeArrayOfString(ShortNames,m1n1);
				Scierror(999,"%s: Memory allocation error.\n",fname);
				return 0;
			}
		}

		for (i = 0;i < m1n1; i++)
		{
			LongNames[i] = getlongpathname(ShortNames[i],&bOK[i]);
		}
		freeArrayOfString(ShortNames, m1n1);

		CreateVarFromPtr( Rhs+1,MATRIX_OF_STRING_DATATYPE,&m1,&n1,LongNames);
		LhsVar(1) = Rhs+1;

		if (Lhs == 2) 
		{
			CreateVarFromPtr(Rhs+2,MATRIX_OF_BOOLEAN_DATATYPE, &m1, &n1, &bOK);
			LhsVar(2) = Rhs+2;
		}

		freeArrayOfString(LongNames, m1n1);
		if (bOK) {FREE(bOK); bOK = NULL;}

        PutLhsVar();
	}
	else
	{
		Scierror(999,_("%s: Wrong type for input argument: A string expected.\n"),fname);
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
