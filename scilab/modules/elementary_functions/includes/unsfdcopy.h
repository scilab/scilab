/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* rewrite unsfdcopy.f */
/*--------------------------------------------------------------------------*/

#ifndef __UNSFDCOPY_H__
#define __UNSFDCOPY_H__

#include "machine.h"
#include "dynlib_elementary_functions.h"

/**
 * <long-description>
 *
 * @param n
 * @param dx
 * @param incx
 * @param dy
 * @param incy
 * @return <ReturnValue>
 */
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(unsfdcopy)(int *n, long long *dx, int *incx, long long *dy, int *incy);

#endif /* __UNSFDCOPY_H__ */
