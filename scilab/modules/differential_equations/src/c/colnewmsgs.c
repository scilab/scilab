/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
 * ...
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

#include "sciprint.h"
#include "Scierror.h" /* bsiz */
#include "dynlib_differential_equations.h"
/*--------------------------------------------------------------------------*/
/* This function is used in colnew.f                                        */
/*--------------------------------------------------------------------------*/
DIFFERENTIAL_EQUATIONS_IMPEXP void C2F(colnewmsgs)(char *msg);
/*--------------------------------------------------------------------------*/
void C2F(colnewmsgs)(char *msg)
{
    int iLen = bsiz;

    while (msg[iLen - 1] == ' ')
    {
        iLen--;
    }
    msg[iLen] = '\0';

    sciprint("%s\n", msg);
}
/*--------------------------------------------------------------------------*/

