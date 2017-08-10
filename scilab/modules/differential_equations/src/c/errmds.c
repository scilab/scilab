/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA - Allan CORNET
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

#include "errmds.h"
#include "common_structure.h"

DIFFERENTIAL_EQUATIONS_IMPEXP STR_ERRGST C2F(errgst);

/*--------------------------------------------------------------------------*/
int C2F(errmds)(int *num, int *imess, int *imode)
{
    /* 100000 is a constant used in fortran to code errct value :( */
#define ERRCATCH_CONST_ERRCT 100000
    *num = 0;
    if (C2F(errgst).errct > 0)
    {
        *num = C2F(errgst).errct - ERRCATCH_CONST_ERRCT * (int)( C2F(errgst).errct / ERRCATCH_CONST_ERRCT );
        *imode = C2F(errgst).errct / ERRCATCH_CONST_ERRCT;
    }
    else if (C2F(errgst).errct < 0)
    {
        *num = -1;
        *imode = -C2F(errgst).errct / ERRCATCH_CONST_ERRCT;
    }
    *imess = *imode / 8;
    *imode = *imode - 8 * (*imess);
    return 0;
}
/*--------------------------------------------------------------------------*/
