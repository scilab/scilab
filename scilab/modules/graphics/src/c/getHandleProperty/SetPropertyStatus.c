/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: SetPropertySatus.c                                               */
/* desc : define the different type of return status for the sci_set      */
/*        routine                                                         */
/*------------------------------------------------------------------------*/

#include "SetPropertyStatus.h"

/*--------------------------------------------------------------------------*/
SetPropertyStatus sciSetFinalStatus(SetPropertyStatus status1, SetPropertyStatus status2)
{
    if (status1 == SET_PROPERTY_ERROR || status2 == SET_PROPERTY_ERROR)
    {
        /* problem */
        return SET_PROPERTY_ERROR;
    }
    else if (status1 == SET_PROPERTY_UNCHANGED && status2 == SET_PROPERTY_UNCHANGED)
    {
        /* nothing changed */
        return SET_PROPERTY_UNCHANGED;
    }
    else
    {
        /* everything should be ok */
        return SET_PROPERTY_SUCCEED;
    }
}

/*--------------------------------------------------------------------------*/
SetPropertyStatus sciSetNoRedrawStatus(SetPropertyStatus status)
{
    if (status == SET_PROPERTY_SUCCEED)
    {
        return SET_PROPERTY_UNCHANGED;
    }
    else
    {
        return status;
    }
}
/*--------------------------------------------------------------------------*/
