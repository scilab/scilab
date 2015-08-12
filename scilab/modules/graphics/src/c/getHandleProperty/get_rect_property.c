/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
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
int get_rect_property(void* _pvCtx, int iObjUID)
{
    double * rect = NULL;
    double _rect[4];

    getGraphicObjectProperty(iObjUID, __GO_DATA_MODEL_MATPLOT_BOUNDS__, jni_double_vector, (void **)&rect);

    if (rect == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "rect");
        return -1;
    }

    _rect[0] = rect[0];
    _rect[1] = rect[2];
    _rect[2] = rect[1];
    _rect[3] = rect[3];

    return sciReturnRowVector(_pvCtx, _rect, 4);
}
/*------------------------------------------------------------------------*/
