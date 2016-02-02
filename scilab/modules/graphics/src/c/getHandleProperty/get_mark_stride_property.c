/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2013 - Scilab Enterprises - Calixte DENIZET
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
/* file: get_mark_stride_property.c                                       */
/* desc : function to retrieve in Scilab the mark_stride field of a       */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_mark_stride_property(void* _pvCtx, int iObjUID)
{
    int iMarkStride = 0;
    int* piMarkStride = &iMarkStride;

    getGraphicObjectProperty(iObjUID, __GO_MARK_STRIDE__, jni_int, (void**)&piMarkStride);

    if (piMarkStride == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "mark_stride");
        return NULL;
    }

    return sciReturnDouble(iMarkStride);
}
/*------------------------------------------------------------------------*/
