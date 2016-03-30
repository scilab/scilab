/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * Copyright (C) 2012 - 2016 - Scilab Enterprises
 *
 * This file is hereby licensed under the terms of the GNU GPL v2.0,
 * pursuant to article 5.3.4 of the CeCILL v.2.1.
 * This file was originally licensed under the terms of the CeCILL v2.1,
 * and continues to be available under such terms.
 * For more information, see the COPYING file which you should have received
 * along with this program.
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
