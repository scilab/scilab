
/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Sylvestre LEDRU
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
#ifndef __GENPROD_H__
#define __GENPROD_H__
#include "dynlib_integer.h"
#include "machine.h"
#include "def.h"

INTEGER_IMPEXP int C2F(genprod)(int *typ, int *n, void *dx, int *incx);
INTEGER_IMPEXP double C2F(genprodasdouble)(int *typ, int *n, void *dx, int *incx);
#endif /* __GENPROD_H__ */
