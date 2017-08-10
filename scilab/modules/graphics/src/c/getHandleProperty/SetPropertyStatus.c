/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
