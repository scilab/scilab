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
/* file:  set_layout_options_property.c                                   */
/* desc : function to modify in Scilab the layout_options field of        */
/*        a handle                                                        */
/*------------------------------------------------------------------------*/

#include <string.h>

#include "api_scilab.h"
#include "sci_malloc.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_types.h"

#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
#include "BorderLayoutType.h"
#include "FillType.h"
#include "AnchorType.h"
/*------------------------------------------------------------------------*/
int clearLayoutOptions(int iObjUID);
/*------------------------------------------------------------------------*/
int set_layout_options_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{

    //[] or tlist
    if (valueType == sci_matrix)
    {
        return clearLayoutOptions(iObjUID);
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
        if (strcmp(pstType, "OptNoLayout") == 0)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            return clearLayoutOptions(iObjUID);
        }
        else if (strcmp(pstType, "OptBorder") == 0)
        {
            //arg2 -> double 1x2 -> int 1*2
            int* piAddr2 = NULL;
            int iRows2 = 0;
            int iCols2 = 0;
            double* pdblPadding = NULL;
            int piPadding[2];

            sciErr = getListItemAddress(_pvCtx, piAddrList, 2, &piAddr2);
            if (sciErr.iErr)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstField);
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr2, &iRows2, &iCols2, &pdblPadding);
            if (sciErr.iErr)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstField);
                return SET_PROPERTY_ERROR;
            }

            piPadding[0] = (int)pdblPadding[0];
            piPadding[1] = (int)pdblPadding[1];

            setGraphicObjectProperty(iObjUID, __GO_BORDER_OPT_PADDING__, piPadding, jni_int_vector, 2);
        }
        else if (strcmp(pstType, "OptGrid") == 0)
        {
            //arg2 -> double 1x2 -> int 1*2
            //arg3 -> double 1x2 -> int 1*2
            int* piAddr2 = NULL;
            int iRows2 = 0;
            int iCols2 = 0;
            double* pdblGrid = NULL;
            int piGrid[2];

            int* piAddr3 = NULL;
            int iRows3 = 0;
            int iCols3 = 0;
            double* pdblPadding = NULL;
            int piPadding[2];

            sciErr = getListItemAddress(_pvCtx, piAddrList, 2, &piAddr2);
            if (sciErr.iErr)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstField);
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr2, &iRows2, &iCols2, &pdblGrid);
            if (sciErr.iErr)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstField);
                return SET_PROPERTY_ERROR;
            }

            sciErr = getListItemAddress(_pvCtx, piAddrList, 3, &piAddr3);
            if (sciErr.iErr)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstField);
                return SET_PROPERTY_ERROR;
            }

            sciErr = getMatrixOfDouble(_pvCtx, piAddr3, &iRows3, &iCols3, &pdblPadding);
            if (sciErr.iErr)
            {
                freeAllocatedMatrixOfString(iRows, iCols, pstField);
                return SET_PROPERTY_ERROR;
            }

            piGrid[0] = (int)pdblGrid[0];
            piGrid[1] = (int)pdblGrid[1];
            piPadding[0] = (int)pdblPadding[0];
            piPadding[1] = (int)pdblPadding[1];

            setGraphicObjectProperty(iObjUID, __GO_GRID_OPT_GRID__, piGrid, jni_int_vector, 2);
            setGraphicObjectProperty(iObjUID, __GO_GRID_OPT_PADDING__, piPadding, jni_int_vector, 2);
        }
        else if (strcmp(pstType, "OptGridBag") == 0)
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            return clearLayoutOptions(iObjUID);
        }
        else
        {
            freeAllocatedMatrixOfString(iRows, iCols, pstField);
            return SET_PROPERTY_ERROR;
        }
        freeAllocatedMatrixOfString(iRows, iCols, pstField);
    }

    return SET_PROPERTY_SUCCEED;
}

int clearLayoutOptions(int iObjUID)
{
    //reset all constraints data in model
    int pi[2] = {0, 0};
    BOOL status = FALSE;

    status = setGraphicObjectProperty(iObjUID, __GO_GRID_OPT_GRID__, pi, jni_int_vector, 2);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_GRID_OPT_PADDING__, pi, jni_int_vector, 2);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
        return SET_PROPERTY_ERROR;
    }

    status = setGraphicObjectProperty(iObjUID, __GO_BORDER_OPT_PADDING__, pi, jni_int_vector, 2);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "layout_options");
        return SET_PROPERTY_ERROR;
    }

    return SET_PROPERTY_SUCCEED;
}
