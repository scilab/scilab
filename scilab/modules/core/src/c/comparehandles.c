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

/*--------------------------------------------------------------------------*/
#include "machine.h"
/* used to test if two graphic handles are equal or not */
int C2F(comparehandles)(long long *h1, long long *h2)
{
    if (*h1 != *h2)
    {
        return 0;
    }
    return 1;
}

