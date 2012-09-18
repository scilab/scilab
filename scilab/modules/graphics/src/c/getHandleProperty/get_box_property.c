/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_box_property.c                                               */
/* desc : function to retrieve in Scilab the box field of                 */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_box_property(void* _pvCtx, char* pobjUID)
{
    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(pobjUID, __GO_TYPE__, jni_int, (void **) &piType);

    /*
     * Required since the Box property is implemented differently for the Axes and Text
     * objects (respectively as an Integer and a Boolean).
     * To be corrected
     */
    switch (type)
    {
    case __GO_AXES__ :
    {
        int iBoxType = 0;
        int* piBoxType = &iBoxType;

        getGraphicObjectProperty(pobjUID, __GO_BOX_TYPE__, jni_int, (void **) &piBoxType);

        if (piBoxType == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"),"box");
            return -1;
        }

        if (iBoxType == 0)
        {
            return sciReturnString(_pvCtx, "off");
        }
        else if (iBoxType == 1)
        {
            return sciReturnString(_pvCtx, "on");
        }
        else if (iBoxType == 2)
        {
            return sciReturnString(_pvCtx, "hidden_axes");
        }
        else if (iBoxType == 3)
        {
            return sciReturnString(_pvCtx, "back_half");
        }

    }
    break;
    case __GO_TEXT__ :
    {
        int iBox = 0;
        int* piBox = &iBox;

        getGraphicObjectProperty(pobjUID, __GO_BOX__, jni_bool, (void **) &piBox);

        if (piBox == NULL)
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"),"box");
            return -1;
        }

        if (iBox)
        {
            return sciReturnString(_pvCtx, "on");
        }
        else
        {
            return sciReturnString(_pvCtx, "off");
        }

    }
    break;
    default :
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"),"box");
        return -1;
    }
    }
}
/*------------------------------------------------------------------------*/
