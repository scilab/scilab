/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
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
#include <string.h>
#include "gw_polynomials.h"
#include "machine.h"
/*--------------------------------------------------------------------------*/
extern int C2F(intptriu)(int *job, int *id);
/*--------------------------------------------------------------------------*/
int sci_ptril(char *fname, unsigned long fname_len)
{
    static int id[6];
    int job = 0;
    C2F(intptriu)(&job, id);
    return 0;
}
/*--------------------------------------------------------------------------*/
