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
/* file: set_mark_background_property.c                                   */
/* desc : function to modify in Scilab the mark_background field of       */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "setHandleProperty.h"
#include "SetProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "setGraphicObjectProperty.h"
#include "graphicObjectProperties.h"

#include "sci_malloc.h"

/*------------------------------------------------------------------------*/
int set_mark_background_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    BOOL status = FALSE;
    int markBackground = 0;
    int *tmp = NULL;
    int colorSet = 0;

    if ( valueType != sci_matrix )
    {
        Scierror(999, _("Wrong type for '%s' property: Integer expected.\n"), "mark_background");
        return SET_PROPERTY_ERROR;
    }

    if ( nbRow != 1 || nbCol <= 0 )
    {
        Scierror(999, _("Wrong size for '%s' property: Row vector expected.\n"), "mark_background");
        return SET_PROPERTY_ERROR;
    }

    if ( nbCol == 1 )
    {
        markBackground = (int)((double*)_pvData)[0];
        status = setGraphicObjectProperty(iObjUID, __GO_MARK_BACKGROUND__, &markBackground, jni_int, 1);
    }
    else
    {
        tmp = MALLOC(nbCol * sizeof(int));
        copyDoubleVectorToIntFromStack(_pvData, tmp, nbCol);
        status = setGraphicObjectProperty(iObjUID, __GO_MARK_BACKGROUNDS__, tmp, jni_int_vector, nbCol);
        FREE(tmp);
        colorSet = 1;
        setGraphicObjectProperty(iObjUID, __GO_COLOR_SET__, &colorSet, jni_bool, 1);
    }

    if (status == TRUE)
    {
        return SET_PROPERTY_SUCCEED;
    }
    else
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "mark_background");
        return SET_PROPERTY_ERROR;
    }
}
/*------------------------------------------------------------------------*/
