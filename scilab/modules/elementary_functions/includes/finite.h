/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#ifndef __FINITE_H__
#define __FINITE_H__

#include "doublecomplex.h"
#include "dynlib_elementary_functions.h"

#ifndef HAVE_FINITE
ELEMENTARY_FUNCTIONS_IMPEXP int finite(double x);
#endif

ELEMENTARY_FUNCTIONS_IMPEXP int finiteComplex(doublecomplex x);

#endif /* __FINITE_H__ */
