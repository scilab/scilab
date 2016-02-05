/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2004-2006 - INRIA - Fabrice Leray
 * Copyright (C) 2006 - INRIA - Allan Cornet
 * Copyright (C) 2006 - INRIA - Jean-Baptiste Silvy
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
/* file: set_parent_property.c                                            */
/* desc : function to modify in Scilab the parent field of                */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "sci_types.h"
#include "setHandleProperty.h"
#include "getPropertyAssignedValue.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "GetProperty.h"
#include "InitUIMenu.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "HandleManagement.h"
#include "FigureList.h"

/*------------------------------------------------------------------------*/
int set_parent_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iParentUID = 0;
    int iParentType = -1;
    int *piParentType = &iParentType;
    int iParentStyle = -1;
    int *piParentStyle = &iParentStyle;
    int iObjType = -1;
    int *piObjType = &iObjType;

    getGraphicObjectProperty(iObjUID, __GO_TYPE__, jni_int, (void **)&piObjType);

    if (iObjType == __GO_UICONTROL__)
    {
        if (valueType == sci_handles)
        {
            iParentUID = getObjectFromHandle((long)((long long*)_pvData)[0]);
        }
        else if (valueType == sci_matrix)
        {
            iParentUID = getFigureFromIndex((int)((double*)_pvData)[0]);
        }
        else
        {
            Scierror(999, _("Wrong type for '%s' property: '%s' handle or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
            return SET_PROPERTY_ERROR;
        }

        if (iParentUID == 0)
        {
            // Can not set the parent
            Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
            return SET_PROPERTY_ERROR;
        }

        getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);

        if (iParentType != __GO_FIGURE__)
        {
            getGraphicObjectProperty(iParentUID, __GO_STYLE__, jni_int, (void **)&piParentStyle);
            if (iParentType != __GO_UICONTROL__ ||
                    (iParentStyle != __GO_UI_FRAME__ && iParentStyle != __GO_UI_TAB__ && iParentStyle != __GO_UI_LAYER__))
            {
                Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
                return SET_PROPERTY_ERROR;
            }
        }

        setGraphicObjectRelationship(iParentUID, iObjUID);
        return SET_PROPERTY_SUCCEED;
    }

    if (iObjType == __GO_UIMENU__)
    {
        if ((valueType != sci_handles) && (valueType != sci_matrix))    /* sci_matrix used for adding menus in console menu */
        {
            Scierror(999, _("Wrong type for '%s' property: '%s' handle or '%s' handle expected.\n"), "Parent", "Figure", "Uimenu");
            return SET_PROPERTY_ERROR;
        }
        else
        {
            return setMenuParent(iObjUID, _pvData, valueType, nbRow, nbCol);
        }
    }

    if (iObjType == __GO_AXES__)
    {
        if (valueType == sci_handles)
        {
            iParentUID = getObjectFromHandle((long)((long long*)_pvData)[0]);
        }
        else if (valueType == sci_matrix)
        {
            iParentUID = getFigureFromIndex((int)((double*)_pvData)[0]);
        }
        else
        {
            Scierror(999, _("Wrong type for '%s' property: '%s' handle or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
            return SET_PROPERTY_ERROR;
        }

        if (iParentUID == 0)
        {
            // Can not set the parent
            Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
            return SET_PROPERTY_ERROR;
        }

        getGraphicObjectProperty(iParentUID, __GO_TYPE__, jni_int, (void **)&piParentType);

        if (iParentType != __GO_FIGURE__)
        {
            getGraphicObjectProperty(iParentUID, __GO_STYLE__, jni_int, (void **)&piParentStyle);
            if (iParentStyle != __GO_UI_FRAME__)
            {
                Scierror(999, _("Wrong value for '%s' property: A '%s' or '%s' handle expected.\n"), "Parent", "Figure", "Frame uicontrol");
                return SET_PROPERTY_ERROR;
            }
        }

        setGraphicObjectRelationship(iParentUID, iObjUID);
        return SET_PROPERTY_SUCCEED;
    }

    Scierror(999, _("Parent property can not be modified directly.\n"));
    return SET_PROPERTY_ERROR;
}

/*------------------------------------------------------------------------*/
