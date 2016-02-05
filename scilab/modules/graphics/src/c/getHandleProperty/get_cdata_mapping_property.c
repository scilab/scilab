/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
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
/* file: get_cdata_mapping_property.c                                     */
/* desc : function to retrieve in Scilab the cdata_mapping field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_cdata_mapping_property(void* _pvCtx, int iObjUID)
{
    int iCDataMapping = 0;
    int* piCDataMapping = &iCDataMapping;

    getGraphicObjectProperty(iObjUID, __GO_DATA_MAPPING__, jni_int, (void **)&piCDataMapping);

    if (piCDataMapping == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "cdata_mapping");
        return NULL;
    }

    if (iCDataMapping == 0)
    {
        return sciReturnString("scaled");
    }
    else if (iCDataMapping == 1)
    {
        return sciReturnString("direct");
    }

    return NULL;
}
/*------------------------------------------------------------------------*/
