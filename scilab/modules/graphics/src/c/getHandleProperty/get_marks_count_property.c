/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Calixte DENIZET
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
void* get_marks_count_property(void* _pvCtx, int iObjUID)
{
    int iMarksCount = 0;
    int* piMarksCount = &iMarksCount;

    getGraphicObjectProperty(iObjUID, __GO_MARKS_COUNT__, jni_int, (void**)&piMarksCount);

    if (piMarksCount == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "marks_count");
        return NULL;
    }

    return sciReturnDouble(iMarksCount);
}
/*------------------------------------------------------------------------*/
