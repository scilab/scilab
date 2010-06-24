
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET , Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
#include "gw_cacsd2.h"
/*--------------------------------------------------------------------------*/
extern int C2F(scitzer)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int sci_tzer(char *fname,unsigned long fname_len)
{
	CheckRhs(4,4);
	CheckLhs(2,3);
	C2F(scitzer)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
