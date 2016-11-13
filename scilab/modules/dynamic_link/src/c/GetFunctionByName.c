/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) INRIA -
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
#include "GetFunctionByName.h"
#include "dynamic_link.h"
#include "sci_malloc.h"
/*--------------------------------------------------------------------------*/
static BOOL SearchComp(FTAB *Ftab, char *op, void (**realop) ());
static void Emptyfunc(void) {}
/*--------------------------------------------------------------------------*/
voidf GetFunctionByName (char *name, int *rep, FTAB *table)
{
    return NULL;
}
/*--------------------------------------------------------------------------*/
static BOOL SearchComp(FTAB *Ftab, char *op, void (**realop) ())
{
    int i = 0;
    while ( Ftab[i].name != (char *) 0)
    {
        int j = 0;
        j = strcmp(op, Ftab[i].name);
        if ( j == 0 )
        {
            *realop = Ftab[i].f;
            return TRUE;
        }
        else
        {
            if ( j <= 0)
            {
                return FALSE;
            }
            else
            {
                i++;
            }
        }
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
