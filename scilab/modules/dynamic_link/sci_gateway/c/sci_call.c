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
#include "gw_dynamic_link.h"
#include "stack-c.h"
#include "Scierror.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intfort)(char *fname,unsigned long fname_len); /* fortran */
/*--------------------------------------------------------------------------*/
int C2F(sci_fort)(char *fname,unsigned long fname_len)
{
	return C2F(sci_call)(fname,fname_len);
}
/*--------------------------------------------------------------------------*/
int C2F(sci_call)(char *fname,unsigned long fname_len)
{
	if (Rhs == 0)
	{
		Error(39);
	}
	else C2F(intfort)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
