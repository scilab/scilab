/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include "sci_getlanguage.h"
#include "setgetlanguage.h"
#include "gw_localization.h"
#include "MALLOC.h"
/*--------------------------------------------------------------------------*/ 
int C2F(sci_getlanguagealias)(char *fname,unsigned long fname_len)
{
	static int l1,n1,m1;
	char *Output=NULL;

	CheckRhs(0,0);
	CheckLhs(0,1);

	Output=getlanguagealias();

	if (Output)
	{
		n1=1;
		CreateVarFromPtr( Rhs+1,STRING_DATATYPE,(m1=(int)strlen(Output), &m1),&n1,&Output);
		LhsVar(1) = Rhs+1;
		PutLhsVar();;	
		if (Output) {FREE(Output);Output=NULL;}
	}
	else
	{
		m1=0;
		n1=0;
		l1=0;
		CreateVar(Rhs+1,MATRIX_OF_DOUBLE_DATATYPE,  &m1, &n1, &l1);
		LhsVar(1)=Rhs+1;
		PutLhsVar();;
		return 0;
	}

	
	return 0;
}
/*--------------------------------------------------------------------------*/ 
