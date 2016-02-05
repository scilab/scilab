/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
/* file: get_image_type_property.c                                         */
/* desc : function to get in Scilab the image_type field of             */
/*        a matplot handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "Matplot.h"

/*------------------------------------------------------------------------*/
void* get_image_type_property(void* _pvCtx, int iObjUID)
{
    int imagetype = 0;
    int * piImagetype = &imagetype;
    ImageType itype;

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_IMAGE_TYPE__, jni_int, (void **)&piImagetype);
    if (piImagetype == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "image_type");
        return NULL;
    }

    itype = (ImageType)imagetype;

    switch (itype)
    {
        case MATPLOT_RGB :
            return sciReturnString("rgb");
        case MATPLOT_RGBA :
            return sciReturnString("rgba");
        case MATPLOT_GRAY :
            return sciReturnString("gray");
        case MATPLOT_INDEX :
            return sciReturnString("index");
        case MATPLOT_RED :
            return sciReturnString("red");
        case MATPLOT_GREEN :
            return sciReturnString("green");
        case MATPLOT_BLUE :
            return sciReturnString("blue");
        case MATPLOT_RGB_332 :
            return sciReturnString("rgb332");
        case MATPLOT_RGB_444 :
            return sciReturnString("rgb444");
        case MATPLOT_RGBA_4444 :
            return sciReturnString("rgba444");
        case MATPLOT_RGB_555 :
            return sciReturnString("rgb555");
        case MATPLOT_RGBA_5551 :
            return sciReturnString("rgba5551");
        case MATPLOT_ARGB :
            return sciReturnString("argb");
        default :
            Scierror(999, _("Wrong value for '%s' property.\n"), "image_type");
            return NULL;
    }
}
/*------------------------------------------------------------------------*/
