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

#include "errmds.h"
#include "stack-def.h"
/*--------------------------------------------------------------------------*/ 
int C2F(errmds)(int *num, int *imess, int *imode)
{
	/* 100000 is a constant used in fortran to code errct value :( */
	#define ERRCATCH_CONST_ERRCT 100000
	*num = 0;
	if (C2F(errgst).errct > 0) 
	{
		*num = C2F(errgst).errct - ERRCATCH_CONST_ERRCT * (int)( C2F(errgst).errct / ERRCATCH_CONST_ERRCT );
		*imode = C2F(errgst).errct / ERRCATCH_CONST_ERRCT;
	} 
	else if (C2F(errgst).errct < 0) 
	{
		*num = -1;
		*imode = -C2F(errgst).errct / ERRCATCH_CONST_ERRCT;
	}
	*imess = *imode / 8;
	*imode = *imode - 8* (*imess);
	return 0;
} 
/*--------------------------------------------------------------------------*/ 
