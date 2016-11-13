/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
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
/* file: set_callback_type_property.c                                     */
/* desc : function to modify in Scilab the callback_type field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_callback_type_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int callbackType = 0;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: A Real scalar expected.\n"), "callback_type");
        return SET_PROPERTY_ERROR;
    }
    if (nbRow * nbCol != 1)
    {
        Scierror(999, _("Wrong size for '%s' property: A Real scalar expected.\n"), "callback_type");
        return SET_PROPERTY_ERROR;
    }

    callbackType = (int)((double*)_pvData)[0];

    /* Check the value */
    if ((callbackType < -1 || callbackType > 2) && callbackType != 10 && callbackType != 12)
    {
        Scierror(999, _("Wrong value for '%s' property: %d, %d, %d or %d expected.\n"), "callback_type", -1, 0, 1, 2);
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_CALLBACKTYPE__, &callbackType, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "callback_type");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
