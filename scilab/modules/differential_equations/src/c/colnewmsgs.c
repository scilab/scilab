/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2015 - Scilab Enterprises - Vincent COUVERT
 * ...
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

