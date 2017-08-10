/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2009 - DIGITEO - Pierre Lando
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
/* file: set_figure_style_property.c                                      */
/* desc : function to modify in Scilab the figure_style field of          */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "os_string.h"
#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_malloc.h"
/*------------------------------------------------------------------------*/
int set_figure_style_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{

    if (valueType != sci_strings)
    {
        Scierror(999, _("Wrong type for '%s' property: String expected.\n"), "figure_style");
        return SET_PROPERTY_ERROR;
    }

    if (stricmp((char*)_pvData, "old") == 0)
    {
        Scierror(999, _("Old graphic mode is no longer available. Please refer to the set help page.\n"));
        return SET_PROPERTY_ERROR;
    }
    else if (stricmp((char*)_pvData, "new") == 0)
    {
        /* graphic mode must be new */
        /* nothing to do */
        return SET_PROPERTY_UNCHANGED;
    }
    else
    {
        Scierror(999, _("Wrong value for '%s' property: %s or %s expected.\n"), "figure_style", "'old'", "'new'");

    }

    return SET_PROPERTY_ERROR;
}
/*------------------------------------------------------------------------*/
