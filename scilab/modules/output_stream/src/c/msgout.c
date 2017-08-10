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

#include "msgout.h"
#include "msgstore.h"
#include "basout.h"
/*--------------------------------------------------------------------------*/
int C2F(msgout)(int *io, int *lunit, char *str, long int str_len)
{
    static int ierr;
    int i__1 = str_len;

    C2F(basout)(io, lunit, str, str_len);
    ierr = C2F(msgstore)(str, &i__1);

    return 0;
}
/*--------------------------------------------------------------------------*/
