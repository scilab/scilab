/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
 * Copyright (C) 2010-2011 - DIGITEO - Manuel Juliachs
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
void* get_box_property(void* _pvCtx, int iObjUID)
{
    int type = -1;
    int *piType = &type;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **) &piType);

    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "type");
        return NULL;
    }


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

            getGraphicObjectProperty(iObjUID, __GO_BOX_TYPE__, jni_int, (void **) &piBoxType);

            if (piBoxType == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "box");
                return NULL;
            }

            if (iBoxType == 0)
            {
                return sciReturnString("off");
            }
            else if (iBoxType == 1)
            {
                return sciReturnString("on");
            }
            else if (iBoxType == 2)
            {
                return sciReturnString("hidden_axes");
            }
            else if (iBoxType == 3)
            {
                return sciReturnString("back_half");
            }

        }
        break;
        case __GO_TEXT__ :
        {
            int iBox = 0;
            int* piBox = &iBox;

            getGraphicObjectProperty(iObjUID, __GO_BOX__, jni_bool, (void **) &piBox);

            if (piBox == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "box");
                return NULL;
            }

            if (iBox)
            {
                return sciReturnString("on");
            }
            else
            {
                return sciReturnString("off");
            }

        }
        break;
        default :
        {
            Scierror(999, _("'%s' property does not exist for this handle.\n"), "box");
            return NULL;
        }
    }

    return NULL;
}
/*------------------------------------------------------------------------*/
