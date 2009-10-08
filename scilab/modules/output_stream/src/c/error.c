/*
 * ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "do_error_number.h"
#include "MALLOC.h" /* NULL*/
#include "error_internal.h"
#include "msgstore.h"
/*--------------------------------------------------------------------------*/ 
int C2F(error)(int *n)
{
	return error_internal(n,NULL,ERROR_FROM_FORTRAN);
} 
/*--------------------------------------------------------------------------*/ 
void SciError(int n)
{
	C2F(error)(&n);
}

void SciStoreError(int n)
{
	C2F(errstore)(&n);
}
/*--------------------------------------------------------------------------*/
