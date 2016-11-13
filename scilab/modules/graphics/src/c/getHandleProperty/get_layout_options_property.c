/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2014 - Scilab Enterprises - Antoine ELIAS
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
void* get_layout_options_property(void* _pvCtx, int iObjUID)
{
    int iLayout = 0;
    int* piLayout = &iLayout;

    getGraphicObjectProperty(iObjUID, __GO_LAYOUT__, jni_int, (void **)&piLayout);
    if (piLayout == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
        return NULL;
    }

    switch (iLayout)
    {
        default:
        case LAYOUT_NONE :
        {
            char * variable_tlist[] = {"OptNoLayout"};
            void * tList = createReturnedList(0, variable_tlist);
            return tList;
        }
        break;
        case LAYOUT_GRID :
        {
            char * variable_tlist[] = {"OptGrid", "grid", "padding"};
            void * tList = createReturnedList(2, variable_tlist);
            double pdblGrid[2];
            double pdblPadding[2];
            int* piGrid = NULL;
            int* piPadding = NULL;

            getGraphicObjectProperty(iObjUID, __GO_GRID_OPT_GRID__, jni_int_vector, (void **)&piGrid);
            if (piGrid == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
                return NULL;
            }

            getGraphicObjectProperty(iObjUID, __GO_GRID_OPT_PADDING__, jni_int_vector, (void **)&piPadding);
            if (piPadding == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
                return NULL;
            }

            //convert to double
            pdblGrid[0] = (double)piGrid[0];
            pdblGrid[1] = (double)piGrid[1];

            pdblPadding[0] = (double)piPadding[0];
            pdblPadding[1] = (double)piPadding[1];

            //add to TList
            addRowVectorToReturnedList(tList, pdblGrid, 2);
            addRowVectorToReturnedList(tList, pdblPadding, 2);
            return tList;
        }
        break;
        case LAYOUT_BORDER :
        {
            char * variable_tlist[] = {"OptBorder", "padding"};
            void * tList = createReturnedList(1, variable_tlist);
            double pdblPadding[2] = {0, 0};
            int* piPadding = NULL;

            getGraphicObjectProperty(iObjUID, __GO_BORDER_OPT_PADDING__, jni_int_vector, (void **)&piPadding);
            if (piPadding == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
                return NULL;
            }

            //convert to double
            pdblPadding[0] = (double)piPadding[0];
            pdblPadding[1] = (double)piPadding[1];

            //add to TList
            addRowVectorToReturnedList(tList, pdblPadding, 2);
            return tList;
        }
        break;
        case LAYOUT_GRIDBAG :
        {
            char * variable_tlist[] = {"OptGridBag"};
            void * tList = createReturnedList(0, variable_tlist);
            return tList;
        }
        break;
    }

    return 0;
}
