/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - SCilab Enterprises - antoine ELIAS
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2.1-en.txt
 *
 */

/*------------------------------------------------------------------------*/
/* file: get_figure_name_property.c                                       */
/* desc : function to retrieve in Scilab the figure_name field of a       */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#include <string.h>
#include "returnProperty.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
#include "BorderLayoutType.h"
/*------------------------------------------------------------------------*/
int get_constraints_property(void* _pvCtx, int iObjUID)
{
    int iParent = 0;
    int* piParent = &iParent;
    int iLayout = 0;
    int* piLayout = &iLayout;

    getGraphicObjectProperty(iObjUID, __GO_PARENT__, jni_int, (void **)&piParent);
    if (piParent == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return -1;
    }

    getGraphicObjectProperty(iParent, __GO_LAYOUT__, jni_int, (void **)&piLayout);
    if (piLayout == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return -1;
    }

    switch (iLayout)
    {
        default:
        case NONE :
            return sciReturnEmptyMatrix(_pvCtx);
        case GRID :
        {
            double* pdblGrid = NULL;
            getGraphicObjectProperty(iObjUID, __GO_UI_GRID_CONSTRAINTS__, jni_double_vector, (void **)&pdblGrid);
            if (pdblGrid == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return SET_PROPERTY_ERROR;
            }

            return sciReturnRowVector(_pvCtx, pdblGrid, 6);
        }
        break;
        case BORDER :
        {
            int iBorder = 0;
            int* piBorder = &iBorder;
            getGraphicObjectProperty(iObjUID, __GO_UI_BORDER_CONSTRAINTS__, jni_int, (void **)&piBorder);
            if (piBorder == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return SET_PROPERTY_ERROR;
            }

            switch (iBorder)
            {
                default:
                case CENTER :
                    return sciReturnString(_pvCtx, "center");
                case SOUTH :
                    return sciReturnString(_pvCtx, "bottom");
                case NORTH :
                    return sciReturnString(_pvCtx, "top");
                case WEST :
                    return sciReturnString(_pvCtx, "left");
                case EAST :
                    return sciReturnString(_pvCtx, "right");
            }
        }
        break;
    }
}
