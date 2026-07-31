/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: get_segs_color_alpha_property.c                                  */
/* desc : function to retrieve in Scilab the segs_color_alpha field       */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "sci_malloc.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_segs_color_alpha_property(void* _pvCtx, int iObjUID)
{
    double* segsColorAlphas = NULL;
    int iNbSegs = 0;
    int* piNbSegs = &iNbSegs;

    getGraphicObjectProperty(iObjUID, __GO_LINE_COLOR_ALPHA__, jni_double_vector, (void**) &segsColorAlphas);

    if (segsColorAlphas == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "segs_color_alpha");
        return NULL;
    }

    getGraphicObjectProperty(iObjUID, __GO_NUMBER_ARROWS__, jni_int, (void**) &piNbSegs);
    if (piNbSegs == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "segs_color_alpha");
        return NULL;
    }

    return sciReturnRowVector(segsColorAlphas, iNbSegs);
}
/*------------------------------------------------------------------------*/
