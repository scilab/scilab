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

#include "machine.h"
#include "core_math.h"
#include "finite.h"

#if !(defined HAVE_FINITE) && (defined hppa)

#include <math.h>

int finite(double x)
{
    if (isfinite(x) == 0)
    {
        return 0;
    }
    return 1;
}

#else
#if !(defined HAVE_FINITE)

typedef unsigned int __uint32_t;
typedef union
{
    struct
    {
        __uint32_t lsw;
        __uint32_t msw;
    } parts;
    double value;
} ieee_double_shape_type;

int finite(double x)
{
    int hx;
    ieee_double_shape_type gh_u;

    gh_u.value = x;
    hx = gh_u.parts.msw;
    return  (int)((__uint32_t)((hx & 0x7fffffff) - 0x7ff00000) >> 31);
}


#endif
#endif

int finiteComplex(doublecomplex x)
{
    return (finite(x.r) && finite(x.i));
}