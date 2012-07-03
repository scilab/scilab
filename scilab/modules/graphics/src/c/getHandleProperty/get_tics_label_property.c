/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 * Copyright (C) 2012 - Scilab Enterprises - Bruno JOFRET
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_tics_labels_property.c                                       */
/* desc : function to retrieve in Scilab the tics_labels field of         */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "Format.h"
#include "MALLOC.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_tics_labels_property(void* _pvCtx, char* pobjUID)
{
    char** labels = NULL;
    int iNumberTicksLabels = 0;
    int* piNumberTicksLabels = &iNumberTicksLabels;

    getGraphicObjectProperty(pobjUID, __GO_NUMBER_TICKS_LABELS__, jni_int, (void **) &piNumberTicksLabels);
    if (piNumberTicksLabels == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_labels");
        return -1;
    }

    getGraphicObjectProperty(pobjUID, __GO_TICKS_LABELS__, jni_string_vector, (void **) &labels);

    if (labels == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"tics_labels");
        return -1;
    }

    /* User-specified ticks labels */
    return sciReturnRowStringVector(_pvCtx, labels, iNumberTicksLabels);
}
/*------------------------------------------------------------------------*/
