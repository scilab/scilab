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
#include "Scierror.h"
#include "localization.h"
/*--------------------------------------------------------------------------*/
int C2F(sci_hidetoolbar) _PARAMS((char *fname,unsigned long l))
{
	static int l1, m1, n1;	

	CheckRhs(1,1);
	CheckLhs(1,1);

	if ( GetType(1) == sci_matrix )
	{
		int num_win=-2;

		GetRhsVar(1,MATRIX_OF_INTEGER_DATATYPE,&m1,&n1,&l1);
		num_win=*istk(l1);

		/* TO DO interface with java */

		LhsVar(1)=0;
		C2F(putlhsvar)();
	}
	else
	{
		Scierror(999,_("Input argument incorrect type.\n"));
		return 0;
	}
	return 0;
}
/*--------------------------------------------------------------------------*/
