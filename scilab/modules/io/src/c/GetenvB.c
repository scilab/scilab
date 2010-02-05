/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) INRIA - Sylvestre LEDRU
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
/*--------------------------------------------------------------------------*/ 
#include "GetenvB.h"
#include "getenvc.h"
/*--------------------------------------------------------------------------*/
void GetenvB(char *name, char *env, int len)
{
	int ierr = 0, one = 1;
	C2F(getenvc)(&ierr,name,env,&len,&one);
	if (ierr == 0) 
	{
		char *last = &env[len-1];
		while ( *last == ' ' ) { last = '\0' ; } 
		last--;
	}
	else 
	{
		env[0] = '\0' ;
	}  
}
/*--------------------------------------------------------------------------*/
