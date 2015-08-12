/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_line_style_property.c                                        */
/* desc : function to modify in Scilab the line_style field of            */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sciprint.h"
#include "warningmode.h"

/*------------------------------------------------------------------------*/
int set_line_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iLineStyle = 1;

    if (valueType != sci_matrix)
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "line_style");
        return SET_PROPERTY_ERROR;
    }

    iLineStyle = (int)((double*)_pvData)[0];

    if (iLineStyle == 0 && getWarningMode())
    {
        sciprint(_("WARNING: %s\n"), _("{0,1} values are equivalent for line_style property."));
        sciprint(_("WARNING: %s\n"), _("0 will be removed after Scilab 5.4.0."));
        sciprint(_("WARNING: %s\n"), _("Please use 1 instead."));
    }

    return sciSetLineStyle(iObjUID, iLineStyle);
}
/*------------------------------------------------------------------------*/
