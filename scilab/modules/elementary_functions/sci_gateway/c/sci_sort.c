/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - DIGITEO - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*-----------------------------------------------------------------------------------*/ 
#include <string.h>
#include "gw_elementary_functions.h"
#include "stack-c.h"
/*-----------------------------------------------------------------------------------*/
extern int C2F(intsort) (int *id);
extern int C2F(intssort) (int *id);
/*-----------------------------------------------------------------------------------*/
int C2F(sci_sort) (char *fname,unsigned long fname_len)
{
	/* TO DO : rewrite sort and merge with gsort */
	/* compatibility with scilab 4.x */

	static int id[6];
	if ( VarType(1) == sci_strings ) 
	{
		C2F(intssort)(id);
	}
	else
	{
		C2F(intsort)(id);
	}
	return 0;
}
/*-----------------------------------------------------------------------------------*/

