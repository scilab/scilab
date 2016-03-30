/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA
 * Copyright (C) ENPC
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

