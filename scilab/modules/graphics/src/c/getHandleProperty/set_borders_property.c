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
#include "api_scilab.h"
#include "MALLOC.h"
#include "freeArrayOfString.h"
#include "Scierror.h"
#include "localization.h"
#include "SetPropertyStatus.h"
#include "sci_types.h"
#include "stricmp.h"

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "LayoutType.h"
#include "BorderLayoutType.h"
#include "FillType.h"
#include "AnchorType.h"
/*------------------------------------------------------------------------*/
int clearBorders(int iObjUID);
int createBorder(void* _pvCtx, int* _piAddrList, int _iParent);
int createLineBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
int createBevelBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
int createSoftBevelBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
int createCommonBevelBorder(void* _pvCtx, int* _piAddrList, int _iObjUID, int _iStyle);
int createEtchedBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
int createTitledBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
int createEmptyBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
int createCompoundBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
int createMatteBorder(void* _pvCtx, int* _piAddrList, int _iObjUID);
/*------------------------------------------------------------------------*/
int set_borders_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{
    //[] or tlist
    if (valueType == sci_matrix)
    {
        return clearBorders(iObjUID);
    }
    else
    {
        int* piAddrList = (int*)_pvData;

        int iBorder = createBorder(_pvCtx, piAddrList, iObjUID);
        if (iBorder == 0)
        {
            return SET_PROPERTY_ERROR;
        }

        setGraphicObjectProperty(iObjUID, __GO_UI_FRAME_BORDER__, &iBorder, jni_int, 1);
        return SET_PROPERTY_SUCCEED;
    }
}
/*------------------------------------------------------------------------*/
int createBorder(void* _pvCtx, int* _piAddrList, int _iParent)
{
    SciErr sciErr;
    int* piAddr = NULL;
    char** pstField = NULL;
    int iRows = 0;
    int iCols = 0;
    char* pstType = NULL;


    sciErr = getListItemAddress(_pvCtx, _piAddrList, 1, &piAddr);
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
    if (strcmp(pstType, "NoBorder") == 0)
    {
        return clearBorders(_iParent);
    }
    else if (strcmp(pstType, "LineBorder") == 0)
    {
        return createLineBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "BevelBorder") == 0)
    {
        return createBevelBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "SoftBevelBorder") == 0)
    {
        return createSoftBevelBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "EtchedBorder") == 0)
    {
        return createEtchedBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "TitledBorder") == 0)
    {
        return createTitledBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "EmptyBorder") == 0)
    {
        return createEmptyBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "CompoundBorder") == 0)
    {
        return createCompoundBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "MatteBorder") == 0)
    {
        return createMatteBorder(_pvCtx, _piAddrList, _iParent);
    }

    //empty uid
    return 0;
}
/*------------------------------------------------------------------------*/
int clearBorders(int iObjUID)
{
    BOOL status = FALSE;
    int iBorder = 0;
    int* piBorder = &iBorder;

    getGraphicObjectProperty(iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete current border
        deleteGraphicObject(iBorder);
    }

    iBorder = 0;
    status = setGraphicObjectProperty(iObjUID, __GO_UI_FRAME_BORDER__, &iBorder, jni_int, 1);
    if (status != TRUE)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "contraints");
        return SET_PROPERTY_ERROR;
    }


    return SET_PROPERTY_SUCCEED;
}
/*------------------------------------------------------------------------*/
int createLineBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    //arg1 = string -> string
    //arg2 = double -> int
    //arg3 = boolean -> boolean
    SciErr sciErr;
    int iBorder = 0;
    int* piBorder = &iBorder;
    int iStyle = 1; //FrameBorderType.LINE in java
    int* piAddr2 = NULL;
    char* pstColor = NULL;
    int* piAddr3 = NULL;
    double dblThickness = 0;
    int iThickness = 0;
    int* piAddr4 = NULL;
    int bRounded = 0;

    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr2, &pstColor))
    {
        return SET_PROPERTY_ERROR;
    }

    sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr3, &dblThickness))
    {
        return SET_PROPERTY_ERROR;
    }

    iThickness = (int)dblThickness;

    sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarBoolean(_pvCtx, piAddr4, &bRounded))
    {
        return SET_PROPERTY_ERROR;
    }

    //get current border
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete previous border
        deleteGraphicObject(iBorder);
    }

    //create new border object
    iBorder = createGraphicObject(__GO_UI_FRAME_BORDER__);
    if (iBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_COLOR__, pstColor, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_LINE_THICKNESS__, &iThickness, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_ROUNDED__, &bRounded, jni_bool, 1);

    freeAllocatedSingleString(pstColor);

    return iBorder;
}
/*------------------------------------------------------------------------*/
int createBevelBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    return createCommonBevelBorder(_pvCtx, _piAddrList, _iObjUID, 2);
}
/*------------------------------------------------------------------------*/
int createSoftBevelBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    return createCommonBevelBorder(_pvCtx, _piAddrList, _iObjUID, 3);
}
/*------------------------------------------------------------------------*/
int createCommonBevelBorder(void* _pvCtx, int* _piAddrList, int _iObjUID, int _iStyle)
{
    //arg1 = string -> int ( RAISED or LOWERED )
    //arg2 = string -> string ( color )
    //arg3 = string -> string ( color )
    //arg4 = string -> string ( color )
    //arg5 = string -> string ( color )

    SciErr sciErr;
    int iBorder = 0;
    int* piBorder = &iBorder;
    int* piAddr2 = NULL;
    char* pstType = NULL;
    int iType = 0;
    int* piAddr3 = NULL;
    char* pstHlOutColor = NULL;
    int* piAddr4 = NULL;
    char* pstHlInColor = NULL;
    int* piAddr5 = NULL;
    char* pstShadowOutColor = NULL;
    int* piAddr6 = NULL;
    char* pstShadowInColor = NULL;

    //type
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr2, &pstType))
    {
        return SET_PROPERTY_ERROR;
    }

    if (stricmp(pstType, "raised") == 0)
    {
        iType = 0;
    }
    else if (stricmp(pstType, "lowered") == 0)
    {
        iType = 1;
    }

    freeAllocatedSingleString(pstType);

    //highlight out
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr3, &pstHlOutColor))
    {
        return SET_PROPERTY_ERROR;
    }

    //highlight in
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr4, &pstHlInColor))
    {
        return SET_PROPERTY_ERROR;
    }

    //shadow out
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 5, &piAddr5);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr5, &pstShadowOutColor))
    {
        return SET_PROPERTY_ERROR;
    }

    //shadow in
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 6, &piAddr6);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr6, &pstShadowInColor))
    {
        return SET_PROPERTY_ERROR;
    }

    //get current border
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete previous border
        deleteGraphicObject(iBorder);
    }

    //create new border object
    iBorder = createGraphicObject(__GO_UI_FRAME_BORDER__);
    if (iBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &_iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_TYPE__, &iType, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, pstHlOutColor, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, pstHlInColor, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_SHADOW_OUT__, pstShadowOutColor, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_SHADOW_IN__, pstShadowInColor, jni_string, 1);

    return iBorder;
}
/*------------------------------------------------------------------------*/
int createEtchedBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    //arg1 = double -> int ( RAISED or LOWERED )
    //arg2 = string -> string ( color ) mapped on hl out
    //arg3 = string -> string ( color ) mapped on shadow out
    SciErr sciErr;
    int iBorder = 0;
    int* piBorder = &iBorder;

    int iStyle = 4; //FrameBorderType.ETCHED in java
    int* piAddr2 = NULL;
    char* pstType = NULL;
    int iType = 0;
    int* piAddr3 = NULL;
    char* pstHlOutColor = NULL;
    int* piAddr4 = NULL;
    char* pstShadowOutColor = NULL;

    //type
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr2, &pstType))
    {
        return SET_PROPERTY_ERROR;
    }

    if (stricmp(pstType, "raised") == 0)
    {
        iType = 0;
    }
    else if (stricmp(pstType, "lowered") == 0)
    {
        iType = 1;
    }

    freeAllocatedSingleString(pstType);

    //highlight out
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr3, &pstHlOutColor))
    {
        return SET_PROPERTY_ERROR;
    }

    //shadow out
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr4, &pstShadowOutColor))
    {
        return SET_PROPERTY_ERROR;
    }


    //get current border
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete previous border
        deleteGraphicObject(iBorder);
    }

    //create new border object
    iBorder = createGraphicObject(__GO_UI_FRAME_BORDER__);
    if (iBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_TYPE__, &iType, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, pstHlOutColor, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_SHADOW_OUT__, pstShadowOutColor, jni_string, 1);

    return iBorder;
}
/*------------------------------------------------------------------------*/
int createTitledBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    //arg1 = another border
    //arg2 = string -> string
    //arg3 = string -> int ( enum  JustificationType in java )
    //arg4 = string -> string
    //arg5 = string -> string
    //arg6 = double -> int
    //arg7 = string -> string
    //arg8 = string -> int (  enum TitlePositionType in java )
    //arg9 = string -> string
    SciErr sciErr;
    int iBorder = 0;
    int* piBorder = &iBorder;
    int iStyle = 5; //FrameBorderType.TITLED in java
    int* piAddr2 = NULL;
    int iChildBorder = 0;
    int* piAddr3 = NULL;
    char* pstTitle = NULL;
    int* piAddr4 = NULL;
    char* pstJustification = NULL;
    int iJustification = 0;
    int* piAddr5 = NULL;
    char* pstFontName = NULL;
    int* piAddr6 = NULL;
    char* pstFontAngle = NULL;
    int* piAddr7 = NULL;
    double dblFontSize = 0;
    int iFontSize = 0;
    int* piAddr8 = NULL;
    char* pstFontWeight = NULL;
    int* piAddr9 = NULL;
    char* pstPosition = NULL;
    int iPosition = 0;
    int* piAddr10 = NULL;
    char* pstColor = NULL;

    //child border
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    //title
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr3, &pstTitle))
    {
        return SET_PROPERTY_ERROR;
    }

    //justification
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr4, &pstJustification))
    {
        return SET_PROPERTY_ERROR;
    }

    if (stricmp(pstJustification, "leading") == 0)
    {
        iJustification = 0;
    }
    else if (stricmp(pstJustification, "left") == 0)
    {
        iJustification = 1;
    }
    else if (stricmp(pstJustification, "center") == 0)
    {
        iJustification = 2;
    }
    else if (stricmp(pstJustification, "right") == 0)
    {
        iJustification = 3;
    }
    else if (stricmp(pstJustification, "trailing") == 0)
    {
        iJustification = 4;
    }

    freeAllocatedSingleString(pstJustification);

    //fontname
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 5, &piAddr5);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr5, &pstFontName))
    {
        return SET_PROPERTY_ERROR;
    }

    //fontangle
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 6, &piAddr6);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr6, &pstFontAngle))
    {
        return SET_PROPERTY_ERROR;
    }

    //fontsize
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 7, &piAddr7);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr7, &dblFontSize))
    {
        return SET_PROPERTY_ERROR;
    }

    iFontSize = (int)dblFontSize;

    //fontweight
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 8, &piAddr8);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr8, &pstFontWeight))
    {
        return SET_PROPERTY_ERROR;
    }

    //position
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 9, &piAddr9);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr9, &pstPosition))
    {
        return SET_PROPERTY_ERROR;
    }

    if (stricmp(pstPosition, "top") == 0)
    {
        iPosition = 0;
    }
    else if (stricmp(pstPosition, "above_top") == 0)
    {
        iPosition = 1;
    }
    else if (stricmp(pstPosition, "below_top") == 0)
    {
        iPosition = 2;
    }
    else if (stricmp(pstPosition, "bottom") == 0)
    {
        iPosition = 3;
    }
    else if (stricmp(pstPosition, "above_bottom") == 0)
    {
        iPosition = 4;
    }
    else if (stricmp(pstPosition, "below_bottom") == 0)
    {
        iPosition = 5;
    }

    freeAllocatedSingleString(pstPosition);

    //color
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 10, &piAddr10);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr10, &pstColor))
    {
        return SET_PROPERTY_ERROR;
    }

    //get current border
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete previous border
        deleteGraphicObject(iBorder);
    }

    //create new border object
    iBorder = createGraphicObject(__GO_UI_FRAME_BORDER__);
    if (iBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    iChildBorder = createBorder(_pvCtx, piAddr2, iBorder);
    if (iChildBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }


    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_TITLE__, &iChildBorder, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_TITLE__, pstTitle, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_JUSTIFICATION__, &iJustification, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FONTNAME__, pstFontName, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FONTANGLE__, pstFontAngle, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FONTSIZE__, &iFontSize, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FONTWEIGHT__, pstFontWeight, jni_string, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_POSITION__, &iPosition, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_COLOR__, pstColor, jni_string, 1);

    return iBorder;
}
/*------------------------------------------------------------------------*/
int createEmptyBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    //arg1 = double -> double
    //arg2 = double -> double
    //arg3 = double -> double
    //arg4 = double -> double
    SciErr sciErr;
    int iBorder = 0;
    int* piBorder = &iBorder;
    int iStyle = 6; //FrameBorderType.EMPTY in java
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;
    int* piAddr5 = NULL;
    double pdblPosition[4];

    //top
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr2, &pdblPosition[0]))
    {
        return SET_PROPERTY_ERROR;
    }

    //left
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr3, &pdblPosition[1]))
    {
        return SET_PROPERTY_ERROR;
    }

    //bottom
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr4, &pdblPosition[2]))
    {
        return SET_PROPERTY_ERROR;
    }

    //right
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 5, &piAddr5);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr5, &pdblPosition[3]))
    {
        return SET_PROPERTY_ERROR;
    }

    //get current border
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete previous border
        deleteGraphicObject(iBorder);
    }

    //create new border object
    iBorder = createGraphicObject(__GO_UI_FRAME_BORDER__);
    if (iBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_POSITION__, pdblPosition, jni_double_vector, 4);
    return iBorder;
}
/*------------------------------------------------------------------------*/
int createCompoundBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    SciErr sciErr;
    int iBorder = 0;
    int* piBorder = &iBorder;
    int iStyle = 7; //FrameBorderType.COMPOUND in java
    int* piAddr2 = NULL;
    int iChildBorderOut = 0;
    int* piAddr3 = NULL;
    int iChildBorderIn = 0;

    //child border out
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    iChildBorderOut = createBorder(_pvCtx, piAddr2, _iObjUID);
    if (iChildBorderOut == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    //child border in
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    iChildBorderIn = createBorder(_pvCtx, piAddr3, _iObjUID);
    if (iChildBorderIn == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    //get current border
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete previous border
        deleteGraphicObject(iBorder);
    }

    //create new border object
    iBorder = createGraphicObject(__GO_UI_FRAME_BORDER__);
    if (iBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_OUT_BORDER__, &iChildBorderOut, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_IN_BORDER__, &iChildBorderIn, jni_int, 1);
    return iBorder;
}
/*------------------------------------------------------------------------*/
int createMatteBorder(void* _pvCtx, int* _piAddrList, int _iObjUID)
{
    //arg1 = double -> double
    //arg2 = double -> double
    //arg3 = double -> double
    //arg4 = double -> double
    //arg4 = string -> string
    SciErr sciErr;
    int iBorder = 0;
    int* piBorder = &iBorder;
    int iStyle = 8; //FrameBorderType.MATTE in java
    int* piAddr2 = NULL;
    int* piAddr3 = NULL;
    int* piAddr4 = NULL;
    int* piAddr5 = NULL;
    double pdblPosition[4];
    int* piAddr6 = NULL;
    char* pstColor = NULL;

    //top
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr2, &pdblPosition[0]))
    {
        return SET_PROPERTY_ERROR;
    }

    //left
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr3, &pdblPosition[1]))
    {
        return SET_PROPERTY_ERROR;
    }

    //bottom
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr4, &pdblPosition[2]))
    {
        return SET_PROPERTY_ERROR;
    }

    //right
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 5, &piAddr5);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getScalarDouble(_pvCtx, piAddr5, &pdblPosition[3]))
    {
        return SET_PROPERTY_ERROR;
    }

    //color
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 6, &piAddr6);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr6, &pstColor))
    {
        return SET_PROPERTY_ERROR;
    }

    //get current border
    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void**)&piBorder);
    if (piBorder != NULL && iBorder != 0)
    {
        //delete previous border
        deleteGraphicObject(iBorder);
    }

    //create new border object
    iBorder = createGraphicObject(__GO_UI_FRAME_BORDER__);
    if (iBorder == 0)
    {
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_POSITION__, pdblPosition, jni_double_vector, 4);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_COLOR__, pstColor, jni_string, 1);
    return iBorder;
}
/*------------------------------------------------------------------------*/
