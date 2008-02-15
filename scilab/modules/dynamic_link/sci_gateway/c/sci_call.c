/*--------------------------------------------------------------------------*/
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
#include "machine.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intfort) _PARAMS((char *fname,unsigned long fname_len)); /* fortran */
/*--------------------------------------------------------------------------*/
int C2F(sci_fort) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intfort)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
int C2F(sci_call) _PARAMS((char *fname,unsigned long fname_len))
{
	C2F(intfort)(fname,fname_len);
	return 0;
}
/*--------------------------------------------------------------------------*/
