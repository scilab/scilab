/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2021-2026 - Stéphane Mottelet
 *
 * This file is hereby licensed under the terms of the GNU GPL v3.0,
 * For more information, see the COPYING file which you should have received
 * along with this program.
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_screen_property.c                                         */
/*------------------------------------------------------------------------*/

#include <stdio.h>
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "sci_malloc.h"
#include "api_scilab.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"


/*------------------------------------------------------------------------*/
#include "GetRootProperty.h"

/*------------------------------------------------------------------------*/
void*  get_screensize_property(void* _pvCtx, int iObjUID)
{
    return GetRootProperty(_pvCtx, iObjUID, "screensize");
}
void*  get_screendepth_property(void* _pvCtx, int iObjUID)
{
    return GetRootProperty(_pvCtx, iObjUID, "screendepth");
}

/*------------------------------------------------------------------------*/


/*------------------------------------------------------------------------*/
