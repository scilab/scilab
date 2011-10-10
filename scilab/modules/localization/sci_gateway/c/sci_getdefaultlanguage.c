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
#include "defaultlanguage.h"
#include "Scierror.h"
#include "MALLOC.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int sci_getdefaultlanguage(char *fname,unsigned long fname_len)
{
	static int n1 = 0,m1 = 0,l1 = 0;
	
	CheckRhs(0,0);
	CheckLhs(0,1);

	m1= (int)strlen(SCILABDEFAULTLANGUAGE);
	n1=1;

	CreateVar( Rhs+1,STRING_DATATYPE,&m1,&n1,&l1);
	strcpy(cstk(l1), SCILABDEFAULTLANGUAGE );

	LhsVar(1) = Rhs+1;
	PutLhsVar();
	return 0;
}
/*--------------------------------------------------------------------------*/
