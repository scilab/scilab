/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - DIGITEO - Bruno JOFRET
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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

int get_pixel_drawing_mode_property(void* _pvCtx, char* pobjUID)
{
    int iPixelDrawingMode = 0;
    int* pixelDrawingMode = &iPixelDrawingMode;
    getGraphicObjectProperty(pobjUID, __GO_PIXEL_DRAWING_MODE__, jni_int, (void**)&pixelDrawingMode);

    if (pixelDrawingMode == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "pixel_drawing_mode");
        return -1 ;
    }

    return sciReturnString(_pvCtx, getPixelMode (*pixelDrawingMode));

}

/*------------------------------------------------------------------------*/
