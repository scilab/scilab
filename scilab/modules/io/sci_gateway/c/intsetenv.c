/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2005 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/* setenv interface */
#include <stdio.h>
#include <string.h>

#include "MALLOC.h" /* MALLOC */
#include "stack-c.h"
#include "setenvc.h"
#include "intsetenv.h"
#include "gw_io.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
static int ReturnValueSetenv(int value);
/*--------------------------------------------------------------------------*/
int	C2F(intsetenv)(char *fname, unsigned long len)
{
	static int l1,n1,m1;
	int TypeVar1=GetType(1);
	int TypeVar2=GetType(2);

	Rhs=Max(0,Rhs);
	CheckRhs(2,2);
	CheckLhs(0,1);

	if ( (TypeVar1 == sci_strings) && (TypeVar2 == sci_strings) )
	{
		char *param1=NULL,*param2=NULL;

		GetRhsVar(1,STRING_DATATYPE,&m1,&n1,&l1);
		param1=cstk(l1);

		GetRhsVar(2,STRING_DATATYPE,&m1,&n1,&l1);
		param2=cstk(l1);

		ReturnValueSetenv(setenvc(param1,param2));
	}
	else
	{
		if(TypeVar1 != sci_strings)
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,1);
		}
		else
		{
			Scierror(999,_("%s: Wrong type for input argument #%d: Matrix of strings expected.\n"),fname,2);
		}
	}
	
	return 0;
}
/*--------------------------------------------------------------------------*/
static int ReturnValueSetenv(int value)
{
	static int n1;
	int *paramoutINT=(int*)MALLOC(sizeof(int));

	if (value == TRUE) *paramoutINT=(int)(TRUE);
	else  *paramoutINT=(int)(FALSE);

	n1=1;
	CreateVarFromPtr(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1, &n1, &paramoutINT);
    LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	if (paramoutINT) {FREE(paramoutINT);paramoutINT=NULL;}
	return 0;
}
/*--------------------------------------------------------------------------*/
