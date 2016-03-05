/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
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

#ifndef __VFINITE__
#define __VFINITE__

#include "machine.h"
#include "doublecomplex.h"
#include "dynlib_elementary_functions.h"


ELEMENTARY_FUNCTIONS_IMPEXP int C2F(vfinite)(int *n, double *v);
ELEMENTARY_FUNCTIONS_IMPEXP int C2F(vfiniteComplex)(int *n, doublecomplex *v);

#endif /* !__VFINITE__ */
