/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include <errno.h>
#include "merror.h"
#include "filesmanagement.h"
/*--------------------------------------------------------------------------*/
void C2F(merror) (int *fd, int *res)
{
    FILE *fa = GetFileOpenedInScilab(*fd);
    if (fa)
    {

        *res = ferror(fa);
    }
    else
    {
        *res = EINVAL;
    }
}
/*--------------------------------------------------------------------------*/

