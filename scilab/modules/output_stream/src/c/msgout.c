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

#include "msgout.h"
#include "msgstore.h"
#include "basout.h"
/*--------------------------------------------------------------------------*/ 
int C2F(msgout)(int *io,int *lunit,char *str,long int str_len)
{
	static int ierr;
	int i__1=str_len;

    C2F(basout)(io, lunit, str, str_len);
    ierr = C2F(msgstore)(str, &i__1);

    return 0;
}
/*--------------------------------------------------------------------------*/ 
