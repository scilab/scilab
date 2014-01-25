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
int getBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getNoBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getLineBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getBevelBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getSoftBevelBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getCommonBevelBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID, int _iBevel);
int getEtchedBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getTitledBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getEmptyBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getCompoundBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
int getMatteBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID);
/*------------------------------------------------------------------------*/
int get_borders_property(void* _pvCtx, int iObjUID)
{
    int iBorder = 0;
    int* piBorder = &iBorder;
    int iRhs = nbInputArgument(_pvCtx) + 1;

    getGraphicObjectProperty(iObjUID, __GO_UI_FRAME_BORDER__, jni_int, (void **)&piBorder);
    if (piBorder == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    return getBorders(_pvCtx, iRhs, 0, 0, iBorder);
}
/*------------------------------------------------------------------------*/
int getBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    int iStyle = 0;
    int* piStyle = &iStyle;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_STYLE__, jni_int, (void **)&piStyle);
    if (piStyle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }


    switch (iStyle)
    {
        default:
        {
            return getNoBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case LINE :
        {
            return getLineBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case BEVEL :
        {
            return getBevelBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case SOFTBEVEL :
        {
            return getSoftBevelBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case ETCHED :
        {
            return getEtchedBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case TITLED :
        {
            return getTitledBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case EMPTY :
        {
            return getEmptyBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case COMPOUND :
        {
            return getCompoundBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
        case MATTE :
        {
            return getMatteBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID);
        }
        break;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getNoBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"NoBorder"};
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

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 1, (const char * const*)&pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getLineBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"LineBorder", "color", "thickness", "rounded"};
    char* pstColor = NULL;
    int iThickness = 0;
    double dblThickness = 0;
    int* piThickness = &iThickness;
    int iRounded = 0;
    int* piRounded = &iRounded;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);
    if (pstColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_LINE_THICKNESS__, jni_int, (void **)&piThickness);
    if (piThickness == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    dblThickness = (double)iThickness;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_ROUNDED__, jni_bool, (void **)&piRounded);
    if (piRounded == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 4, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 4, &piAddrList);
    }

    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 4, (const char * const*)&pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 2, 1, 1, &pstColor);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 3, 1, 1, &dblThickness);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfBooleanInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &iRounded);
    if (sciErr.iErr)
    {
        return -1;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getBevelBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    return getCommonBevelBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID, 1);
}
/*------------------------------------------------------------------------*/
int getSoftBevelBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    return getCommonBevelBorders(_pvCtx, _iVar, _piParent, _iPos, _iObjUID, 2);
}
/*------------------------------------------------------------------------*/
int getCommonBevelBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID, int _iBevel)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"BevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"};
    char * pstSoftFieldList[] = {"SoftBevelBorder", "type", "hlouter", "hlinner", "shadowouter", "shadowinner"};
    int iType = 0;
    int* piType = &iType;
    char* pstType[] = {"raised", "lowered"};
    char* pstHlOutColor = NULL;
    char* pstHlInColor = NULL;
    char* pstShadowOutColor = NULL;
    char* pstShadowInColor = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_TYPE__, jni_int, (void **)&piType);
    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, jni_string, (void **)&pstHlOutColor);
    if (pstHlOutColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_IN__, jni_string, (void **)&pstHlInColor);
    if (pstHlInColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_OUT__, jni_string, (void **)&pstShadowOutColor);
    if (pstShadowOutColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_IN__, jni_string, (void **)&pstShadowInColor);
    if (pstShadowInColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 6, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 6, &piAddrList);
    }
    if (sciErr.iErr)
    {
        return -1;
    }

    if (_iBevel == 1)
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 6, (const char * const*)&pstFieldList);
    }
    else
    {
        sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 6, (const char * const*)&pstSoftFieldList);
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

    return 0;
}
/*------------------------------------------------------------------------*/
int getEtchedBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"EtchedBorder", "type", "hl", "shadow"};
    int iType = 0;
    int* piType = &iType;
    char* pstType[] = {"raised", "lowered"};
    char* pstHlOutColor = NULL;
    char* pstShadowOutColor = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_TYPE__, jni_int, (void **)&piType);
    if (piType == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_HIGHLIGHT_OUT__, jni_string, (void **)&pstHlOutColor);
    if (pstHlOutColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_SHADOW_OUT__, jni_string, (void **)&pstShadowOutColor);
    if (pstShadowOutColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 4, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 4, &piAddrList);
    }

    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 4, (const char * const*)&pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 2, 1, 1, &pstType[iType]);
    if (sciErr.iErr)
    {
        return -1;
    }

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

    return 0;
}
/*------------------------------------------------------------------------*/
int getTitledBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"TitledBorder", "border", "title", "justification", "fontname", "fontangle", "fontsize", "fontweight", "position", "color"};
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

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_TITLE__, jni_int, (void **)&piChildBorder);
    if (piChildBorder == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_TITLE__, jni_string, (void **)&pstTitle);
    if (pstTitle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_JUSTIFICATION__, jni_int, (void **)&piJustification);
    if (piJustification == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTNAME__, jni_string, (void **)&pstFontName);
    if (pstFontName == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTANGLE__, jni_string, (void **)&pstFontAngle);
    if (pstFontAngle == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTSIZE__, jni_int, (void **)&piFontSize);
    if (piFontSize == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    dblFontSize = (double)iFontSize;
    getGraphicObjectProperty(_iObjUID, __GO_UI_FONTWEIGHT__, jni_string, (void **)&pstFontWeight);
    if (pstFontWeight == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_POSITION__, jni_int, (void **)&piPosition);
    if (piPosition == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);
    if (pstColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 10, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 10, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 10, (const char * const*)&pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    //get child information and fill current list
    if (getBorders(_pvCtx, _iVar, piAddrList, 2, iChildBorder))
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 3, 1, 1, &pstTitle);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 4, 1, 1, &pstJustification[iJustification]);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 5, 1, 1, &pstFontName);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 6, 1, 1, &pstFontAngle);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfDoubleInList(_pvCtx, _iVar, piAddrList, 7, 1, 1, &dblFontSize);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 8, 1, 1, &pstFontWeight);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 9, 1, 1, &pstPosition[iPosition]);
    if (sciErr.iErr)
    {
        return -1;
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 10, 1, 1, &pstColor);
    if (sciErr.iErr)
    {
        return -1;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getEmptyBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"EmptyBorder", "top", "left", "bottom", "right"};
    double* pdblPosition = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblPosition);
    if (pdblPosition == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 5, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 5, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 5, (const char * const*)&pstFieldList);
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

    return 0;
}
/*------------------------------------------------------------------------*/
int getCompoundBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"CompoundBorder", "outer", "inner"};
    int iChildBorderOut = 0;
    int* piChildBorderOut = &iChildBorderOut;
    int iChildBorderIn = 0;
    int* piChildBorderIn = &iChildBorderIn;

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_OUT_BORDER__, jni_int, (void **)&piChildBorderOut);
    if (piChildBorderOut == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_IN_BORDER__, jni_int, (void **)&piChildBorderIn);
    if (piChildBorderIn == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 3, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 3, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 3, (const char * const*)&pstFieldList);
    if (sciErr.iErr)
    {
        return -1;
    }

    //get child information and fill current list
    if (getBorders(_pvCtx, _iVar, piAddrList, 2, iChildBorderOut))
    {
        return -1;
    }

    if (getBorders(_pvCtx, _iVar, piAddrList, 3, iChildBorderIn))
    {
        return -1;
    }

    return 0;
}
/*------------------------------------------------------------------------*/
int getMatteBorders(void* _pvCtx, int _iVar, int* _piParent, int _iPos, int _iObjUID)
{
    SciErr sciErr;
    int* piAddrList = NULL;
    char * pstFieldList[] = {"MatteBorder", "top", "left", "bottom", "right", "color"};
    double* pdblPosition = NULL;
    char* pstColor = NULL;

    getGraphicObjectProperty(_iObjUID, __GO_POSITION__, jni_double_vector, (void **)&pdblPosition);
    if (pdblPosition == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    getGraphicObjectProperty(_iObjUID, __GO_UI_FRAME_BORDER_COLOR__, jni_string, (void **)&pstColor);
    if (pstColor == NULL)
    {
        Scierror(999, _("'%s' property does not exist for this handle.\n"), "borders");
        return -1;
    }

    if (_piParent)
    {
        sciErr = createTListInList(_pvCtx, _iVar, _piParent, _iPos, 6, &piAddrList);
    }
    else
    {
        sciErr = createTList(_pvCtx, _iVar, 6, &piAddrList);
    }

    sciErr = createMatrixOfStringInList(_pvCtx, _iVar, piAddrList, 1, 1, 6, (const char * const*)&pstFieldList);
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
