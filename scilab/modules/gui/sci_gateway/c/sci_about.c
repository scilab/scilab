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
 
#include "gw_gui.h"
#include "stack-c.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_about) _PARAMS((char *fname,unsigned long l))
{
	Rhs=Max(Rhs,0);
	CheckRhs(0,0) ;
	CheckLhs(0,1) ;

	/* TO DO interface with java */


	LhsVar(1) = 0;
	C2F(putlhsvar)();	
	
	return 0;
}
/*--------------------------------------------------------------------------*/
