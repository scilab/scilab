/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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
/* file: set_current_entity_property.c                                    */
/* desc : function to modify in Scilab the current_entity field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CurrentObject.h"
#include "HandleManagement.h"
#include "sci_types.h"

/*------------------------------------------------------------------------*/
int set_current_entity_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iCurEntity = 0;

    if (iObjUID != 0)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_entity");
        return SET_PROPERTY_ERROR;
    }

    if (valueType != sci_handles)
    {
        Scierror(999, _("Wrong type for '%s' property: Handle expected.\n"), "current_entity");
        return SET_PROPERTY_ERROR;
    }

    iCurEntity = getObjectFromHandle((long)((long long*)_pvData)[0]);

    if (iCurEntity == 0)
    {
        Scierror(999, _("Wrong value for '%s' property: Must be a valid handle.\n"), "current_entity");
        return SET_PROPERTY_ERROR;
    }

    setCurrentObject(iCurEntity);
    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
