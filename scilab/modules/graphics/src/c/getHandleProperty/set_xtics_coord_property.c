/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
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
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_xtics_coord_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int N = 0;
    double * vector = NULL;
    char c_format[5];
    int iXNumberTicks = 0;
    int* piXNumberTicks = &iXNumberTicks;
    char** stringVector = NULL;
    double* coordsVector = NULL;
    int iTicksStyle = 0;
    int* piTicksStyle = &iTicksStyle;
    char ticksStyle = 0;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow != 1 )
    {
        Scierror(999, _("Wrong size for '%s' property: Row vector expected.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(pobjUID, __GO_X_NUMBER_TICKS__, jni_int, (void**)&piXNumberTicks);

    if (piXNumberTicks == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    if ( iXNumberTicks == 1 && nbCol != 1 )
    {
        Scierror(999, _("Wrong size for '%s' property: Scalar expected.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    if ( iXNumberTicks != 1 && nbCol == 1 )
    {
        Scierror(999, _("Wrong size for '%s' property: At least %d elements expected.\n"), "xtics_coord", 2);
        return SET_PROPERTY_ERROR;
    }

    /* what follows remains here as it was */
    coordsVector = createCopyDoubleVectorFromStack( stackPointer, nbCol );

    status = setGraphicObjectProperty(pobjUID, __GO_X_TICKS_COORDS__, coordsVector, jni_double_vector, nbCol);

    if (status == FALSE)
    {
        FREE(coordsVector);
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "xtics_coord");
        return SET_PROPERTY_ERROR;
    }

    FREE(coordsVector);

    getGraphicObjectProperty(pobjUID, __GO_TICKS_STYLE__, jni_int, (void**)&piTicksStyle);

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

    if (ComputeXIntervals( pobjUID, ticksStyle, &vector, &N, 0 ) != 0)
    {
        /* Something wrong happened */
        FREE( vector );
        return -1;
    }

    if (ComputeC_format( pobjUID, c_format ) != 0)
    {
        /* Something wrong happened */
        FREE( vector );
        return -1;
    }

    stringVector = copyFormatedArray( vector, N, c_format, 256 );

    status = setGraphicObjectProperty(pobjUID, __GO_TICKS_LABELS__, stringVector, jni_string_vector, N);

    FREE( vector );

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
