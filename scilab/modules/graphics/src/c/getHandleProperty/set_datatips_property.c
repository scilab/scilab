/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - Gustavo Barbosa Libotte
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_datatips_property.c                                          */
/* desc: function to modify in Scilab the datatips field of               */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "MALLOC.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "CallDatatipCreateField.h"

/*------------------------------------------------------------------------*/
int set_datatips_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    double* currentDatatipsCoords;
    double* newDatatipsCoords;
    int i;

    if ( !( valueType == sci_matrix ) )
    {
        Scierror(999, _("Wrong type for '%s' property: Real matrix expected.\n"), "datatips");
        return SET_PROPERTY_ERROR ;
    }

    if ( !( nbRow*nbCol % 2 == 0 ) )
    {
        Scierror(999, _("Wrong number of input arguments for '%s' property: X and Y expected.\n"), "datatips");
        return SET_PROPERTY_ERROR ;
    }

    currentDatatipsCoords = getDoubleMatrixFromStack(stackPointer);

    newDatatipsCoords = CallDatatipCreateField((char*)pobjUID, currentDatatipsCoords, nbRow * nbCol);

    status = setGraphicObjectProperty(pobjUID, __GO_DATATIPS__, newDatatipsCoords, jni_double_vector, nbRow * nbCol);

    FREE(newDatatipsCoords);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' does not exist for this handle.\n"), "datatips");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
