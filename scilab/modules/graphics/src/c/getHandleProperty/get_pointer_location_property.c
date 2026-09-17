/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2021-2026 - Stéphane Mottelet
 *
 * This file is hereby licensed under the terms of the GNU GPL v3.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*-------------------------------------------------------------------------*/
/* file: get_pointer_location_property.c                                   */
/* desc : function to retrieve in Scilab  mouse pointer screen coordinates */
/*-------------------------------------------------------------------------*/

#include <stdlib.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "GetRootProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_pointer_location_property(void* _pvCtx, int iObjUID)
{
    return GetRootProperty(_pvCtx, iObjUID, "pointer_location");
}
/*------------------------------------------------------------------------*/
