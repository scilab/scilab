/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - DIGITEO - Manuel Juliachs
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
#include "configvariable_interface.h"
#include "Sciwarning.h"

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
        Sciwarning(_("WARNING: %s\n"), _("Please use 1 instead."));
    }

    return sciSetLineStyle(iObjUID, iLineStyle);
}
/*------------------------------------------------------------------------*/
