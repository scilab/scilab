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
        case LAYOUT_NONE :
        {
            char * variable_tlist[] = {"NoLayoutConstraint"};
            returnedList * tList = createReturnedList(0, variable_tlist);
            destroyReturnedList(tList);
        }
        break;
        case LAYOUT_GRID :
        {
            char * variable_tlist[] = {"GridConstraints"};
            returnedList * tList = createReturnedList(0, variable_tlist);
            destroyReturnedList(tList);
        }
        break;
        case LAYOUT_BORDER :
        {
            char * variable_tlist[] = {"BorderConstraint", "position", "preferredsize"};
            returnedList * tList = createReturnedList(2, variable_tlist);

            int iBorder = 0;
            int* piBorder = &iBorder;

            double pdblPreferredSize[2];
            int* piPreferredSize = NULL;

            getGraphicObjectProperty(iObjUID, __GO_UI_BORDER_POSITION__, jni_int, (void **)&piBorder);
            if (piBorder == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            switch (iBorder)
            {
                default:
                case BORDER_CENTER :
                    addStringToReturnedList(tList, "center");
                    break;
                case BORDER_BOTTOM :
                    addStringToReturnedList(tList, "bottom");
                    break;
                case BORDER_TOP :
                    addStringToReturnedList(tList, "top");
                    break;
                case BORDER_LEFT :
                    addStringToReturnedList(tList, "left");
                    break;
                case BORDER_RIGHT :
                    addStringToReturnedList(tList, "right");
                    break;
            }

            getGraphicObjectProperty(iObjUID, __GO_UI_BORDER_PREFERREDSIZE__, jni_int_vector, (void **)&piPreferredSize);
            if (piPreferredSize == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            //convert to double
            pdblPreferredSize[0] = (double)piPreferredSize[0];
            pdblPreferredSize[1] = (double)piPreferredSize[1];

            addRowVectorToReturnedList(tList, pdblPreferredSize, 2);

            destroyReturnedList(tList);
        }
        break;
        case LAYOUT_GRIDBAG :
        {
            char * variable_tlist[] = {"GridBagConstraints", "grid", "weight", "fill", "anchor", "padding", "preferredsize"};
            returnedList * tList = createReturnedList(6, variable_tlist);
            int* piGrid = NULL;
            double pdblGrid[4];
            double* pdblWeight = NULL;

            int iFill = 0;
            int* piFill = &iFill;
            int iAnchor = 0;
            int* piAnchor = &iAnchor;

            double pdblPadding[2];
            int* piPadding = NULL;

            double pdblPreferredSize[2];
            int* piPreferredSize = NULL;

            getGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_GRID__, jni_int_vector, (void **)&piGrid);
            if (piGrid == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            getGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_WEIGHT__, jni_double_vector, (void **)&pdblWeight);
            if (pdblWeight == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            getGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_FILL__, jni_int, (void **)&piFill);
            if (piFill == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            getGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_ANCHOR__, jni_int, (void **)&piAnchor);
            if (piAnchor == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            getGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_PADDING__, jni_int_vector, (void **)&piPadding);
            if (piPadding == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            getGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_PREFERREDSIZE__, jni_int_vector, (void **)&piPreferredSize);
            if (piPreferredSize == NULL)
            {
                Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
                return -1;
            }

            //convert to double
            pdblGrid[0] = (double)piGrid[0];
            pdblGrid[1] = (double)piGrid[1];
            pdblGrid[2] = (double)piGrid[2];
            pdblGrid[3] = (double)piGrid[3];

            pdblPadding[0] = (double)piPadding[0];
            pdblPadding[1] = (double)piPadding[1];

            pdblPreferredSize[0] = (double)piPreferredSize[0];
            pdblPreferredSize[1] = (double)piPreferredSize[1];

            addRowVectorToReturnedList(tList, pdblGrid, 4);
            addRowVectorToReturnedList(tList, pdblWeight, 2);

            switch (iFill)
            {
                default :
                case FILL_NONE :
                    addStringToReturnedList(tList, "none");
                    break;
                case FILL_HORIZONTAL :
                    addStringToReturnedList(tList, "horizontal");
                    break;
                case FILL_VERTICAL :
                    addStringToReturnedList(tList, "vertical");
                    break;
                case FILL_BOTH :
                    addStringToReturnedList(tList, "both");
                    break;
            }

            switch (iAnchor)
            {
                default :
                case ANCHOR_CENTER :
                    addStringToReturnedList(tList, "center");
                    break;
                case ANCHOR_UPPER :
                    addStringToReturnedList(tList, "upper");
                    break;
                case ANCHOR_LOWER :
                    addStringToReturnedList(tList, "lower");
                    break;
                case ANCHOR_RIGHT :
                    addStringToReturnedList(tList, "right");
                    break;
                case ANCHOR_LEFT :
                    addStringToReturnedList(tList, "left");
                    break;
                case ANCHOR_UPPER_RIGHT :
                    addStringToReturnedList(tList, "upper_right");
                    break;
                case ANCHOR_UPPER_LEFT :
                    addStringToReturnedList(tList, "upper_left");
                    break;
                case ANCHOR_LOWER_RIGHT :
                    addStringToReturnedList(tList, "lower_right");
                    break;
                case ANCHOR_LOWER_LEFT :
                    addStringToReturnedList(tList, "lower_left");
                    break;
            }

            addRowVectorToReturnedList(tList, pdblPadding, 2);
            addRowVectorToReturnedList(tList, pdblPreferredSize, 2);
            destroyReturnedList(tList);
        }
        break;
    }

    return 0;
}
