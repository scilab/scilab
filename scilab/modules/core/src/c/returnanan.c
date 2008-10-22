/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include "returnanan.h"
/*--------------------------------------------------------------------------*/
double C2F(returnanan)(void)
{
	static int first = 1;
	static double nan = 1.0;

	if ( first )
	{
		nan = (nan - (double) first)/(nan - (double) first);
		first = 0;
	}
	return (nan);
}
/*--------------------------------------------------------------------------*/
