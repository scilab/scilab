/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006-2008 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*--------------------------------------------------------------------------*/
/* CORNET Allan */
/* INRIA 2006 */
/*--------------------------------------------------------------------------*/
#include <string.h>
#include "stack-c.h"
#include "basout.h"
#include "libinter.h"
/*--------------------------------------------------------------------------*/
int C2F(out)(char *str, long int str_len)
{
    static int io;
    C2F(basout)(&io, &C2F(iop).wte, str, (long int)strlen(str));
    return 0;
}
/*--------------------------------------------------------------------------*/
