/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2012 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include <string.h>

#ifdef _MSC_VER
#include "strdup_windows.h"
#endif

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_figure_resizefcn_property(void* _pvCtx, char* pobjUID)
{
    char* resizeFcn = NULL;
    char* type = NULL;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_string, (void **)&type);

    if (strcmp(type, __GO_FIGURE__) != 0)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "resizefcn");
        return -1;
    }

    getGraphicObjectProperty(pobjUID, __GO_RESIZEFCN__, jni_string, (void **) &resizeFcn);

    if (resizeFcn == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "resizefcn");
        return -1;
    }

    return sciReturnString(_pvCtx, strdup(resizeFcn));
}
/*------------------------------------------------------------------------*/
