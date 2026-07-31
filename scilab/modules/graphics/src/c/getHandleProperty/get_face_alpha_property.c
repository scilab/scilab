/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: get_face_alpha_property.c                                        */
/* desc : function to retrieve in Scilab the face_alpha field of a handle */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_face_alpha_property(void* _pvCtx, int iObjUID)
{
    double dblFaceAlpha = 1.0;
    double* pdblFaceAlpha = &dblFaceAlpha;

    getGraphicObjectProperty(iObjUID, __GO_FACE_ALPHA__, jni_double, (void**) &pdblFaceAlpha);

    if (pdblFaceAlpha == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "face_alpha");
        return NULL;
    }

    return sciReturnDouble(dblFaceAlpha);
}
/*------------------------------------------------------------------------*/
