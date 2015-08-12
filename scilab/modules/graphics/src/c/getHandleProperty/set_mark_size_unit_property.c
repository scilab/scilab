/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_mark_size_unit_property.c                                    */
/* desc : function to modify in Scilab the mark_size_unit field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "stricmp.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "MALLOC.h"

/*------------------------------------------------------------------------*/
int set_mark_size_unit_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int markSizeUnit = 0;

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "mark_size_unit");
        return SET_PROPERTY_ERROR;
    }

    /* 0 : point, 1 : tabulated */
    if (stricmp((char*)_pvData, "point") == 0)
    {
        markSizeUnit = 0;
    }
    else if (stricmp((char*)_pvData, "tabulated") == 0)
    {
        markSizeUnit = 1;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "mark_size_unit", "'point'", "'tabulated'");

    }

    status = setGraphicObjectProperty(iObjUID, __GO_MARK_SIZE_UNIT__, &markSizeUnit, jni_int, 1);

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "mark_size_unit");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
