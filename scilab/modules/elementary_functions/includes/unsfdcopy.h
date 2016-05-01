/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Sylvestre LEDRU
 * Copyright (C) 2010 - DIGITEO - Allan CORNET
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
