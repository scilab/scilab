/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: get_mark_size_unit_property.c                                    */
/* desc : function to retrieve in Scilab the mark_size_unit field of a    */
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
void* get_mark_size_unit_property(void* _pvCtx, int iObjUID)
{
    int iMarkSizeUnit = 0;
    int* piMarkSizeUnit = &iMarkSizeUnit;

    getGraphicObjectProperty(iObjUID, __GO_MARK_SIZE_UNIT__, jni_int, (void**)&piMarkSizeUnit);

    if (piMarkSizeUnit == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "mark_size_unit");
        return NULL;
    }

    if (iMarkSizeUnit == 0)
    {
        return sciReturnString("point");
    }
    else if (iMarkSizeUnit == 1)
    {
        return sciReturnString("tabulated");
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property.\n"), "mark_size_unit");
        return NULL;
    }

}
/*------------------------------------------------------------------------*/
