/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2011 - DIGITEO - Vincent Couvert
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_auto_clear_property.c                                        */
/* desc : function to retrieve in Scilab the auto_clear field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
int get_auto_clear_property(void* _pvCtx, int iObjUID)
{
    int iAutoClear = 0;
    int* piAutoClear = &iAutoClear;

    getGraphicObjectProperty(iObjUID, __GO_AUTO_CLEAR__, jni_bool, (void **)&piAutoClear);

    if (piAutoClear == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "auto_clear");
        return -1;
    }

    if (iAutoClear)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}

/*------------------------------------------------------------------------*/
