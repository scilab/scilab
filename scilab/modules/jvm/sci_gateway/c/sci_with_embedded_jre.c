
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
#include "gw_jvm.h"
#include "MALLOC.h"
#include "stack-c.h"
#include "JVM_functions.h"
/*--------------------------------------------------------------------------*/
int sci_with_embedded_jre(char *fname,unsigned long fname_len)
{
	static int n1=0,l1=0;
	n1=1;
	CreateVar(Rhs+1,MATRIX_OF_BOOLEAN_DATATYPE, &n1,&n1,&l1);
	*istk(l1)=(int)withEmbeddedJRE();
	
	LhsVar(1)=Rhs+1;
	C2F(putlhsvar)();
	return 0;
}
/*--------------------------------------------------------------------------*/
