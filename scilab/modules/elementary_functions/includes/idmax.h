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
#ifndef __IDMAX_H__
#define __IDMAX_H__

#include "machine.h" /* C2F */
#include "dynlib_elementary_functions.h"

/**
* finds the index of the first element having maximum value
 * this function return 1 if x has only nan components : may be this is not a good behavior
 * this function doesn't test if n<1 or incx<1 : this is done
 *          by the scilab interface
 * @param n size of x
 * @param x vector
 * @param incr indice
 * @return 0
*/
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(idmax)(int *n, double *x, int *incx);

#endif /* __IDMAX_H__ */
/*--------------------------------------------------------------------------*/
