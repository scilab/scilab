/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
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
/* file: set_old_style_property.c                                         */
/* desc : function to modify in Scilab the old_style field of             */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

/*------------------------------------------------------------------------*/
int set_old_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int b = tryGetBooleanValueFromStack(_pvData, valueType, nbRow, nbCol, "old_style");
    if (b == NOT_A_BOOLEAN_VALUE)
    {
        return SET_PROPERTY_ERROR;
    }

    if (b)
    {
        Scierror(999, _("Old graphic mode is no longer available. Please refer to the set help page.\n"));
        return SET_PROPERTY_ERROR;
    }
    else
    {
        return SET_PROPERTY_UNCHANGED;
    }
}
/*------------------------------------------------------------------------*/
