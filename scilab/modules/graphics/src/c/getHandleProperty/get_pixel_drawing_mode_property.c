/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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
/* file: get_pixel_drawing_mode_property.c                                */
/* desc : function to retrieve in Scilab the pixel_drawing_mode field of  */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "pixel_mode.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/

void* get_pixel_drawing_mode_property(void* _pvCtx, int iObjUID)
{
    int iPixelDrawingMode = 0;
    int* pixelDrawingMode = &iPixelDrawingMode;
    getGraphicObjectProperty(iObjUID, __GO_PIXEL_DRAWING_MODE__, jni_int, (void**)&pixelDrawingMode);

    if (pixelDrawingMode == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "pixel_drawing_mode");
        return NULL;
    }

    return sciReturnString(getPixelMode (*pixelDrawingMode));

}

/*------------------------------------------------------------------------*/
