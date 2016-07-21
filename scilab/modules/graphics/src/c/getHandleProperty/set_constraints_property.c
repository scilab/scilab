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
/* file: set_figure_name_property.c                                       */
/* desc : function to modify in Scilab the figure_name field of           */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/
#include "api_scilab.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_types.h"
#include "os_string.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
#include "BorderLayoutType.h"
#include "FillType.h"
#include "AnchorType.h"
/*------------------------------------------------------------------------*/
int clearConstraints(int iObjUID);
/*------------------------------------------------------------------------*/
int set_constraints_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{

    //[] or tlist
    if (valueType == sci_matrix)
    {
        return clearConstraints(iObjUID);
    }
    else
    {
        SciErr sciErr;
        int i = 0;
        int* piAddrList = (int*)_pvData;

        int* piAddr = NULL;
        int iRows = 0;
        int iCols = 0;
        char** pstField = NULL;
        char* pstType = NULL;


        if (valueType != sci_tlist)
        {
            Scierror(999, _("Wrong type for '%s' property: A '%s' expected.\n"), "constraints", "tlist");
            return 1;
        }

        sciErr = getListItemAddress(_pvCtx, piAddrList, 1, &piAddr);
        if (sciErr.iErr)
        {
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedMatrixOfString(_pvCtx, piAddr, &iRows, &iCols, &pstField))
        {
            return SET_PROPERTY_ERROR;
        }

        pstType = pstField[0];
        //depend of kind of tlist
        if (strcmp(pstType, "NoLayoutConstraint") == 0)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            return clearConstraints(iObjUID);
        }
        else if (strcmp(pstType, "BorderConstraint") == 0)
        {
            //arg2 -> string -> int enum
            //arg3 -> double[] -> int[]

            int* piAddr2 = NULL;
            char* pstPos = NULL;
            int iPos = 0;

            int* piAddr3 = NULL;
            int iRows3 = 0;
            int iCols3 = 0;
            double* pdblPreferredSize = NULL;
            int piPreferredSize[2];

            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            sciErr = getListItemAddress(_pvCtx, piAddrList, 2, &piAddr2);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            if (getAllocatedSingleString(_pvCtx, piAddr2, &pstPos))
            {
                return SET_PROPERTY_ERROR;
            }

            //convert string value to enum
            if (stricmp(pstPos, "center") == 0)
            {
                iPos = BORDER_CENTER;
            }
            else if (stricmp(pstPos, "bottom") == 0)
            {
                iPos = BORDER_BOTTOM;
            }
            else if (stricmp(pstPos, "top") == 0)
            {
                iPos = BORDER_TOP;
            }
            else if (stricmp(pstPos, "left") == 0)
            {
                iPos = BORDER_LEFT;
            }
            else if (stricmp(pstPos, "right") == 0)
            {
                iPos = BORDER_RIGHT;
            }
            else
            {
                Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "constraints.position", "center, top, bottom, left, right");
                freeAllocatedSingleString(pstPos);
                return SET_PROPERTY_ERROR;
            }

            freeAllocatedSingleString(pstPos);

            sciErr = getListItemAddress(_pvCtx, piAddrList, 3, &piAddr3);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr3, &iRows3, &iCols3, &pdblPreferredSize);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            //reassign double values in int[]
            piPreferredSize[0] = (int)pdblPreferredSize[0];
            piPreferredSize[1] = (int)pdblPreferredSize[1];

            setGraphicObjectProperty(iObjUID, __GO_UI_BORDER_POSITION__, &iPos, jni_int, 1);
            setGraphicObjectProperty(iObjUID, __GO_UI_BORDER_PREFERREDSIZE__, piPreferredSize, jni_int_vector, 2);
        }
        else if (strcmp(pstType, "GridConstraints") == 0)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            return clearConstraints(iObjUID);
        }
        else if (strcmp(pstType, "GridBagConstraints") == 0)
        {
            //arg2 -> double 1x4 -> int 1*4
            //arg3 -> double 1x2 -> double 1*2
            //arg4 -> string -> int enum
            //arg5 -> string -> int enum
            //arg6 -> double 1x2 -> int 1*2
            //arg7 -> double 1x2 -> int 1*2

            int* piAddr2 = NULL;
            int iRows2 = 0;
            int iCols2 = 0;
            double* pdblGrid = NULL;
            int piGrid[4];

            int* piAddr3 = NULL;
            int iRows3 = 0;
            int iCols3 = 0;
            double* pdblWeight = NULL;

            int* piAddr4 = NULL;
            char* pstFill = NULL;
            int iFill = 0;

            int* piAddr5 = NULL;
            char* pstAnchor = NULL;
            int iAnchor = 0;

            int* piAddr6 = NULL;
            int iRows6 = 0;
            int iCols6 = 0;
            double* pdblPadding = NULL;
            int piPadding[2];

            int* piAddr7 = NULL;
            int iRows7 = 0;
            int iCols7 = 0;
            double* pdblPreferredSize = NULL;
            int piPreferredSize[2];

            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            sciErr = getListItemAddress(_pvCtx, piAddrList, 2, &piAddr2);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr2, &iRows2, &iCols2, &pdblGrid);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getListItemAddress(_pvCtx, piAddrList, 3, &piAddr3);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr3, &iRows3, &iCols3, &pdblWeight);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getListItemAddress(_pvCtx, piAddrList, 4, &piAddr4);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            if (getAllocatedSingleString(_pvCtx, piAddr4, &pstFill))
            {
                return SET_PROPERTY_ERROR;
            }

            //convert string value to enum
            if (stricmp(pstFill, "none") == 0)
            {
                iFill = FILL_NONE;
            }
            else if (stricmp(pstFill, "horizontal") == 0)
            {
                iFill = FILL_HORIZONTAL;
            }
            else if (stricmp(pstFill, "vertical") == 0)
            {
                iFill = FILL_VERTICAL;
            }
            else if (stricmp(pstFill, "both") == 0)
            {
                iFill = FILL_BOTH;
            }
            else
            {
                Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "constraints.fill", "none, horizontal, vertical, both");
                freeAllocatedSingleString(pstFill);
                return SET_PROPERTY_ERROR;
            }

            freeAllocatedSingleString(pstFill);

            sciErr = getListItemAddress(_pvCtx, piAddrList, 5, &piAddr5);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            if (getAllocatedSingleString(_pvCtx, piAddr5, &pstAnchor))
            {
                return SET_PROPERTY_ERROR;
            }

            //convert string value to enum
            if (stricmp(pstAnchor, "center") == 0)
            {
                iAnchor = ANCHOR_CENTER;
            }
            else if (stricmp(pstAnchor, "upper") == 0)
            {
                iAnchor = ANCHOR_UPPER;
            }
            else if (stricmp(pstAnchor, "lower") == 0)
            {
                iAnchor = ANCHOR_LOWER;
            }
            else if (stricmp(pstAnchor, "right") == 0)
            {
                iAnchor = ANCHOR_RIGHT;
            }
            else if (stricmp(pstAnchor, "left") == 0)
            {
                iAnchor = ANCHOR_LEFT;
            }
            else if (stricmp(pstAnchor, "upper_right") == 0)
            {
                iAnchor = ANCHOR_UPPER_RIGHT;
            }
            else if (stricmp(pstAnchor, "upper_left") == 0)
            {
                iAnchor = ANCHOR_UPPER_LEFT;
            }
            else if (stricmp(pstAnchor, "lower_right") == 0)
            {
                iAnchor = ANCHOR_LOWER_RIGHT;
            }
            else if (stricmp(pstAnchor, "lower_left") == 0)
            {
                iAnchor = ANCHOR_LOWER_LEFT;
            }
            else
            {
                Scierror(999, _("Wrong value for '%s' property: Must be in the set {%s}.\n"), "constraints.anchor", "center, upper, lower, right, left, upper_right, upper_left, lower_right, lower_left");
                freeAllocatedSingleString(pstAnchor);
                return SET_PROPERTY_ERROR;
            }

            freeAllocatedSingleString(pstAnchor);

            sciErr = getListItemAddress(_pvCtx, piAddrList, 6, &piAddr6);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr6, &iRows6, &iCols6, &pdblPadding);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getListItemAddress(_pvCtx, piAddrList, 7, &piAddr7);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr7, &iRows7, &iCols7, &pdblPreferredSize);
            if (sciErr.iErr)
            {
                return SET_PROPERTY_ERROR;
            }

            //reassign double values in int[]
            piGrid[0] = (int)pdblGrid[0];
            piGrid[1] = (int)pdblGrid[1];
            piGrid[2] = (int)pdblGrid[2];
            piGrid[3] = (int)pdblGrid[3];

            piPadding[0] = (int)pdblPadding[0];
            piPadding[1] = (int)pdblPadding[1];

            piPreferredSize[0] = (int)pdblPreferredSize[0];
            piPreferredSize[1] = (int)pdblPreferredSize[1];

            setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_GRID__, piGrid, jni_int_vector, 4);
            setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_WEIGHT__, pdblWeight, jni_double_vector, 2);
            setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_FILL__, &iFill, jni_int, 1);
            setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_ANCHOR__, &iAnchor, jni_int, 1);
            setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_PADDING__, piPadding, jni_int_vector, 2);
            setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_PREFERREDSIZE__, piPreferredSize, jni_int_vector, 2);
        }
        else
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            return SET_PROPERTY_ERROR;
        }
    }

    return SET_PROPERTY_SUCCEED;
}

int clearConstraints(int iObjUID)
{
    //reset all constraints data in model
    int pi[4] = {0, 0, 0, 0};
    double pdbl[2] = {0, 0};
    int iPos = BORDER_CENTER;
    int iFill = FILL_NONE;
    int iAnchor = ANCHOR_CENTER;
    BOOL status = FALSE;

    status = setGraphicObjectProperty(iObjUID, __GO_UI_BORDER_POSITION__, &iPos, jni_int, 1);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_GRID__, pi, jni_int_vector, 4);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_WEIGHT__, pdbl, jni_double_vector, 2);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_FILL__, &iFill, jni_int, 1);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_ANCHOR__, &iAnchor, jni_int, 1);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_UI_GRIDBAG_PADDING__, pi, jni_int_vector, 2);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
