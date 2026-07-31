/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: get_background_alpha_property.c                                  */
/* desc : function to retrieve in Scilab the background_alpha field of a  */
/*        non-surface handle (alias of face_alpha for surfaces).          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_background_alpha_property(void* _pvCtx, int iObjUID)
{
    double dblBackgroundAlpha = 1.0;
    double* pdblBackgroundAlpha = &dblBackgroundAlpha;

    getGraphicObjectProperty(iObjUID, __GO_BACKGROUND_ALPHA__, jni_double, (void**) &pdblBackgroundAlpha);

    if (pdblBackgroundAlpha == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "background_alpha");
        return NULL;
    }

    return sciReturnDouble(dblBackgroundAlpha);
}
/*------------------------------------------------------------------------*/
