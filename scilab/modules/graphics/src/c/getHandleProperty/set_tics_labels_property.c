/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: set_tics_labels_property.c                                       */
/* desc : function to modify in Scilab the tics_labels field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "BasicAlgos.h"
#include "loadTextRenderingAPI.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_tics_labels_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int iNbTicksLabels = 0;
    int* piNbTicksLabels = &iNbTicksLabels;
    char** stringVector = NULL;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: string expected.\n"), "tics_labels");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_NUMBER_TICKS_LABELS__, jni_int, (void**)&piNbTicksLabels);

    if (piNbTicksLabels == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_labels");
        return SET_PROPERTY_ERROR;
    }

    if (iNbTicksLabels > nbRow * nbCol)
    {
        Scierror(999, _("Wrong size for '%s' property: At least %d elements expected.\n"), "tics_labels", iNbTicksLabels);
        return SET_PROPERTY_ERROR;
    }

    stringVector = createCopyStringMatrixFromStack(_pvData, nbRow * nbCol);

    /* Check if we should load LaTex / MathML Java libraries */
    loadTextRenderingAPI(stringVector, nbRow * nbCol, 1);

    status = setGraphicObjectProperty(iObjUID, __GO_TICKS_LABELS__, stringVector, jni_string_vector, nbRow * nbCol);

    destroyStringArray(stringVector, nbRow * nbCol);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "tics_labels");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
