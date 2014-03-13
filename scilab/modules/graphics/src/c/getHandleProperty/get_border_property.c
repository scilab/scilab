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
#include "api_scilab.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "stricmp.h"

#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "FrameBorderType.h"
#include "returnPropertyList.h"

/*------------------------------------------------------------------------*/
int getBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getNoBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getLineBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getBevelBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getSoftBevelBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getCommonBevelBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID, int _iBevel);
int getEtchedBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getTitledBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getEmptyBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getCompoundBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getMatteBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
/*------------------------------------------------------------------------*/
int get_border_property(void* _pvCtx, int iObjUID)
{
    int iBorder = 0;
    int* piBorder = &iBorder;
    int iRhs = nbInputArgument(_pvCtx) + 1;

    getGraphicObjectProperty(iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void **)&piBorder);
    if (piBorder == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return -1;
    }

    return getBorder(_pvCtx, iRhs, 0, 0, iBorder);
}
/*------------------------------------------------------------------------*/
int getBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    int iStyle = 0;
    int* piStyle = &iStyle;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_STYLE__, jni_int, (void **)&piStyle);
    if (piStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return -1;
    }


    switch (iStyle)
    {
        default:
        {
            return getNoBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case LINE :
        {
            return getLineBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case BEVEL :
        {
            return getBevelBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case SOFTBEVEL :
        {
            return getSoftBevelBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case ETCHED :
        {
            return getEtchedBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case TITLED :
        {
            return getTitledBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case EMPTY :
        {
            return getEmptyBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case COMPOUND :
        {
            return getCompoundBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case MATTE :
        {
            return getMatteBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getNoBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList[] = {"NoBorder"};
    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 1, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 1, &piAddrList);
    }

    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 1, pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getLineBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList[] = {"LineBorder", "color", "thickness", "rounded"};
    char* pstColor = NULL;
    int iThickness = 0;
    double dblThickness = 0;
    int* piThickness = &iThickness;
    int iRounded = 0;
    int* piRounded = &iRounded;
    int iListSize = 4;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);
    if (pstColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_LINE_THICKNESS__, jni_int, (void **)&piThickness);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_ROUNDED__, jni_bool, (void **)&piRounded);

    if (piRounded == NULL)
    {
        iListSize = 3;
    }

    if (piThickness == NULL)
    {
        iListSize = 2;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, iListSize, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, iListSize, &piAddrList);
    }

    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 2, 1, 1, &pstColor);
    if (sciErr.iErr)
    {
        return -1;
    }

    if (iListSize > 2)
    {
        dblThickness = (double)iThickness;
        sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 3, 1, 1, &dblThickness);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    if (iListSize > 3)
    {
        sciErr = createMatrixOfBooleanInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &iRounded);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getBevelBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    return getCommonBevelBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID, 1);
}
/*------------------------------------------------------------------------*/
int getSoftBevelBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    return getCommonBevelBorder(_pvCtx, _iVar, _piParent, _iPos, _iObjUID, 2);
}
/*------------------------------------------------------------------------*/
int getCommonBevelBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID, int _iBevel)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList1[] = {"BevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"};
    char* pstFieldList2[] = {"BevelBorder", "type", "hlouter", "shadowouter"};
    char** pstFieldList = pstFieldList1;
    char* pstSoftFieldList1[] = {"SoftBevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"};
    char* pstSoftFieldList2[] = {"SoftBevelBorder", "type", "hlouter", "shadowouter"};
    char** pstSoftFieldList = pstSoftFieldList1;
    int iType = 0;
    int* piType = &iType;
    char* pstType[] = {"raised", "lowered"};
    char* pstHlOutColor = NULL;
    char* pstHlInColor = NULL;
    char* pstShadowOutColor = NULL;
    char* pstShadowInColor = NULL;
    int iListSize = 6;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_TYPE__, jni_int, (void **)&piType);
    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "border");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, jni_string, (void **)&pstHlOutColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, jni_string, (void **)&pstHlInColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_OUT__, jni_string, (void **)&pstShadowOutColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_IN__, jni_string, (void **)&pstShadowInColor);

    if (pstHlInColor == NULL || pstShadowInColor == NULL)
    {
        iListSize = 4;
        pstFieldList = pstFieldList2;
        pstSoftFieldList = pstSoftFieldList2;
    }

    if (pstHlOutColor == NULL || pstShadowOutColor == NULL)
    {
        iListSize = 2;
        pstFieldList = pstFieldList2;
        pstSoftFieldList = pstSoftFieldList2;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, iListSize, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, iListSize, &piAddrList);
    }
    if (sciErr.iErr)
    {
        return -1;
    }

    if (_iBevel == 1)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstFieldList);
    }
    else
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstSoftFieldList);
    }

    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 2, 1, 1, &pstType[iType]);
    if (sciErr.iErr)
    {
        return -1;
    }

    if (iListSize == 4)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 3, 1, 1, &pstHlOutColor);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &pstShadowOutColor);
        if (sciErr.iErr)
        {
            return -1;
        }
    }
    else if (iListSize == 6)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 3, 1, 1, &pstHlOutColor);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &pstHlInColor);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 5, 1, 1, &pstShadowOutColor);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 6, 1, 1, &pstShadowInColor);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getEtchedBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList1[] = {"EtchedBorder", "type", "hl", "shadow"};
    char* pstFieldList2[] = {"EtchedBorder", "hl", "shadow"};
    char** pstFieldList = pstFieldList1;
    int iType = 0;
    int* piType = &iType;
    char* pstType[] = {"raised", "lowered"};
    char* pstHlOutColor = NULL;
    char* pstShadowOutColor = NULL;
    int iListSize = 4;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_TYPE__, jni_int, (void **)&piType);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, jni_string, (void **)&pstHlOutColor);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_OUT__, jni_string, (void **)&pstShadowOutColor);

    if (piType == NULL)
    {
        if (pstHlOutColor == NULL || pstShadowOutColor == NULL)
        {
            iListSize = 1;
        }
        else
        {
            iListSize = 3;
            pstFieldList = pstFieldList2;
        }
    }
    else
    {
        if (pstHlOutColor == NULL || pstShadowOutColor == NULL)
        {
            iListSize = 2;
        }
        else
        {
            iListSize = 4;
        }
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, iListSize, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, iListSize, &piAddrList);
    }

    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    if (iListSize == 2 || iListSize == 4)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 2, 1, 1, &pstType[iType]);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    if (iListSize == 3 || iListSize == 4)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, iListSize - 1, 1, 1, &pstHlOutColor);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, iListSize, 1, 1, &pstShadowOutColor);
        if (sciErr.iErr)
        {
            return -1;
        }
    }
    return 0;
}
/*------------------------------------------------------------------------*/
int getTitledBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList1[] = {"TitledBorder", "border", "title", "justification", "position", "font", "color"};
    char* pstFieldList2[] = {"TitledBorder", "title"};
    char** pstFieldList = pstFieldList1;
    char* pstJustification[] = {"leading" , "left" , "center" , "right" , "trailing"};
    char* pstPosition[] = {"top" , "above_top" , "below_top" , "bottom" , "above_bottom", "below_bottom"};
    int iChildBorder = 0;
    int* piChildBorder = &iChildBorder;
    char* pstTitle = NULL;
    int iJustification = 0;
    int* piJustification = &iJustification;
    char* pstFontName = NULL;
    char* pstFontAngle = NULL;
    int iFontSize = 0;
    int* piFontSize = &iFontSize;
    double dblFontSize = 0;
    char* pstFontWeight = NULL;
    int iPosition = 0;
    int* piPosition = &iPosition;
    char* pstColor = NULL;
    int iListSize = 7;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_TITLE__, jni_int, (void **)&piChildBorder);
    getGraphicObjectProperty(_iObjUID, __GO_TITLE__, jni_string, (void **)&pstTitle);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_JUSTIFICATION__, jni_int, (void **)&piJustification);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTNAME__, jni_string, (void **)&pstFontName);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTANGLE__, jni_string, (void **)&pstFontAngle);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTSIZE__, jni_int, (void **)&piFontSize);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTWEIGHT__, jni_string, (void **)&pstFontWeight);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_POSITION__, jni_int, (void **)&piPosition);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);

    //2 3 5 6 7
    if (pstColor == NULL)
    {
        iListSize = 6;
    }

    if (pstFontName == NULL || pstFontAngle == NULL || piFontSize == NULL || pstFontWeight == NULL)
    {
        iListSize = 5;
    }

    if (piJustification == NULL)
    {
        iListSize = 3;
    }

    if (pstTitle == NULL)
    {
        iListSize = 2;
    }

    if (piChildBorder == NULL)
    {
        iListSize = 2;
        pstFieldList = pstFieldList2;
    }


    dblFontSize = (double)iFontSize;

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, iListSize, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, iListSize, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    if (piChildBorder)
    {
        //get child information and fill current list
        if (getBorder(_pvCtx, _iVar, piAddrList, 2, iChildBorder))
        {
            return -1;
        }
    }

    if (pstTitle)
    {
        //pos 2 or 3
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, piChildBorder == NULL ? 2 : 3, 1, 1, &pstTitle);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    if (iListSize > 3)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &pstJustification[iJustification]);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 5, 1, 1, &pstPosition[iPosition]);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    if (iListSize > 5)
    {
        //create a Font Border
        int* piFont = NULL;
        char* pstFontBorder[] = {"BorderFont", "name", "size", "angle", "weight"};

        sciErr = createTListInList(_pvCtx, _iVar, piAddrList, 6, 5, &piFont);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piFont, 1, 1, 5, pstFontBorder);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piFont, 2, 1, 1, &pstFontName);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piFont, 3, 1, 1, &dblFontSize);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piFont, 4, 1, 1, &pstFontAngle);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piFont, 5, 1, 1, &pstFontWeight);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    if (iListSize > 6)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 7, 1, 1, &pstColor);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getEmptyBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList[] = {"EmptyBorder", "top", "left", "bottom", "right"};
    double* pdblPosition = NULL;
    int iListSize = 5;

    getGraphicObjectProperty(_iObjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblPosition);

    if (pdblPosition == NULL)
    {
        iListSize = 1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, iListSize, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, iListSize, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    if (iListSize > 1)
    {
        sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 2, 1, 1, &pdblPosition[0]);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 3, 1, 1, &pdblPosition[1]);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &pdblPosition[2]);
        if (sciErr.iErr)
        {
            return -1;
        }

        sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 5, 1, 1, &pdblPosition[3]);
        if (sciErr.iErr)
        {
            return -1;
        }
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getCompoundBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList[] = {"CompoundBorder", "outer", "inner"};
    int iChildBorderOut = 0;
    int* piChildBorderOut = &iChildBorderOut;
    int iChildBorderIn = 0;
    int* piChildBorderIn = &iChildBorderIn;
    int iListSize = 3;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_OUT_BORDER__, jni_int, (void **)&piChildBorderOut);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_IN_BORDER__, jni_int, (void **)&piChildBorderIn);

    if (piChildBorderOut == NULL || piChildBorderIn == NULL)
    {
        iListSize = 1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, iListSize, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, iListSize, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    if (iListSize > 1)
    {
        //get child information and fill current list
        if (getBorder(_pvCtx, _iVar, piAddrList, 2, iChildBorderOut))
        {
            return -1;
        }

        if (getBorder(_pvCtx, _iVar, piAddrList, 3, iChildBorderIn))
        {
            return -1;
        }
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getMatteBorder(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char* pstFieldList[] = {"MatteBorder", "top", "left", "bottom", "right", "color"};
    double* pdblPosition = NULL;
    char* pstColor = NULL;
    int iListSize = 6;

    getGraphicObjectProperty(_iObjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblPosition);
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, iListSize, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, iListSize, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, iListSize, pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 2, 1, 1, &pdblPosition[0]);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 3, 1, 1, &pdblPosition[1]);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &pdblPosition[2]);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 5, 1, 1, &pdblPosition[3]);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 6, 1, 1, &pstColor);
    if (sciErr.iErr)
    {
        return -1;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
