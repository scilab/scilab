/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2011 - DIGITEO - Vincent COUVERT
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_tag_property.c                                               */
/* desc : function to modify in Scilab the tag field of                   */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "sci_types.h"
#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "Scierror.h"
#include "localization.h"

#include "graphicObjectProperties.h"
#include "setGraphicObjectProperty.h"

/*------------------------------------------------------------------------*/
int set_tag_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    // Tag must be only one character string

    BOOL status = FALSE;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: A string expected.\n"), "Tag");
        return SET_PROPERTY_ERROR;
    }

    if (nbCol != 1)
    {
        Scierror(999, _("Wrong size for '%s' property: A string expected.\n"), "Tag");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_TAG__, (char*)_pvData, jni_string, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "Tag");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
