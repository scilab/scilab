/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_interp_color_vector_property.c                               */
/* desc : function to modify in Scilab the background field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "GetProperty.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_interp_color_vector_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int iNumElements = 0;
    int* piNumElements = &iNumElements;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "interp_color_vector");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(pobjUID, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void **) &piNumElements);

    /*
     * A way to display a more explicit message would be to first get the
     * interpolation vector set flag and test it for NULL.
     */
    if (piNumElements == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "data");
        return SET_PROPERTY_ERROR;
    }

    if ( ( nbCol == 3 && iNumElements == 3 ) ||
            ( nbCol == 4 && iNumElements == 4 ) )
    {
        int tmp[4];
        stk( stackPointer );

        copyDoubleVectorToIntFromStack( stackPointer, tmp, nbCol );

        status = setGraphicObjectProperty(pobjUID, __GO_INTERP_COLOR_VECTOR__, tmp, jni_int_vector, nbCol);

        if (status == TRUE)
        {
            return SET_PROPERTY_SUCCEED;
        }
        else
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "interp_color_vector");
            return SET_PROPERTY_ERROR;
        }

    }
    else
    {
        Scierror(999, _("The number of column of the color vector must match the number of points defining the line (which must be %d or %d).\n"), 3, 4);
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
