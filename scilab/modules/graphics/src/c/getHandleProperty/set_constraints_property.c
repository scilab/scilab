/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: set_figure_name_property.c                                       */
/* desc : function to modify in Scilab the figure_name field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_types.h"
#include "stricmp.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
#include "BorderLayoutType.h"
/*------------------------------------------------------------------------*/
int set_constraints_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    int iParent = 0;
    int* piParent = &iParent;
    int iLayout = 0;
    int* piLayout = &iLayout;

    getGraphicObjectProperty(iObjUID, __GO_PARENT__, jni_int, (void **)&piParent);
    if (piParent == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    getGraphicObjectProperty(iParent, __GO_LAYOUT__, jni_int, (void **)&piLayout);
    if (piLayout == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    switch (iLayout)
    {
        default:
        case NONE :
        {
            Scierror(999, _("'%s' Please set 'layout' property in parent before.\n"), "constraints");
            return SET_PROPERTY_ERROR;
        }
        case GRID :
        {
            if (valueType != sci_matrix || nbRow * nbCol != 6)
            {
                Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "constraints", 6);
                return SET_PROPERTY_ERROR;
            }

            if (setGraphicObjectProperty(iObjUID, __GO_UI_GRID_CONSTRAINTS__, _pvData, jni_double_vector, 6) == FALSE)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return SET_PROPERTY_ERROR;
            }

            return SET_PROPERTY_SUCCEED;
        }
        case BORDER :
        {
            int iBorder = -1;
            if (valueType != sci_strings)
            {
                Scierror(999, _("Wrong size for '%s' property: %d elements expected.\n"), "constraints", 1);
                return SET_PROPERTY_ERROR;
            }

            if (stricmp((char*)_pvData, "center") == 0)
            {
                iBorder = CENTER;
            }
            else if (stricmp((char*)_pvData, "bottom") == 0)
            {
                iBorder = SOUTH;
            }
            else if (stricmp((char*)_pvData, "top") == 0)
            {
                iBorder = NORTH;
            }
            else if (stricmp((char*)_pvData, "left") == 0)
            {
                iBorder = WEST;
            }
            else if (stricmp((char*)_pvData, "right") == 0)
            {
                iBorder = EAST;
            }
            else
            {
                Scierror(999, _("Wrong value for '%s' property: %s, %s or %s expected.\n"), "constraints", "'bottom', 'top'", "'left', 'right'", "'center'");
                return SET_PROPERTY_ERROR;
            }
            if (setGraphicObjectProperty(iObjUID, __GO_UI_BORDER_CONSTRAINTS__, &iBorder, jni_int, 1) == FALSE)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return SET_PROPERTY_ERROR;
            }

            return SET_PROPERTY_SUCCEED;
        }
    }
}
