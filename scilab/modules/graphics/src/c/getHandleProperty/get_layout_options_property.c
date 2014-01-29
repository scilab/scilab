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
#include "FillType.h"
#include "AnchorType.h"
#include "returnPropertyList.h"
/*------------------------------------------------------------------------*/
int get_layout_options_property(void* _pvCtx, int iObjUID)
{
    int iLayout = 0;
    int* piLayout = &iLayout;

    getGraphicObjectProperty(iObjUID, __GO_LAYOUT__, jni_int, (void **)&piLayout);
    if (piLayout == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
        return -1;
    }

    switch (iLayout)
    {
        default:
        case LAYOUT_NONE :
        {
            char * variable_tlist[] = {"OptNoLayout"};
            returnedList * tList = createReturnedList(0, variable_tlist);
            destroyReturnedList(tList);
        }
        break;
        case LAYOUT_GRID :
        {
            char * variable_tlist[] = {"OptGrid", "grid", "padding"};
            returnedList * tList = createReturnedList(2, variable_tlist);
            double pdblGrid[2];
            double pdblPadding[2];
            int* piGrid = NULL;
            int* piPadding = NULL;

            getGraphicObjectProperty(iObjUID, __GO_GRID_OPT_GRID__, jni_int_vector, (void **)&piGrid);
            if (piGrid == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
                return -1;
            }

            getGraphicObjectProperty(iObjUID, __GO_GRID_OPT_PADDING__, jni_int_vector, (void **)&piPadding);
            if (piPadding == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
                return -1;
            }

            //convert to double
            pdblGrid[0] = (double)piGrid[0];
            pdblGrid[1] = (double)piGrid[1];

            pdblPadding[0] = (double)piPadding[0];
            pdblPadding[1] = (double)piPadding[1];

            //add to TList
            addRowVectorToReturnedList(tList, pdblGrid, 2);
            addRowVectorToReturnedList(tList, pdblPadding, 2);
            destroyReturnedList(tList);
        }
        break;
        case LAYOUT_BORDER :
        {
            char * variable_tlist[] = {"OptBorder", "padding"};
            returnedList * tList = createReturnedList(1, variable_tlist);
            double pdblPadding[2] = {0, 0};
            int* piPadding = NULL;

            getGraphicObjectProperty(iObjUID, __GO_BORDER_OPT_PADDING__, jni_int_vector, (void **)&piPadding);
            if (piPadding == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
                return -1;
            }

            //convert to double
            pdblPadding[0] = (double)piPadding[0];
            pdblPadding[1] = (double)piPadding[1];

            //add to TList
            addRowVectorToReturnedList(tList, pdblPadding, 2);
            destroyReturnedList(tList);
        }
        break;
        case LAYOUT_GRIDBAG :
        {
            char * variable_tlist[] = {"OptGridBag"};
            returnedList * tList = createReturnedList(0, variable_tlist);
            destroyReturnedList(tList);
        }
        break;
    }

    return 0;
}
