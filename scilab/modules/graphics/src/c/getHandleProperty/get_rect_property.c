/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
/* file: get_rect_property.c                                         */
/* desc : function to get in Scilab the rect field of             */
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

/*------------------------------------------------------------------------*/
void* get_rect_property(void* _pvCtx, int iObjUID)
{
    double * rect = NULL;
    double _rect[4];

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_BOUNDS__, jni_double_vector, (void **)&rect);

    if (rect == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "rect");
        return NULL;
    }

    _rect[0] = rect[0];
    _rect[1] = rect[2];
    _rect[2] = rect[1];
    _rect[3] = rect[3];

    return sciReturnRowVector(_rect, 4);
}
/*------------------------------------------------------------------------*/
