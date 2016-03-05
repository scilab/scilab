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
/* file: set_xtics_coord_property.c                                       */
/* desc : function to modify in Scilab the xtics_coord field of           */
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
#include "Format.h"
#include "sci_malloc.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_xtics_coord_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int N = 0;
    double* vector = NULL;
    char c_format[5];
    int iXNumberTicks = 0;
    int* piXNumberTicks = &iXNumberTicks;
    char** stringVector = NULL;
    int iTicksStyle = 0;
    int* piTicksStyle = &iTicksStyle;
    char ticksStyle = 0;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    if (nbRow != 1)
    {
        Scierror(999, _("Wrong size for '%s' property: Row vector expected.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_X_NUMBER_TICKS__, jni_int, (void**)&piXNumberTicks);

    if (piXNumberTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    if (iXNumberTicks == 1 && nbCol != 1)
    {
        Scierror(999, _("Wrong size for '%s' property: Scalar expected.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    if (iXNumberTicks != 1 && nbCol == 1)
    {
        Scierror(999, _("Wrong size for '%s' property: At least %d elements expected.\n"), "xtics_coord", 2);
        return SET_PROPERTY_ERROR;
    }

    /* what follows remains here as it was */
    status = setGraphicObjectProperty(iObjUID, __GO_X_TICKS_COORDS__, _pvData, jni_double_vector, nbCol);

    if (status == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iObjUID, __GO_TICKS_STYLE__, jni_int, (void**)&piTicksStyle);

    if (iTicksStyle == 0)
    {
        ticksStyle = 'v';
    }
    else if (iTicksStyle == 1)
    {
        ticksStyle = 'r';
    }
    else if (iTicksStyle == 2)
    {
        ticksStyle = 'i';
    }

    if (ComputeXIntervals(iObjUID, ticksStyle, &vector, &N, 0) != 0)
    {
        /* Something wrong happened */
        FREE(vector);
        return SET_PROPERTY_ERROR;
    }

    if (ComputeC_format(iObjUID, c_format) != 0)
    {
        /* Something wrong happened */
        FREE(vector);
        return SET_PROPERTY_ERROR;
    }

    stringVector = copyFormatedArray(vector, N, c_format, 256);

    status = setGraphicObjectProperty(iObjUID, __GO_TICKS_LABELS__, stringVector, jni_string_vector, N);

    FREE(vector);

    destroyStringArray(stringVector, N);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        return SET_PROPERTY_ERROR;
    }

}
/*------------------------------------------------------------------------*/
