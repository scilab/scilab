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

#include "createGraphicObject.h"
#include "deleteGraphicObject.h"
#include "setGraphicObjectProperty.h"
#include "getGraphicObjectProperty.h"
#include "graphicObjectProperties.h"
#include "BorderLayoutType.h"
/*------------------------------------------------------------------------*/
int clearBorder(int iObjUID);
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
int set_border_property(void* _pvCtx, int iObjUID, void* _pvData, int valueType, int nbRow, int nbCol)
{

    if ((valueType != sci_tlist && valueType != sci_matrix) || (valueType == sci_matrix && (nbRow != 0 || nbCol != 0)))
    {
        Scierror(999, _("Wrong type for '%s' property: An empty matrix or a tlist expected.\n"), "border");
        return SET_PROPERTY_ERROR;
    }
    else if (valueType == sci_matrix)
    {
        return clearBorder(iObjUID);
    }
    else
    {
        int iHidden = 1;
        int* piAddrList = (int*)_pvData;

        int iBorder = createBorder(_pvCtx, piAddrList, iObjUID);
        if (iBorder == 0)
        {
            return SET_PROPERTY_ERROR;
        }

        setGraphicObjectProperty(iObjUID, __GO_UI_FRAME_BORDER__, &iBorder, jni_int, 1);
        setGraphicObjectProperty(iBorder, __GO_HIDDEN__, &iHidden, jni_bool, 1);
        setGraphicObjectRelationship(iObjUID, iBorder);
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
    int ret = 0; //empty uid

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
        ret = clearBorder(_iParent);
    }
    else if (strcmp(pstType, "LineBorder") == 0)
    {
        ret = createLineBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "BevelBorder") == 0)
    {
        ret = createBevelBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "SoftBevelBorder") == 0)
    {
        ret = createSoftBevelBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "EtchedBorder") == 0)
    {
        ret = createEtchedBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "TitledBorder") == 0)
    {
        ret = createTitledBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "EmptyBorder") == 0)
    {
        ret = createEmptyBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "CompoundBorder") == 0)
    {
        ret = createCompoundBorder(_pvCtx, _piAddrList, _iParent);
    }
    else if (strcmp(pstType, "MatteBorder") == 0)
    {
        ret = createMatteBorder(_pvCtx, _piAddrList, _iParent);
    }

    freeAllocatedMatrixOfString(iRows, iCols, pstField);
    return ret;
}
/*------------------------------------------------------------------------*/
int clearBorder(int iObjUID)
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
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "constraints");
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
    int iCount = 0;

    sciErr = getListItemNumber(_pvCtx, _piAddrList, &iCount);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    //color
    sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (getAllocatedSingleString(_pvCtx, piAddr2, &pstColor))
    {
        return SET_PROPERTY_ERROR;
    }


    //thickness
    if (iCount > 2)
    {
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 3, &piAddr3);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstColor);
            return SET_PROPERTY_ERROR;
        }

        if (getScalarDouble(_pvCtx, piAddr3, &dblThickness))
        {
            freeAllocatedSingleString(pstColor);
            return SET_PROPERTY_ERROR;
        }

        iThickness = (int)dblThickness;
    }

    //rounded
    if (iCount > 3)
    {
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstColor);
            return SET_PROPERTY_ERROR;
        }

        if (getScalarBoolean(_pvCtx, piAddr4, &bRounded))
        {
            freeAllocatedSingleString(pstColor);
            return SET_PROPERTY_ERROR;
        }
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
        freeAllocatedSingleString(pstColor);
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_COLOR__, pstColor, jni_string, 1);
    freeAllocatedSingleString(pstColor);

    if (iCount > 2)
    {
        setGraphicObjectProperty(iBorder, __GO_LINE_THICKNESS__, &iThickness, jni_int, 1);
    }

    if (iCount > 3)
    {
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_ROUNDED__, &bRounded, jni_bool, 1);
    }


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
    int iCount = 0;

    sciErr = getListItemNumber(_pvCtx, _piAddrList, &iCount);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

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

    //highlight out and highlight in or hl and shadow
    if (iCount > 2)
    {
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
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr4, &pstHlInColor))
        {
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }
    }

    if (iCount > 4)
    {
        //shadow out
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 5, &piAddr5);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstHlInColor);
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr5, &pstShadowOutColor))
        {
            freeAllocatedSingleString(pstHlInColor);
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }

        //shadow in
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 6, &piAddr6);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstShadowOutColor);
            freeAllocatedSingleString(pstHlInColor);
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr6, &pstShadowInColor))
        {
            freeAllocatedSingleString(pstShadowOutColor);
            freeAllocatedSingleString(pstHlInColor);
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }
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
        freeAllocatedSingleString(pstShadowInColor);
        freeAllocatedSingleString(pstShadowOutColor);
        freeAllocatedSingleString(pstHlInColor);
        freeAllocatedSingleString(pstHlOutColor);
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &_iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_TYPE__, &iType, jni_int, 1);

    if (iCount > 2)
    {
        if (iCount == 4)
        {
            setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, pstHlOutColor, jni_string, 1);
            setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_SHADOW_OUT__, pstHlInColor, jni_string, 1);

            freeAllocatedSingleString(pstHlOutColor);
            freeAllocatedSingleString(pstHlInColor);
        }
        else
        {
            setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, pstHlOutColor, jni_string, 1);
            setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, pstHlInColor, jni_string, 1);
            setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_SHADOW_OUT__, pstShadowOutColor, jni_string, 1);
            setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_SHADOW_IN__, pstShadowInColor, jni_string, 1);

            freeAllocatedSingleString(pstHlOutColor);
            freeAllocatedSingleString(pstHlInColor);
            freeAllocatedSingleString(pstShadowOutColor);
            freeAllocatedSingleString(pstShadowInColor);
        }
    }

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
    int iCount = 0;

    sciErr = getListItemNumber(_pvCtx, _piAddrList, &iCount);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (iCount == 2 || iCount == 4)
    {
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
    }

    if (iCount == 3 || iCount == 4)
    {
        int iPos = iCount - 1;
        //highlight out
        sciErr = getListItemAddress(_pvCtx, _piAddrList, iPos, &piAddr3);
        if (sciErr.iErr)
        {
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr3, &pstHlOutColor))
        {
            return SET_PROPERTY_ERROR;
        }

        //shadow out
        sciErr = getListItemAddress(_pvCtx, _piAddrList, iPos + 1, &piAddr4);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr4, &pstShadowOutColor))
        {
            freeAllocatedSingleString(pstHlOutColor);
            return SET_PROPERTY_ERROR;
        }
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
        freeAllocatedSingleString(pstShadowOutColor);
        freeAllocatedSingleString(pstHlOutColor);
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    if (iCount == 2 || iCount == 4)
    {
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_TYPE__, &iType, jni_int, 1);
    }

    if (iCount == 3 || iCount == 4)
    {
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, pstHlOutColor, jni_string, 1);
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_SHADOW_OUT__, pstShadowOutColor, jni_string, 1);

        freeAllocatedSingleString(pstHlOutColor);
        freeAllocatedSingleString(pstShadowOutColor);
    }

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
    int iPosition = 0;
    char* pstPosition = NULL;
    int* piAddr6 = NULL;
    int* piFont = NULL;
    int* piAddr7 = NULL;
    char* pstColor = NULL;

    //font data
    char* pstFontName = NULL;
    int iFontSize = 0;
    char* pstFontAngle = NULL;
    char* pstFontWeight = NULL;

    int iCount = 0;

    sciErr = getListItemNumber(_pvCtx, _piAddrList, &iCount);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (iCount == 2)
    {
        //child border or title
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 2, &piAddr2);
        if (sciErr.iErr)
        {
            return SET_PROPERTY_ERROR;
        }
    }

    if (iCount > 2)
    {
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
    }

    if (iCount > 3)
    {
        //justification
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 4, &piAddr4);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr4, &pstJustification))
        {
            freeAllocatedSingleString(pstTitle);
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

        //position
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 5, &piAddr5);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr5, &pstPosition))
        {
            freeAllocatedSingleString(pstTitle);
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
    }

    if (iCount > 5)
    {
        int* piAddrFont = NULL;
        double dblFontSize = 0;
        //font
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 6, &piAddr6);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            return SET_PROPERTY_ERROR;
        }

        //read data from font tlist
        sciErr = getListItemAddress(_pvCtx, piAddr6, 2, &piAddrFont);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddrFont, &pstFontName))
        {
            freeAllocatedSingleString(pstTitle);
            return SET_PROPERTY_ERROR;
        }

        sciErr = getListItemAddress(_pvCtx, piAddr6, 3, &piAddrFont);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            return SET_PROPERTY_ERROR;
        }

        if (getScalarDouble(_pvCtx, piAddrFont, &dblFontSize))
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            return SET_PROPERTY_ERROR;
        }

        iFontSize = (int)dblFontSize;

        sciErr = getListItemAddress(_pvCtx, piAddr6, 4, &piAddrFont);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddrFont, &pstFontAngle))
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            return SET_PROPERTY_ERROR;
        }

        sciErr = getListItemAddress(_pvCtx, piAddr6, 5, &piAddrFont);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            freeAllocatedSingleString(pstFontAngle);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddrFont, &pstFontWeight))
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            freeAllocatedSingleString(pstFontAngle);
            return SET_PROPERTY_ERROR;
        }
    }

    if (iCount > 6)
    {
        //color
        sciErr = getListItemAddress(_pvCtx, _piAddrList, 7, &piAddr7);
        if (sciErr.iErr)
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            freeAllocatedSingleString(pstFontAngle);
            freeAllocatedSingleString(pstFontWeight);
            return SET_PROPERTY_ERROR;
        }

        if (getAllocatedSingleString(_pvCtx, piAddr7, &pstColor))
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            freeAllocatedSingleString(pstFontAngle);
            freeAllocatedSingleString(pstFontWeight);
            return SET_PROPERTY_ERROR;
        }
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
        freeAllocatedSingleString(pstTitle);
        freeAllocatedSingleString(pstFontName);
        freeAllocatedSingleString(pstFontAngle);
        freeAllocatedSingleString(pstFontWeight);
        freeAllocatedSingleString(pstColor);
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);

    if (iCount == 2)
    {
        if (isTListType(_pvCtx, piAddr2))
        {
            int iHidden = 1;
            iChildBorder = createBorder(_pvCtx, piAddr2, iBorder);
            if (iChildBorder == 0)
            {
                return SET_PROPERTY_ERROR;
            }

            setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_TITLE__, &iChildBorder, jni_int, 1);
            setGraphicObjectProperty(iChildBorder, __GO_HIDDEN__, &iHidden, jni_bool, 1);
            setGraphicObjectRelationship(iBorder, iChildBorder);
        }
        else //title
        {
            if (getAllocatedSingleString(_pvCtx, piAddr2, &pstTitle))
            {
                return SET_PROPERTY_ERROR;
            }

            setGraphicObjectProperty(iBorder, __GO_TITLE__, pstTitle, jni_string, 1);

            freeAllocatedSingleString(pstTitle);
        }

        return iBorder;
    }


    if (iCount > 2)
    {
        iChildBorder = createBorder(_pvCtx, piAddr2, iBorder);
        if (iChildBorder == 0)
        {
            freeAllocatedSingleString(pstTitle);
            freeAllocatedSingleString(pstFontName);
            freeAllocatedSingleString(pstFontAngle);
            freeAllocatedSingleString(pstFontWeight);
            freeAllocatedSingleString(pstColor);
            return SET_PROPERTY_ERROR;
        }

        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_TITLE__, &iChildBorder, jni_int, 1);
        setGraphicObjectProperty(iBorder, __GO_TITLE__, pstTitle, jni_string, 1);

        freeAllocatedSingleString(pstTitle);
    }

    if (iCount > 3)
    {
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_JUSTIFICATION__, &iJustification, jni_int, 1);
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_POSITION__, &iPosition, jni_int, 1);
    }

    if (iCount > 5)
    {
        setGraphicObjectProperty(iBorder, __GO_UI_FONTNAME__, pstFontName, jni_string, 1);
        setGraphicObjectProperty(iBorder, __GO_UI_FONTANGLE__, pstFontAngle, jni_string, 1);
        setGraphicObjectProperty(iBorder, __GO_UI_FONTSIZE__, &iFontSize, jni_int, 1);
        setGraphicObjectProperty(iBorder, __GO_UI_FONTWEIGHT__, pstFontWeight, jni_string, 1);

        freeAllocatedSingleString(pstFontName);
        freeAllocatedSingleString(pstFontAngle);
        freeAllocatedSingleString(pstFontWeight);
    }

    if (iCount > 6)
    {
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_COLOR__, pstColor, jni_string, 1);

        freeAllocatedSingleString(pstColor);
    }

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
    int iCount = 0;

    sciErr = getListItemNumber(_pvCtx, _piAddrList, &iCount);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (iCount > 1)
    {
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

    if (iCount > 1)
    {
        setGraphicObjectProperty(iBorder, __GO_POSITION__, pdblPosition, jni_double_vector, 4);
    }

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
    int iCount = 0;

    sciErr = getListItemNumber(_pvCtx, _piAddrList, &iCount);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

    if (iCount > 1)
    {
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

    if (iCount > 1)
    {
        int iHidden = 1;
        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_OUT_BORDER__, &iChildBorderOut, jni_int, 1);
        setGraphicObjectProperty(iChildBorderOut, __GO_HIDDEN__, &iHidden, jni_bool, 1);
        setGraphicObjectRelationship(iBorder, iChildBorderOut);

        setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_IN_BORDER__, &iChildBorderIn, jni_int, 1);
        setGraphicObjectProperty(iChildBorderIn, __GO_HIDDEN__, &iHidden, jni_bool, 1);
        setGraphicObjectRelationship(iBorder, iChildBorderIn);
    }

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
    int iCount = 0;

    sciErr = getListItemNumber(_pvCtx, _piAddrList, &iCount);
    if (sciErr.iErr)
    {
        return SET_PROPERTY_ERROR;
    }

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
        freeAllocatedSingleString(pstColor);
        return SET_PROPERTY_ERROR;
    }

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_STYLE__, &iStyle, jni_int, 1);
    setGraphicObjectProperty(iBorder, __GO_POSITION__, pdblPosition, jni_double_vector, 4);

    setGraphicObjectProperty(iBorder, __GO_UI_FRAME_BORDER_COLOR__, pstColor, jni_string, 1);
    freeAllocatedSingleString(pstColor);

    return iBorder;
}
/*------------------------------------------------------------------------*/
