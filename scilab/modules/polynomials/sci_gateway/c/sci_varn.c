/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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
#include "stack-c.h"
#include "gw_polynomials.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intvarn)(int *id);
/*--------------------------------------------------------------------------*/
int C2F(sci_varn)(char *fname,unsigned long fname_len)
{
	static int id[6];
	C2F(intvarn)(id);
	return 0;
}
/*--------------------------------------------------------------------------*/
