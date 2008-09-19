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
#include <string.h>
#include <stdio.h>
#include "gw_core.h"
#include "stack-c.h"
#include "version.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#ifdef _MSC_VER
#include "getdynamicDebugInfo_Windows.h"
#include "getstaticDebugInfo_Windows.h"
#else
#include "getstaticdebuginfo.h"
#include "getdynamicdebuginfo.h"
#endif
/*--------------------------------------------------------------------------*/
int C2F(sci_getdebuginfo)(char *fname,unsigned long fname_len)
{
	char **outputDynamicList = NULL;
	char **outputStaticList = NULL;
	static int n1=1,m1=0;
	static int n2=1,m2=0;

	CheckRhs(0,0);
	CheckLhs(0,2);

#ifdef _MSC_VER
	outputDynamicList = getDynamicDebugInfo_Windows(&m1);
	outputStaticList = getStaticDebugInfo_Windows(&m2);
#else
	outputDynamicList = getDynamicDebugInfo(&m1);
	outputStaticList = getStaticDebugInfo(&m2);
#endif

	CreateVarFromPtr(Rhs+1,MATRIX_OF_STRING_DATATYPE, &m1, &n1, outputDynamicList);
	LhsVar(1) = Rhs+1;

	if (Lhs == 2)
	{
		CreateVarFromPtr(Rhs+2,MATRIX_OF_STRING_DATATYPE, &m2, &n2, outputStaticList);
		LhsVar(2) = Rhs+2;
	}
	C2F(putlhsvar)();

	freeArrayOfString(outputDynamicList,m1);
	freeArrayOfString(outputStaticList,m2);
	return 0;
}
/*--------------------------------------------------------------------------*/
