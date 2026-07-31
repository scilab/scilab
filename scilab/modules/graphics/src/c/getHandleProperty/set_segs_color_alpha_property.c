/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2026 - Dassault Systèmes S.E. - Vincent COUVERT
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

/*------------------------------------------------------------------------*/
/* file: set_segs_color_alpha_property.c                                  */
/* desc : function to modify in Scilab the segs_color_alpha field         */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "sci_malloc.h"
#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_segs_color_alpha_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int iNbSegs = 0;
    int* piNbSegs = &iNbSegs;
    int i = 0;
    double* alphas = NULL;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "segs_color_alpha");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_NUMBER_ARROWS__, jni_int, (void**)&piNbSegs);

    if (piNbSegs == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "segs_color_alpha");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow * nbCol != 1 && nbRow * nbCol != iNbSegs)
    {
        Scierror(999, _("Wrong size for '%s' property: %d or %d elements expected.\n"), "segs_color_alpha", 1, iNbSegs);
        return SET_PROPERTY_ERROR;
    }

    for (i = 0; i < nbRow * nbCol; i++)
    {
        if (((double*)_pvData)[i] < 0.0 || ((double*)_pvData)[i] > 1.0)
        {
            Scierror(999, _("Wrong value for '%s' property: Must be between 0 and 1.\n"), "segs_color_alpha");
            return SET_PROPERTY_ERROR;
        }
    }

    if (nbRow * nbCol == 1)
    {
        /* 1-element array which is internally duplicated for every arrow */
        double value = ((double*)_pvData)[0];
        status = setGraphicObjectProperty(iObjUID, __GO_LINE_COLOR_ALPHA__, &value, jni_double_vector, 1);
    }
    else
    {
        alphas = (double*) MALLOC(iNbSegs * sizeof(double));
        if (alphas == NULL)
        {
            Scierror(999, _("%s: No more memory.\n"), "set_segs_color_alpha_property");
            return SET_PROPERTY_ERROR;
        }

        for (i = 0; i < iNbSegs; i++)
        {
            alphas[i] = ((double*)_pvData)[i];
        }

        status = setGraphicObjectProperty(iObjUID, __GO_LINE_COLOR_ALPHA__, alphas, jni_double_vector, iNbSegs);
        FREE(alphas);
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }

    Scierror(999, _("'%s' property does not exist for this handle.\n"), "segs_color_alpha");
    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
