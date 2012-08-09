/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
int set_current_entity_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    char* curEntity = NULL ;

    if (pobjUID != NULL)
    {
        /* This property should not be called on an handle */
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "current_entity");
        return -1;
    }

    if ( !( valueType == sci_handles ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Handle expected.\n"), "current_entity");
        return SET_PROPERTY_ERROR ;
    }

    curEntity = (char*)getObjectFromHandle( getHandleFromStack( stackPointer ) ) ;

    if ( curEntity == NULL )
    {
        Scierror(999, _("Wrong value for '%s' property: Must be a valid handle.\n"), "current_entity");
        return SET_PROPERTY_ERROR ;
    }

    setCurrentObject(curEntity) ;
    return SET_PROPERTY_SUCCEED ;
}
/*------------------------------------------------------------------------*/
