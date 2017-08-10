/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Scilab Enterprises - Calixte DENIZET
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
/* file: get_colors_property.c                                            */
/* desc : function to get in Scilab the polyline colors                */
/*------------------------------------------------------------------------*/

#include "SetPropertyStatus.h"
#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_colors_property(void* _pvCtx, int iObjUID)
{
    int * colors = NULL;
    int numColors = 0;
    int * piNumColors = &numColors;
    int colorSet = 0;
    int *piColorSet = &colorSet;

    getGraphicObjectProperty(iObjUID, __GO_COLOR_SET__, jni_bool, (void **)&piColorSet);
    if (piColorSet == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "colors");
        return NULL;
    }

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_COLORS__, jni_int_vector, (void **)&colors);
    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_NUM_COLORS__, jni_int, &piNumColors);

    if (colors == NULL || numColors == 0)
    {
        return sciReturnEmptyMatrix();
    }

    return sciReturnRowVectorFromInt(colors, numColors);
}
/*------------------------------------------------------------------------*/
