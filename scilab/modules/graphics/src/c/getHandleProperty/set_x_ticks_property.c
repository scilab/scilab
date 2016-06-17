/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
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
/* file: set_x_ticks_property.c                                           */
/* desc : function to modify in Scilab the x_ticks field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "GetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "CheckTicksProperty.h"
#include "sci_malloc.h"
#include "BasicAlgos.h"
#include "DrawObjects.h"
#include "freeArrayOfString.h"
#include "loadTextRenderingAPI.h"
#include "sci_types.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_x_ticks_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL autoTicks = FALSE;
    BOOL status = FALSE;
    AssignedList * tlist     = NULL;
    int            nbTicsRow = 0   ;
    int            nbTicsCol = 0   ;

    double* userGrads = NULL;
    char** userLabels = NULL;

    if (!(valueType == sci_tlist))
    {
        Scierror(999, _("Wrong type for '%s' property: Typed list expected.\n"), "x_ticks");
        return SET_PROPERTY_ERROR;
    }

    tlist = createTlistForTicks(_pvCtx);

    if (tlist == NULL)
    {
        return SET_PROPERTY_ERROR;
    }

    /* locations */
    userGrads = createCopyDoubleMatrixFromList(_pvCtx, tlist, &nbTicsRow, &nbTicsCol);

    if (userGrads == NULL && nbTicsRow == -1)
    {
        Scierror(999, _("%s: No more memory.\n"), "set_x_ticks_property");
        destroyAssignedList(tlist);
        return SET_PROPERTY_ERROR;
    }

    /* Automatic ticks must be first deactivated in order to set user ticks */
    autoTicks = FALSE;

    setGraphicObjectProperty(iObjUID, __GO_X_AXIS_AUTO_TICKS__, &autoTicks, jni_bool, 1);

    status = setGraphicObjectProperty(iObjUID, __GO_X_AXIS_TICKS_LOCATIONS__, userGrads, jni_double_vector, nbTicsRow * nbTicsCol);

    if (status == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "x_ticks");
        destroyAssignedList(tlist);
        FREE(userGrads);
        return SET_PROPERTY_ERROR;
    }

    /*  labels */
    // Here we check the size of "locations" instead of "labels", but they have the same size.
    // We need to check the size to not be 0 because an empty matrix is a matrix of double
    // and 'getCurrentStringMatrixFromList' expect a matrix of string (see bug 5148).
    // P.Lando
    if (nbTicsCol * nbTicsRow)
    {
        userLabels = getCurrentStringMatrixFromList(_pvCtx, tlist, &nbTicsRow, &nbTicsCol);
        /* Check if we should load LaTex / MathML Java libraries */
        loadTextRenderingAPI(userLabels, nbTicsCol, nbTicsRow);

        setGraphicObjectProperty(iObjUID, __GO_X_AXIS_TICKS_LABELS__, userLabels, jni_string_vector, nbTicsRow * nbTicsCol);
    }
    else
    {
        /* To be implemented */
#if 0
        ppSubWin->axes.u_xlabels = NULL;
#endif
    }

    if (userGrads != NULL)
    {
        FREE(userGrads);
    }
    freeAllocatedMatrixOfString(nbTicsRow, nbTicsCol, userLabels);
    destroyAssignedList(tlist);

    return SET_PROPERTY_SUCCEED;

}
/*------------------------------------------------------------------------*/
