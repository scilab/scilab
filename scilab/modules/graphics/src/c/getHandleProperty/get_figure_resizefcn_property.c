/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Vincent Couvert
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

#include <string.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_figure_resizefcn_property(void* _pvCtx, int iObjUID)
{
    char* resizeFcn = NULL;
    int iType = -1;
    int *piType = &iType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piType);

    if (piType == NULL || iType != __GO_FIGURE__)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "resizefcn");
        return NULL;
    }

    getGraphicObjectProperty(iObjUID, __GO_RESIZEFCN__, jni_string, (void **) &resizeFcn);

    if (resizeFcn == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "resizefcn");
        return NULL;
    }

    return sciReturnString(resizeFcn);
}
/*------------------------------------------------------------------------*/
