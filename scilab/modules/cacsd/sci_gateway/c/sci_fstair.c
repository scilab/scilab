
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
#include "machine.h"
#include "stack-c.h"
#include "gw_cacsd0.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intfstair)(char *fname,unsigned long fname_len);
/*--------------------------------------------------------------------------*/
int C2F(sci_fstair)(char *fname,unsigned long fname_len)
{
	C2F(intfstair)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
