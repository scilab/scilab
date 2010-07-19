/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
