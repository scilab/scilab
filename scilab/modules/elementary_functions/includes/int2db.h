/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#ifndef __INT2DB_H__
#define __INT2DB_H__

#include "machine.h" /* C2F */
#include "dynlib_elementary_functions.h"

/**
* translate a int vector to double precision vector
* @param n size of dx vector
* @param dx int vector
* @param incx increment order
* @param dy double precision vector
* @param incy increment order
*/
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(int2db)(int *n, int *dx, int *incx, double *dy, int *incy);

#endif /* __INT2DB_H__ */
/*--------------------------------------------------------------------------*/
