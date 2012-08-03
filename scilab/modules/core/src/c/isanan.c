/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */
#include <stdio.h>
#include <math.h>
#include "isanan.h"


#ifdef _MSC_VER
#include <float.h>
#endif


/* testing Nan returns 1 if a Nan is found and 0 elsewhere */
/* should be changed to use a libm isnan function when possible */

int C2F(isanan)(double *x)
{
#ifdef _MSC_VER
    return (_isnan(*x) == 1);
#else
    return ((!( *x <= 1.0 )) && (!( *x >= 1.0 )));
#endif
}

