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
/* file: get_line_width_property.c                                        */
/* desc : function to retrieve in Scilab the line_width field of a        */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include "getHandleProperty.h"
#include "GetProperty.h"
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

/*------------------------------------------------------------------------*/
void* get_line_width_property(void* _pvCtx, int iObjUID)
{
    double dblLineWidth = 0.0;
    double *pdblLineWidth = &dblLineWidth;

    getGraphicObjectProperty(iObjUID, __GO_LINE_WIDTH__, jni_double, (void **)&pdblLineWidth);

    if (pdblLineWidth == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "line_width");
        return NULL;
    }

    return sciReturnDouble(dblLineWidth);
}
/*------------------------------------------------------------------------*/
