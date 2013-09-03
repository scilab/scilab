/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Allan CORNET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */
#include "scilabmode.h"
/*--------------------------------------------------------------------------*/
/* default mode is API */
static scilabMode CurrentScilabMode = SCILAB_API;
/*--------------------------------------------------------------------------*/
scilabMode getScilabMode(void)
{
    return CurrentScilabMode;
}
/*--------------------------------------------------------------------------*/
char * getScilabModeString(void)
{
    switch (CurrentScilabMode)
    {
        case SCILAB_API:
            return "API";
            break;
        case SCILAB_STD:
            return "STD";
            break;
        case SCILAB_NW:
            return "NW";
            break;
        case SCILAB_NWNI:
            return "NWNI";
            break;
        default:
            return "STD";
            break;
    }
}
/*--------------------------------------------------------------------------*/
BOOL setScilabMode(scilabMode newmode)
{
    if ( (newmode == SCILAB_API) || (newmode == SCILAB_STD) || (newmode == SCILAB_NW) || (newmode == SCILAB_NWNI) )
    {
        CurrentScilabMode = (scilabMode)newmode;
        return TRUE;
    }
    return FALSE;
}
/*--------------------------------------------------------------------------*/
int C2F(getscilabmode)(scilabMode *mode)
{
    *mode = getScilabMode();
    return 0;
}
/*--------------------------------------------------------------------------*/
