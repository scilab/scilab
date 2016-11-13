/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
void* get_fractional_font_property(void* _pvCtx, int iObjUID)
{
    int iFractionalFont = 0;
    int* fractionalFont = &iFractionalFont;

    getGraphicObjectProperty(iObjUID, __GO_FONT_FRACTIONAL__, jni_bool, (void **)&fractionalFont);

    if (fractionalFont == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "fractional_font");
        return NULL;
    }

    if (iFractionalFont)
    {
        return sciReturnString("on");
    }
    else
    {
        return sciReturnString("off");
    }
}
/*------------------------------------------------------------------------*/
