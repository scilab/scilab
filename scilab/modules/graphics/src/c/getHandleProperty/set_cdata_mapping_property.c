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
/* file: set_cdata_mapping_property.c                                     */
/* desc : function to modify in Scilab the cdata_mapping field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "MALLOC.h"
#include "BasicAlgos.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int set_cdata_mapping_property(void* _pvCtx, char* pobjUID, size_t stackPointer, int valueType, int nbRow, int nbCol )
{
    BOOL status = FALSE;
    int cdataMapping = 0;

    if ( !( valueType == sci_strings ) )
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "cdata_mapping");
        return SET_PROPERTY_ERROR;
    }

    if (isStringParamEqual( stackPointer, "scaled" ))
    {
        cdataMapping = 0;
    }
    else if (isStringParamEqual( stackPointer, "direct" ))
    {
        cdataMapping = 1;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "cdata_mapping", "scaled", "direct");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(pobjUID, __GO_DATA_MAPPING__, &cdataMapping, jni_int, 1);

    if (status == FALSE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "cdata_mapping");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
