/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_marks_count_property.c                                       */
/* desc : function to retrieve in Scilab the marks_count field of a       */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include <stdlib.h>

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_marks_count_property(void* _pvCtx, int iObjUID)
{
    int iMarksCount = 0;
    int* piMarksCount = &iMarksCount;

    getGraphicObjectProperty(iObjUID, __GO_MARKS_COUNT__, jni_int, (void**)&piMarksCount);

    if (piMarksCount == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "marks_count");
        return -1;
    }

    return sciReturnDouble(_pvCtx, iMarksCount);
}
/*------------------------------------------------------------------------*/
