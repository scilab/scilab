/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
/* file: get_fractional_font_property.c                                   */
/* desc : function to retrieve in Scilab the fractional_font field of     */
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
int get_fractional_font_property(void* _pvCtx, char* pobjUID)
{
    int iFractionalFont = 0;
    int* fractionalFont = &iFractionalFont;

    getGraphicObjectProperty(pobjUID, __GO_FONT_FRACTIONAL__, jni_bool, (void **)&fractionalFont);

    if (fractionalFont == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "fractional_font");
        return -1;
    }

    if (iFractionalFont)
    {
        return sciReturnString(_pvCtx, "on");
    }
    else
    {
        return sciReturnString(_pvCtx, "off");
    }
}
/*------------------------------------------------------------------------*/
