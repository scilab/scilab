/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include "gw_core.h"
#include "machine.h"
#include "stack-c.h"
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
extern int C2F(where)(void);
/*--------------------------------------------------------------------------*/
int C2F(sci_where)(char *fname,unsigned long fname_len)
{
	if (Rhs > 0)
	{
		Scierror(39,_("%s: Wrong number of input arguments.\n"),"where");
		return 0;
	}

	if (Lhs != 2)
	{
		Scierror(41,_("%s: Wrong number of output arguments: %d expected.\n"),"where",2);
		return 0;
	}

 	C2F(where)();
	return 0;
}
/*--------------------------------------------------------------------------*/
