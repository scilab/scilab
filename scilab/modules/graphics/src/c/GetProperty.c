/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2002 - 2004 - INRIA - Serge Steer
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010-2012 - DIGITEO - Manuel Juliachs
 * Copyright (C) 2010 - Paul Griffiths
 *
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

/*------------------------------------------------------------------------
 *    Graphic library
 *    newGraph Library header
 *    Comment:
 *    This file contains all functions used to GET the properties of graphics
 *    objects.
 --------------------------------------------------------------------------*/

#include <stdio.h>
#include <string.h>

#include "GetProperty.h"
#include "Scierror.h"
#include "InitObjects.h"
#include "CurrentFigure.h"
#include "ObjectSelection.h"
#include "GetJavaProperty.h"
#include "BasicAlgos.h"
#include "localization.h"
#include "Axes.h"
#include "stack-c.h"
#include "HandleManagement.h"

#include "MALLOC.h" /* MALLOC */

#include "graphicObjectProperties.h"
#include "getGraphicObjectProperty.h"
#include "FigureModel.h"

/**sciGetEntityType
* Gets the type of this Scilab graphic object return -1 if bad param !
*/
sciEntityType
sciGetEntityType (sciPointObj * pobj)
{
    // TODO
    // Call assert to force no more call to this method.
    printf("[ABORT] sciGetEntityType must no more be called.\n");
    printf("[ABORT] scilab will now abort.\n");
    abort();

    if (pobj != (sciPointObj *) NULL)
        return pobj->entitytype;
    return (sciEntityType) - 1;
}


/**sciGetGraphicContext
* Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
*/
sciGraphicContext *
sciGetGraphicContext (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            return &(pFIGURE_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_SUBWIN:
            return  &(pSUBWIN_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_ARC:
            return  &(pARC_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_SEGS:
            return  &(pSEGS_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_FEC:
            return  &(pFEC_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_GRAYPLOT:
            return  &(pGRAYPLOT_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_POLYLINE:
            return  &(pPOLYLINE_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_RECTANGLE:
            return  &(pRECTANGLE_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_SURFACE:
            return  &(pSURFACE_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_AXES:
            return  &(pAXES_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_LEGEND:
            return  &(pLEGEND_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_TEXT:
            return  &(pTEXT_FEATURE (pobj)->graphiccontext);
            break;
        case SCI_LABEL: /* F.Leray 28.05.04, modif JB.Silvy 03/06 */
            return sciGetGraphicContext( pLABEL_FEATURE(pobj)->text );
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            return (sciGraphicContext *) NULL;
            break;
    }
    return (sciGraphicContext *) NULL;
}


/**sciGetNumColors
* This function gets the number of the color defined in colormap
*/
int
sciGetNumColors (char *pobjUID)
{
    if (pobjUID)
    {
        char* parentFigure = NULL;
        int iNumColors = 0;
        int* piNumColors = &iNumColors;

        getGraphicObjectProperty(pobjUID, __GO_PARENT_FIGURE__, jni_string, (void **)&parentFigure);
        getGraphicObjectProperty(parentFigure, __GO_COLORMAP_SIZE__, jni_int, (void**)&piNumColors);

        return iNumColors;
    }
    return -1;
}

/**sciGetFontContext
* Returns the structure of the Font Context. Do not use this in the Consturctor Functions !
* @param sciPointObj * pobj: the pointer to the entity
* @return the pointer to the font context structure if ok, NULL if not
*/
sciFont *
sciGetFontContext (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {
        case SCI_TEXT:
            return  &(pTEXT_FEATURE (pobj)->fontcontext);
            break;
        case SCI_LEGEND:
            return  &(pLEGEND_FEATURE (pobj)->text.fontcontext);
            break;
        case SCI_SUBWIN: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
            return &(pSUBWIN_FEATURE (pobj)->axes.fontcontext);
            break;
        case SCI_AXES:
            return  &(pAXES_FEATURE (pobj)->fontcontext);
            break;
        case SCI_FIGURE: /* F.Leray 08.04.04 THE MOST IMPORTANT*/
            return &(pFIGURE_FEATURE (pobj)->fontcontext);
            break;
        case SCI_LABEL: /* F.Leray 27.05.04 */
            return sciGetFontContext( pLABEL_FEATURE(pobj)->text ) ;
            break;
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            return (sciFont *)NULL;
            break;
    }
}



/****************************************** TEXT ******************************
**sciGetFontSize
* Gets the Font Size
*/
double sciGetFontSize(char * pobjUID)
{
    abort();
#if 0
    if (sciGetFontContext(pobj) != NULL)
    {
        return (sciGetFontContext(pobj))->fontSize;
    }
    else
    {
        printSetGetErrorMessage("font_size");
        return -1.0;
    }
#endif
    return -1.0;
}


/*sciGetFontOrientation
* gets the Font Orientation
*/
double
sciGetFontOrientation (sciPointObj * pobj)
{

    if (sciGetFontContext(pobj) != NULL)
    {
        return (sciGetFontContext(pobj))->textorientation;
    }
    printSetGetErrorMessage("font_angle");
    return 0.0;

}




/**sciGetText
* Gets the Text in TEXT, TITLE or LEGEND. Be Carreful,
* this return a unterminated string (without \0)
* To obtain the length of the text, use sciGetTextLength@param  sciPointObj *pobj
* @param sciPointObj * pobj: the pointer to the entity
* @return  point to char[] if OK or NULL if not
*/
StringMatrix * sciGetText( sciPointObj * pobj )
{

    switch (sciGetEntityType (pobj))
    {
        case SCI_TEXT:
            return pTEXT_FEATURE (pobj)->pStrings;
            break;
        case SCI_LEGEND:
            return pLEGEND_FEATURE (pobj)->text.pStrings;
            break;
        case SCI_LABEL: /* F.Leray 28.05.04 */
            return sciGetText( pLABEL_FEATURE (pobj)->text ) ;
            break;
        case SCI_UIMENU:
        case SCI_FIGURE:
        case SCI_SUBWIN:
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_AGREG:
        default:
            return  NULL;
            break;
    }
}

/**
* Get the size of the string matrix of a text object
* @param nbRow, number of row of the text conatianed in pobj
*        If an error occures, the value is negative.
* @param nbCol, same as nbRow for columns.
*/
void sciGetTextSize( sciPointObj * pobj, int * nbRow, int * nbCol )
{
    StringMatrix * text = sciGetText( pobj ) ;
    if ( text == NULL )
    {
        *nbRow = -1 ;
        *nbCol = -1 ;
    }
    else
    {
        *nbRow = getMatNbRow( text ) ;
        *nbCol = getMatNbCol( text ) ;
    }
}
/**
* Checks if a text object is empty #rows*#columns==0 or #rows*#columns==1 and entry is  zero length
* This function has been adapted to the MVC: its parameter's type has been changed from sciPointObj to char*
* (MVC identifier), the reason being that it is exclusively used to get the properties of Label objects,
* which can be only be known by their MVC identifiers at the present moment.
*/
//BOOL sciisTextEmpty( sciPointObj * pobj)

BOOL sciisTextEmpty(char* identifier)
{
    int nbElements = 0;
    int* dimensions = NULL;

    getGraphicObjectProperty(identifier, __GO_TEXT_ARRAY_DIMENSIONS__, jni_int_vector, (void **)&dimensions);

    if (dimensions == NULL)
    {
        return TRUE;
    }

    nbElements = dimensions[0] * dimensions[1];

    if (nbElements == 0)
    {
        return TRUE;
    }

    if (nbElements == 1)
    {
        char** textMatrix = NULL;
        getGraphicObjectProperty(identifier, __GO_TEXT_STRINGS__, jni_string_vector, (void **) &textMatrix);

        if (textMatrix[0] == NULL)
        {
            return TRUE;
        }
        else if (strcmp(textMatrix[0], "") == 0)
        {
            /* empty string */
            return TRUE;
        }

    }

    return FALSE;
}

/**sciGetFontStyle
* Gets the font style

* @param sciPointObj * pobj: the pointer to the entity
* @return  int 0 OK, -1 if not
*/
int
sciGetFontStyle (sciPointObj * pobj)
{


    switch (sciGetEntityType (pobj))
    {
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_SUBWIN:
        case SCI_FIGURE:
        case SCI_LABEL: /* F.Leray 10.06.04 */
        case SCI_AXES:
            return sciGetFontContext(pobj)->fonttype;
            break;
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            printSetGetErrorMessage("font_style");
            return -1;
            break;
    }
}


/**sciGetLegendPlace
* Returns the Title place with SCI_TITLE_IN_TOP or SCI_TITLE_IN_BOTTOM and calculate the real position in the window
* @param sciPointObj * pobj: the pointer to the entity
* @return sciTitlePlace
*/
sciLegendPlace
sciGetLegendPlace (sciPointObj * pobj)
{
    if ( sciGetEntityType(pobj) == SCI_LEGEND )
    {
        return pLEGEND_FEATURE (pobj)->place ;
    }
    else
    {
        Scierror(999, _("You are not using a legend object.\n"));
        return SCI_LEGEND_OUT_LOWER_LEFT;
    }
    return SCI_LEGEND_OUT_LOWER_LEFT;
}



/**sciGetLegendPos
* Returns the Title Position
*/

void sciGetLegendPos (sciPointObj * pobj, double position[2])
{
    POINT2D tmppoint;
    if (sciGetEntityType (pobj) == SCI_LEGEND)
    {
        tmppoint = pLEGEND_FEATURE (pobj)->pos;
        position[0] = tmppoint.x;
        position[1] = tmppoint.y;
    }

    else
    {
        position[0] = -1;
        position[1] = -1;
        Scierror(999, _("You are not using a legend object.\n"));
        return;
    }
}




/**sciGetParentFigure
* Returns the the Figure parent
*/
sciPointObj *
sciGetParentFigure (sciPointObj * pobj)
{

    if (pobj == NULL)
    {
        return NULL;
    }

    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            return pobj ;
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_SUBWIN:
        case SCI_AGREG:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        case SCI_UICONTROL:
        case SCI_UIMENU:
        {
            return sciGetParentFigure( sciGetParent( pobj ) ) ; /* jbs 06/2006 */
        }
        break;
        default:
            return NULL;
            break;
    }
    return NULL;
}
/** 19/09/2002 ***/
/**sciGetParentSubwin
* Returns the the Subwin parent
*/
sciPointObj *
sciGetParentSubwin (sciPointObj * pobj)
{
    sciPointObj *subwin = NULL;

    subwin = pobj;
    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            return sciGetFirstTypedSelectedSon( pobj, SCI_SUBWIN );
            break;
        case SCI_SUBWIN:
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_AGREG:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        case SCI_UIMENU:
            while (sciGetEntityType(subwin) != SCI_SUBWIN)
                subwin = sciGetParent(subwin);
            return (sciPointObj *) subwin;
            break;
        default:
            return NULL;
            break;
    }
    return (sciPointObj *) NULL;
}

/**sciGetNumfigure
* Returns the the Number of parent figure
*/
int
sciGetNumFigure (sciPointObj * pobj)
{
    sciPointObj *figure = pobj;

    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            return  sciGetNum(figure) ;
            break;
        case SCI_SUBWIN:
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_AGREG:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        case SCI_UIMENU:
            while (sciGetEntityType(figure) != SCI_FIGURE)
                figure = sciGetParent(figure);
            return sciGetNum(figure);
            break;
        default:
            return -1;
            break;
    }
    return -1;
}

/**sciGetGraphicMode
* Returns the structure of the Graphic Context. Do not use this in the Consturctor Functions !
*/
scigMode *sciGetGraphicMode (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            return &(pFIGURE_FEATURE (pobj)->gmode);
            break;
        case SCI_SUBWIN:
            return &(pSUBWIN_FEATURE (pobj)->gmode);
            break;
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_AGREG:
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        case SCI_UIMENU:
        default:
            return (scigMode *) NULL;
            break;
    }
    return (scigMode *) NULL;
}


/**sciGetIsClipRegionValuated*/
/* To know if clip_region has been set at least once */
/* (using sciSetClipping) */
int
sciGetIsClipRegionValuated (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_ARC:
            return pARC_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_RECTANGLE:
            return pRECTANGLE_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_SEGS:
            return pSEGS_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_TEXT:
            return pTEXT_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_AXES:
            return pAXES_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_SURFACE:
            return pSURFACE_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_FEC:
            return pFEC_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_GRAYPLOT:
            return pGRAYPLOT_FEATURE (pobj)->clip_region_set;
            break;
        case SCI_LEGEND:
            return pLEGEND_FEATURE(pobj)->clip_region_set;
        case SCI_AGREG:
        case SCI_FIGURE:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        case SCI_UIMENU:
        default:
            return -2;
            break;
    }
    return -2;
}


/**sciGetIsClipping*/
int
sciGetIsClipping (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {

        case SCI_SUBWIN:
            return pSUBWIN_FEATURE (pobj)->isclip;
            break;
        case SCI_ARC:
            return pARC_FEATURE (pobj)->isclip;
            break;
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE (pobj)->isclip;
            break;
        case SCI_RECTANGLE:
            return pRECTANGLE_FEATURE (pobj)->isclip;
            break;
        case SCI_SEGS:
            return pSEGS_FEATURE (pobj)->isclip;
            break;
        case SCI_TEXT:
            return pTEXT_FEATURE (pobj)->isclip;
            break;
        case SCI_AXES:
            return pAXES_FEATURE (pobj)->isclip;
            break;
        case SCI_SURFACE:
            return pSURFACE_FEATURE(pobj)->isclip;
            break;
        case SCI_FEC:
            return pFEC_FEATURE(pobj)->isclip;
            break;
        case SCI_GRAYPLOT:
            return pGRAYPLOT_FEATURE(pobj)->isclip;
            break;
        case SCI_LEGEND:
            return pLEGEND_FEATURE(pobj)->isclip;
        case SCI_AGREG:
        case SCI_FIGURE:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        case SCI_UIMENU:
        default:
            return -2;
            break;
    }
    return -2;
}


/**sciGetClipping
* Gets the clipping area
* 29/11/2002
*/
double *
sciGetClipping (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {

        case SCI_SUBWIN:
            return pSUBWIN_FEATURE (pobj)->clip_region;
            break;
        case SCI_ARC:
            return pARC_FEATURE (pobj)->clip_region;
            break;
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE (pobj)->clip_region;
            break;
        case SCI_RECTANGLE:
            return pRECTANGLE_FEATURE (pobj)->clip_region;
            break;
        case SCI_SEGS:
            return pSEGS_FEATURE (pobj)->clip_region;
            break;
        case SCI_TEXT:
            return pTEXT_FEATURE (pobj)->clip_region;
            break;
        case SCI_AXES:
            return pAXES_FEATURE (pobj)->clip_region;
            break;
        case SCI_LABEL:
            return sciGetClipping( pLABEL_FEATURE(pobj)->text ) ;
            break;
        case SCI_LEGEND:
            return pLEGEND_FEATURE (pobj)->clip_region;
            break;
        case SCI_SURFACE:
            return pSURFACE_FEATURE (pobj)->clip_region;
            break;
        case SCI_FEC:
            return pFEC_FEATURE (pobj)->clip_region;
            break;
        case SCI_GRAYPLOT:
            return pGRAYPLOT_FEATURE (pobj)->clip_region;
            break;
        case SCI_UIMENU:
        case SCI_AGREG:
        case SCI_FIGURE:
        default:
            printSetGetErrorMessage("clip_box");
            return (double *) NULL;
            break;
    }
    printSetGetErrorMessage("clip_box");
    return (double *) NULL;

}

/**sciGetZooming
* Returns the zooming state of the object is it or not zoomed.
*/
BOOL
sciGetZooming (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            return (sciGetGraphicMode (pobj))->zooming;
            break;
        case SCI_SUBWIN:
            /* the value is inherited by the parent */
            return (sciGetGraphicMode (pobj))->zooming;
            break;
        case SCI_TEXT:
        case SCI_LEGEND:
        case SCI_ARC:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_AGREG:
        case SCI_UIMENU:
        case SCI_LABEL: /* F.Leray 28.05.04 */
        default:
            printSetGetErrorMessage("zoom");
            return FALSE;
            break;
    }
    return FALSE;
}


/**
* To know wether an object will be displayed on the screen or not.
* Because if one of its parent is not visible then it won't be displayed.
*/
BOOL
sciGetRealVisibility (sciPointObj * pobj)
{

    if ( pobj == NULL )
    {
        return FALSE ;
    }

    if ( sciGetEntityType( pobj ) == SCI_FIGURE )
    {
        return sciGetVisibility( pobj ) ;
    }

    if ( !sciGetVisibility( pobj ) )
    {
        return FALSE ;
    }

    return sciGetRealVisibility( sciGetParent( pobj ) ) ;

}

/**sciGetVisibility*/
BOOL
sciGetVisibility (sciPointObj * pobj)
{
    int visible = 0;
    int* piVisible = &visible;
    getGraphicObjectProperty(pobj->UID, __GO_VISIBLE__, jni_bool, (void **)&piVisible);

    return visible;
}

/**sciGetNum
* Returns the number of the Figure.
* @param sciPointObj * pobj: the pointer to the entity
* @return the number of the windows contening this entity
*/
int
sciGetNum (sciPointObj * pobj)
{
    int iFigureId = 0;
    int* piFigureId = &iFigureId;

    getGraphicObjectProperty(pobj->UID, __GO_ID__, jni_int, (void**)&piFigureId);

    if (piFigureId == NULL)
    {
        printSetGetErrorMessage("figure_id");
        return -1;
    }

    return iFigureId;

    /*
     * Deactivated for now.
     * The SUBWIN (Axes) case must be taken into account.
     * To be implemented using the MVC framework.
     */
#if 0
    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            return pFIGURE_FEATURE (pobj)->number;
            break;
        case SCI_SUBWIN:
            return sciGetNum (sciGetParent(pobj));
            break;
        case SCI_AGREG:
        default:
            printSetGetErrorMessage("figure_id");
            return -1;
            break;
    }
#endif
}

/**
* Get the size of the window enclosing a figure object
*/
int sciGetWindowWidth(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_FIGURE:
            if ( pObj == (sciPointObj *)getFigureModel() )
            {
                return pFIGURE_FEATURE(pObj)->pModelData->windowWidth ;
            }
            else
            {
                int size[2] ;
                sciGetJavaWindowSize(pObj, size) ;
                return size[0] ;
            }
            break;
        default:
            printSetGetErrorMessage("figure_size");
            break;
    }
    return -1;
}


int sciGetWindowHeight(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_FIGURE:
            if ( pObj == (sciPointObj *)getFigureModel() )
            {
                return pFIGURE_FEATURE(pObj)->pModelData->windowHeight ;
            }
            else
            {
                int size[2] ;
                sciGetJavaWindowSize(pObj, size) ;
                return size[1] ;
            }
            break;
        default:
            printSetGetErrorMessage("figure_size");
            break;
    }
    return -1;
}


/**sciIsExistingSubWin
* Determines if this SubWindow is an existing one in the current SCI_FIGURE
* in association with the wrect and frect....
* @param WRect: Window rectangle dimension in double,
* @param FRect: window user's scale dimension in double,
* @param logscale: flag for logarithmic window
* @return the pointer to the existing SubWindow, or Null if no one is existing
*/
sciPointObj *
sciIsExistingSubWin (double WRect[4])
{

    // ???
#if 0
    sciPointObj *pparentfigure;
    sciSons *psonstmp;

    double WRectTmp[4];
    int i;


    /* Initialisation de WRectTmp a 0*/
    for (i = 0; i < 4; i++)
    {
        WRectTmp[i] = 0.0;
    }

    pparentfigure = (sciPointObj *)sciGetCurrentFigure();
    if (pparentfigure == NULL)
    {
        return NULL;
    }

    /**  15/03/2002 **/
    if ((WRect[0] == 0.) && (WRect[1] == 0.) && (WRect[2] == 1.) && (WRect[3] == 1.))
    {
        /* return the last subwindow */
        sciSons * curSon = sciGetLastSons(pparentfigure);
        while (sciGetEntityType(curSon->pointobj) != SCI_SUBWIN)
        {
            curSon = curSon->pprev;
        }
        return curSon->pointobj;
    }

    psonstmp = sciGetSons (pparentfigure);
    while (psonstmp != NULL)
    {
        sciPointObj * curObj = psonstmp->pointobj;
        if (sciGetEntityType (curObj) == SCI_SUBWIN)
        {
            WRectTmp[0] = pSUBWIN_FEATURE (curObj)->WRect[0];
            WRectTmp[1] = pSUBWIN_FEATURE (curObj)->WRect[1];
            WRectTmp[2] = pSUBWIN_FEATURE (curObj)->WRect[2];
            WRectTmp[3] = pSUBWIN_FEATURE (curObj)->WRect[3];

            if (   (Abs(WRectTmp[0] - WRect[0]) < 1e-8)
                    && (Abs(WRectTmp[1] - WRect[1]) < 1e-8)
                    && (Abs(WRectTmp[2] - WRect[2]) < 1e-8)
                    && (Abs(WRectTmp[3] - WRect[3]) < 1e-8))
            {
                /* subwin found */
                return curObj;
            }
        }
        psonstmp = psonstmp->pnext;
    }

#endif
    return NULL;
}

/*--------------------------------------------------------------------------*/


/**sciGetPoint
* returns pointer to the points of the entity, and a pointer to the number of points. This function allocates memory for the tab of point, so after using the tab don't forget to free it
*/

/**MAJ pour le 3D DJ.Abdemouche 2003**/
double *sciGetPoint(char * pthis, int *numrow, int *numcol)
{
    char* type = NULL;
    double *tab = NULL;
    int i = 0;

    getGraphicObjectProperty(pthis, __GO_TYPE__, jni_string, (void **)&type);

    /*
     * Object type determined by string comparisons
     * Required as we have no better way to do this for the moment
     */
    if (strcmp(type, __GO_FIGURE__) == 0)
    {
        int* figurePosition = NULL;
        int* axesSize = NULL;

        *numrow = 2;
        *numcol = 2;
        if ((tab = CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
        {
            *numrow = -1;
            *numcol = -1;
            return NULL;
        }

        getGraphicObjectProperty(pthis, __GO_POSITION__, jni_int_vector, (void **)&figurePosition);
        getGraphicObjectProperty(pthis, __GO_AXES_SIZE__, jni_int_vector, (void **)&axesSize);

        tab[0] = (double) figurePosition[0];
        tab[1] = (double) figurePosition[1];
        tab[2] = (double) axesSize[0];
        tab[3] = (double) axesSize[1];

        return tab;
    }
    else if (strcmp(type, __GO_POLYLINE__) == 0)
    {
        char* parentAxes = NULL;
        double* dataX = NULL;
        double* dataY = NULL;
        double* dataZ = NULL;
        int iTmp = 0;
        int* piTmp = &iTmp;
        int iView = 0;
        int *piView = &iView;

        /*
         * Testing whether data properties exist for this object
         * is currently done only for this property. The type comparison already
         * ensures that this is the case, though doing so is awkward.
         */
        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_NUM_ELEMENTS__, jni_int, (void**)&piTmp);

        if (piTmp == NULL)
        {
            *numrow = -2;
            *numcol = -2;
            return NULL;
        }

        *numrow = iTmp;

        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_Z_COORDINATES_SET__, jni_int, (void**)&piTmp);

        if (iTmp)
        {
            *numcol = 3;
        }
        else
        {
            *numcol = 2;
        }

        if ( (*numrow) * (*numcol) == 0 )
        {
            /* empty data, no warnings */
            *numrow = 0;
            *numcol = 0;
            return NULL;
        }

        getGraphicObjectProperty(pthis, __GO_PARENT_AXES__, jni_string, (void **)&parentAxes);
        getGraphicObjectProperty(parentAxes, __GO_VIEW__, jni_int, (void**)&piView);

        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_X__, jni_double_vector, (void **)&dataX);
        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_Y__, jni_double_vector, (void **)&dataY);

        if (*numcol == 2 && iView)
        {
            *numcol = (*numcol) + 1; /* colonne de 0. a prendre en compte / afficher => numcol+1*/
            if ((tab = CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return NULL;
            }

            for ( i = 0 ; i < *numrow ; i++ )
            {
                tab[i] = dataX[i];
                tab[*numrow + i] = dataY[i];
                tab[(2 * (*numrow)) + i] = 0.;
            }
        }
        else
        {
            if ((tab = CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
            {
                *numrow = -1;
                *numcol = -1;
                return NULL;
            }

            if (*numcol == 3)
            {
                getGraphicObjectProperty(pthis, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&dataZ);
            }

            for ( i = 0 ; i < *numrow ; i++ )
            {
                tab[i] = dataX[i];
                tab[*numrow + i] = dataY[i];
                if (*numcol == 3)
                {
                    tab[(2 * (*numrow)) + i] = dataZ[i];
                }
            }

        }
        return tab;
    }
    else if (strcmp(type, __GO_RECTANGLE__) == 0)
    {
        char* parentAxes = NULL;
        double* upperLeftPoint = NULL;
        double width = 0.0;
        double *pdblWidth = &width;
        double height = 0.0;
        double *pdblHeight = &height;
        int iView = 0;
        int *piView = &iView;

        *numrow = 1;

        getGraphicObjectProperty(pthis, __GO_PARENT_AXES__, jni_string, (void **)&parentAxes);
        getGraphicObjectProperty(parentAxes, __GO_VIEW__, jni_int, (void**)&piView);

        *numcol = iView ? 5 : 4;

        if ((tab = CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
        {
            *numrow = -1;
            *numcol = -1;
            return NULL;
        }

        getGraphicObjectProperty(pthis, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&upperLeftPoint);

        getGraphicObjectProperty(pthis, __GO_WIDTH__, jni_double, (void** )&pdblWidth);
        getGraphicObjectProperty(pthis, __GO_HEIGHT__, jni_double, (void **)&pdblHeight);

        tab[0] = upperLeftPoint[0];
        tab[1] = upperLeftPoint[1];

        if (iView)
        {
            tab[2] = upperLeftPoint[2];
            tab[3] = width;
            tab[4] = height;
        }
        else
        {
            tab[2] = width;
            tab[3] = height;
        }
        return (double*)tab;
    }
    else if (strcmp(type, __GO_ARC__) == 0)
    {
        char* parentAxes = NULL;
        double* upperLeftPoint = NULL;
        double width = 0.0;
        double *pdblWidth = &width;

        double height = 0.;
        double *pdblHeight = &height;

        double startAngle = 0.;
        double *pdblStartAngle = &startAngle;
        double endAngle = 0.;
        double *pdblEndAngle = &endAngle;
        int view = 0;
        int *piView = &view;

        *numrow = 1;

        getGraphicObjectProperty(pthis, __GO_PARENT_AXES__, jni_string, (void **)&parentAxes);
        getGraphicObjectProperty(parentAxes, __GO_VIEW__, jni_int, (void**)&piView);

        *numcol = view ? 7 : 6;

        if ((tab = CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
        {
            *numrow = -1;
            *numcol = -1;
            return NULL;
        }

        getGraphicObjectProperty(pthis, __GO_UPPER_LEFT_POINT__, jni_double_vector, (void **)&upperLeftPoint);

        getGraphicObjectProperty(pthis, __GO_WIDTH__, jni_double, (void **)&pdblWidth);
        getGraphicObjectProperty(pthis, __GO_HEIGHT__, jni_double, (void **)&pdblHeight);

        getGraphicObjectProperty(pthis, __GO_START_ANGLE__, jni_double, (void **)&pdblStartAngle);
        getGraphicObjectProperty(pthis, __GO_END_ANGLE__, jni_double, (void **)&pdblEndAngle);

        tab[0] = upperLeftPoint[0];
        tab[1] = upperLeftPoint[1];

        if (view)
        {
            tab[2] = upperLeftPoint[2];
            tab[3] = width;
            tab[4] = height;
            tab[5] = RAD2DEG(startAngle);
            tab[6] = RAD2DEG(endAngle);
        }
        else
        {
            tab[2] = width;
            tab[3] = height;
            tab[4] = RAD2DEG(startAngle);
            tab[5] = RAD2DEG(endAngle);
        }

        return (double*)tab;
    }
    else if (strcmp(type, __GO_COMPOUND__) == 0)
    {
        return (double*)NULL;
    }
    else if (strcmp(type, __GO_TEXT__) == 0)
    {
        char* parentAxes = NULL;
        double* textPosition = NULL;
        int iView = 0;
        int* piView = &iView;

        *numrow = 1;

        getGraphicObjectProperty(pthis, __GO_PARENT_AXES__, jni_string, (void **)&parentAxes);
        getGraphicObjectProperty(parentAxes, __GO_VIEW__, jni_int, (void**)&piView);

        *numcol = iView ? 3 : 2;

        if ((tab = CALLOC((*numrow) * (*numcol), sizeof(double))) == NULL)
        {
            *numrow = -1;
            *numcol = -1;
            return NULL;
        }

        getGraphicObjectProperty(pthis, __GO_POSITION__, jni_double_vector, (void **)&textPosition);

        tab[0] = textPosition[0];
        tab[1] = textPosition[1];

        if (iView)
        {
            tab[2] = textPosition[2];
        }

        return (double*)tab;
    }
    else if (strcmp(type, __GO_SEGS__) == 0)
    {
        int iView = 0;
        int* piView = &iView;
        int iNumArrows = 0;
        int* piNumArrows = &iNumArrows;
        char* parentAxes = NULL;
        double* arrowBases = NULL;
        double* arrowDirections = NULL;

        getGraphicObjectProperty(pthis, __GO_NUMBER_ARROWS__, jni_int, (void**)&piNumArrows);
        *numrow = iNumArrows;

        /* only two coordinates are displayed if the axe is in 2d
        and the z coordinates has never been modified */

        getGraphicObjectProperty(pthis, __GO_PARENT_AXES__, jni_string, (void **)&parentAxes);

        getGraphicObjectProperty(parentAxes, __GO_VIEW__, jni_int, (void**)&piView);

        if (iView == 0) // 2-D
        {
            *numcol = 2;
        }
        else // 3-D
        {
            *numcol = 3;
        }

        if ((tab = CALLOC(2 * (*numrow) * (*numcol), sizeof(double))) == NULL)
        {
            *numrow = -1;
            *numcol = -1;
            return NULL;
        }

        getGraphicObjectProperty(pthis, __GO_BASE__, jni_double_vector, (void **)&arrowBases);
        getGraphicObjectProperty(pthis, __GO_DIRECTION__, jni_double_vector, (void **)&arrowDirections);

        for (i = 0; i < *numrow; i++)
        {
            tab[2 * i] = arrowBases[3 * i];
            tab[2 * i + 1] = arrowDirections[3 * i];
            tab[2 * (*numrow) + 2 * i] = arrowBases[3 * i + 1];
            tab[2 * (*numrow) + 2 * i + 1] = arrowDirections[3 * i + 1];
            if (*numcol == 3)
            {
                tab[4 * (*numrow) + 2 * i] = arrowBases[3 * i + 2];
                tab[4 * (*numrow) + 2 * i + 1] = arrowDirections[3 * i + 2];
            }
        }

        /* There are twice as many points as arrows (2 endpoints) */
        *numrow = 2 * (*numrow);

        return (double*)tab;
    }
    else if (strcmp(type, __GO_FAC3D__) == 0)
    {
        *numrow = -1;
        *numcol = -1;
        return (double*) NULL;
    }
    else if (strcmp(type, __GO_PLOT3D__) == 0)
    {
        *numrow = -1;
        *numcol = -1;
        return (double*) NULL;
    }
    else if (strcmp(type, __GO_MATPLOT__) == 0)
    {
        int nx = 0;
        int *piNx = &nx;
        int ny = 0;
        int *piNy = &ny;
        double* data;

        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_NUM_X__, jni_int, (void**)&piNx);

        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_NUM_Y__, jni_int, (void**)&piNy);

        /* The z data matrix has (ny-1) rows and (nx-1) cols */
        nx = nx - 1;
        ny = ny - 1;

        *numrow = ny;
        *numcol = nx;

        if ((tab = CALLOC(nx * ny, sizeof(double))) == NULL)
        {
            *numrow = -1;
            *numcol = -1;
            return (double*)NULL;
        }

        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_Z__, jni_double_vector, (void **)&data);

        for (i = 0; i < nx * ny; i++)
        {
            tab[i] = data[i];
        }

        return (double*)tab;
    }
    else if (strcmp(type, __GO_FEC__) == 0)
    {
        double* coordinates = NULL;
        double* values = NULL;
        int iTmp = 0;
        int* piTmp = &iTmp;

        *numcol = 3;

        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_NUM_VERTICES__, jni_int, (void**)&piTmp);
        *numrow = iTmp;

        if ((tab = CALLOC(*numrow * 3, sizeof(double))) == NULL)
        {
            *numrow = -1;
            *numcol = -1;
            return (double*)NULL;
        }

        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_COORDINATES__, jni_double_vector, (void **)&coordinates);
        getGraphicObjectProperty(pthis, __GO_DATA_MODEL_VALUES__, jni_double_vector, (void **)&values);

        for (i = 0; i < *numrow; i++)
        {
            tab[i] = coordinates[3 * i];
            tab[*numrow + i] = coordinates[3 * i + 1];
            tab[*numrow * 2 + i] = values[i];

        }

        return (double*)tab;
    }
    else if (strcmp(type, __GO_LEGEND__) == 0)
    {
        *numrow = -2;
        *numcol = -2;
        return (double*)NULL;
    }
    else if (strcmp(type, __GO_AXES__) == 0)
    {
        *numrow = -2;
        *numcol = -2;
        return (double*)NULL;
    }
    /* F.Leray 28.05.04 */
    else if (strcmp(type, __GO_LABEL__) == 0)
    {
        *numrow = -2;
        *numcol = -2;
        return (double*)NULL;
    }
    else
    {
        *numrow = -2;
        *numcol = -2;
        return (double*)NULL;
    }

    /*
     * Deactivated for now
     * Same as the else condition
     */
#if 0
case SCI_UIMENU:
#endif
    return (double*)NULL;
}

/**sciGetPointerToToUserData
* Returns the pointer to the user_data and size_of_user_data fields associated with
the pobj object
*/
void sciGetPointerToUserData (sciPointObj * pobj, int ***user_data_ptr, int **size_ptr)
{

    /* perhaps is it necessary to cast the returns value with void* type ! */
    switch (sciGetEntityType (pobj))
    {
        case SCI_FIGURE:
            *user_data_ptr = &(((sciFigure *) pFIGURE_FEATURE (pobj))->user_data);
            *size_ptr = &(((sciFigure *) pFIGURE_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_SUBWIN:
            *user_data_ptr = &(((sciSubWindow *) pSUBWIN_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciSubWindow *) pSUBWIN_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_TEXT:
            *user_data_ptr = &(((sciText *) pTEXT_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciText *) pTEXT_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_LEGEND:
            *user_data_ptr = &(((sciLegend *) pLEGEND_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciLegend *) pLEGEND_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_ARC:
            *user_data_ptr = &(((sciArc *) pARC_FEATURE (pobj))->user_data);
            *size_ptr = &(((sciArc *) pARC_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_POLYLINE:
            *user_data_ptr = &(((sciPolyline *) pPOLYLINE_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciPolyline *) pPOLYLINE_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_SEGS:
            *user_data_ptr = &(((sciSegs *) pSEGS_FEATURE (pobj))->user_data);
            *size_ptr = &(((sciSegs *) pSEGS_FEATURE (pobj))->size_of_user_data );
            break;
        case SCI_FEC:
            *user_data_ptr = &(((sciFec *) pFEC_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciFec *) pFEC_FEATURE (pobj))->size_of_user_data) ;
            break;
        case SCI_GRAYPLOT:
            *user_data_ptr = &(((sciGrayplot *) pGRAYPLOT_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciGrayplot *) pGRAYPLOT_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_RECTANGLE:
            *user_data_ptr = &(((sciRectangle *) pRECTANGLE_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciRectangle *) pRECTANGLE_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_SURFACE:
            *user_data_ptr = &(((sciSurface *) pSURFACE_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciSurface *) pSURFACE_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_AXES:
            *user_data_ptr = &(((sciAxes *) pAXES_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciAxes *) pAXES_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_AGREG:
            *user_data_ptr = &(((sciAgreg *) pAGREG_FEATURE (pobj))->user_data);
            *size_ptr =  &(((sciAgreg *) pAGREG_FEATURE (pobj))->size_of_user_data);
            break;
        case SCI_LABEL:
            sciGetPointerToUserData ( pLABEL_FEATURE(pobj)->text, user_data_ptr, size_ptr ) ;
            break;
        case SCI_UIMENU:
            *user_data_ptr = &(pUIMENU_FEATURE (pobj)->user_data);
            *size_ptr = &(pUIMENU_FEATURE (pobj)->size_of_user_data);
            break;
        case SCI_UICONTROL:
            *user_data_ptr = &(pUICONTROL_FEATURE (pobj)->user_data);
            *size_ptr = &(pUICONTROL_FEATURE (pobj)->size_of_user_data);
            break;
        default:
            *user_data_ptr = (int **)NULL;
            *size_ptr =   (int *)NULL;
            break;
    }
}


/*-------------------------------------------------------------------------------*/
/**
* Return the first surface found within the descendant of the object.
* @param[in] psubwin Object from which the surface will be searched.
* @return The first surface object if found, NULL otherwise.
*/
sciPointObj * sciGetSurface( sciPointObj * pObj )
{
    sciSons * psonstmp = NULL;
    sciPointObj * sonSurface = NULL ;

    psonstmp = sciGetSons( pObj ) ;
    while ( psonstmp != NULL )
    {
        if ( sciGetEntityType( psonstmp->pointobj ) == SCI_SURFACE )
        {
            /* We found one, return it.*/
            return psonstmp->pointobj ;
        }
        /* check the sons of this children */
        sonSurface = sciGetSurface( psonstmp->pointobj ) ;
        if ( sonSurface != NULL )
        {
            return sonSurface ;
        }
        psonstmp = psonstmp->pnext;
    }
    /* nothing has been found */
    return NULL;
}

/**sciGetOriginalSubWin
* PRIVATE.
* @param sciPointObj * pfigure: the pointer to the parent figure
* @return the pointer of the original subwindow
*/
sciPointObj *
sciGetOriginalSubWin (sciPointObj * pfigure)
{
    return (sciPointObj *)pFIGURE_FEATURE(pfigure)->originalsubwin0011;
}


/* get the property of the axes box */
EAxesBoxType sciGetBoxType( sciPointObj * pobj )
{
    switch (sciGetEntityType (pobj))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pobj)->axes.rect ;
        default:
            printSetGetErrorMessage("box_type");
            return (EAxesBoxType)0 ;
    }
    return (EAxesBoxType)0;
}


/**sciGetIsBoxed
* Returns the box existence
*/
BOOL
sciGetIsBoxed (sciPointObj * pobj)
{
    switch (sciGetEntityType (pobj))
    {
        case SCI_TEXT:
            return pTEXT_FEATURE(pobj)->isboxed;
            break;
        case SCI_SUBWIN:
            return ( pSUBWIN_FEATURE(pobj)->axes.rect == BT_ON || pSUBWIN_FEATURE(pobj)->axes.rect == BT_BACK_HALF ) ;
        case SCI_LABEL:
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_ARC:
        case SCI_FIGURE:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_LEGEND:
        case SCI_SEGS:
        case SCI_FEC:
        case SCI_GRAYPLOT:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            printSetGetErrorMessage("box_type");
            return 0;
            break;
    }
    return 0;
}

int * sciGetInterpVector(sciPointObj * pobj)
{

    if (sciGetEntityType(pobj) != SCI_POLYLINE)
        return (int *) NULL;

    return pPOLYLINE_FEATURE(pobj)->scvector;
}

BOOL sciGetIsColorInterpolated(sciPointObj * pobj)
{
    switch ( sciGetEntityType (pobj) )
    {
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE(pobj)->isinterpshaded;
        default:
            printSetGetErrorMessage("interp_vector");
            break;
    }
    return FALSE;
}

BOOL sciGetAutoRotation ( sciPointObj * pObj )
{
    switch (sciGetEntityType (pObj))
    {
        case SCI_LABEL:
            return pLABEL_FEATURE(pObj)->auto_rotation ;
            break;
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_ARC:
        case SCI_TEXT:
        case SCI_FIGURE:
        case SCI_SUBWIN:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_FEC:
        case SCI_SEGS:
        case SCI_LEGEND:
        case SCI_GRAYPLOT:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            return FALSE;
            break;
    }
    return FALSE;
}

BOOL sciGetAutoPosition ( sciPointObj * pObj )
{
    switch (sciGetEntityType (pObj))
    {
        case SCI_LABEL:
            return pLABEL_FEATURE(pObj)->auto_position ;
            break;
        case SCI_POLYLINE:
        case SCI_RECTANGLE:
        case SCI_ARC:
        case SCI_TEXT:
        case SCI_FIGURE:
        case SCI_SUBWIN:
        case SCI_SURFACE:
        case SCI_AXES:
        case SCI_FEC:
        case SCI_SEGS:
        case SCI_LEGEND:
        case SCI_GRAYPLOT:
        case SCI_AGREG:
        case SCI_UIMENU:
        default:
            return FALSE;
            break;
    }
    return FALSE;
}

/*-----------------------------------------------------------------------------------*/
BOOL sciGetLegendDefined(char * pObjUID)
{
    char* xLabelId = NULL;
    char* yLabelId = NULL;
    char* zLabelId = NULL;

    if (pObjUID == NULL)
    {
        return FALSE;
    }

    getGraphicObjectProperty(pObjUID, __GO_X_AXIS_LABEL__, jni_string, (void **)&xLabelId);
    getGraphicObjectProperty(pObjUID, __GO_Y_AXIS_LABEL__, jni_string, (void **)&yLabelId);
    getGraphicObjectProperty(pObjUID, __GO_Z_AXIS_LABEL__, jni_string, (void **)&zLabelId);

    /* get the text size of labels */
    if (sciisTextEmpty(xLabelId) &&
            sciisTextEmpty(yLabelId) &&
            sciisTextEmpty(zLabelId))
    {
        return FALSE;
    }
    else
    {
        return TRUE;
    }
}
/*-----------------------------------------------------------------------------------*/
BOOL sciGetAutoSize( sciPointObj * pObj )
{
    switch (sciGetEntityType (pObj))
    {
        case SCI_TEXT:
            return pTEXT_FEATURE(pObj)->autoSize ;
            break;
        default:
            return FALSE;
            break;
    }
    return FALSE;
}
/*-----------------------------------------------------------------------------------*/
sciTextAlignment sciGetAlignment( sciPointObj * pObj )
{
    switch (sciGetEntityType (pObj))
    {
        case SCI_TEXT:
            return pTEXT_FEATURE(pObj)->stringsAlign ;
            break;
        default:
            printSetGetErrorMessage("text_alignment");
            return ALIGN_NONE ;
    }
    return ALIGN_NONE ;
}
/*-----------------------------------------------------------------------------------*/
void sciGetUserSize( sciPointObj * pObj, double * width, double * height )
{
    switch ( sciGetEntityType( pObj ) )
    {
        case SCI_TEXT:
            *width  = pTEXT_FEATURE(pObj)->userSize[0] ;
            *height = pTEXT_FEATURE(pObj)->userSize[1] ;
            break ;
        default:
            printSetGetErrorMessage("text_box");
            break ;
    }
}
/*-----------------------------------------------------------------------------------*/
/**
* get how is displayed the text around its position.
* If TRUE, the position is the middle of the displayed string.
* If FALSE, it is the lower right corner.
*/
BOOL sciGetCenterPos( sciPointObj * pObj )
{
    switch ( sciGetEntityType( pObj ) )
    {
        case SCI_TEXT:
            return pTEXT_FEATURE(pObj)->centeredPos ;
            break ;
        default:
            printSetGetErrorMessage("text_box_mode");
            return FALSE ;
    }
}
/*-----------------------------------------------------------------------------------*/
/**
* return wether the current object is displayed in 2d or 3d mode.
*/
BOOL sciGetIs3d( sciPointObj * pObj )
{
    switch ( sciGetEntityType( pObj ) )
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->is3d ;
        case SCI_TEXT:
            return pTEXT_FEATURE( pObj )->is3d ;
        case SCI_LABEL:
            return sciGetIs3d( pLABEL_FEATURE( pObj )->text ) ;
        default:
            printSetGetErrorMessage("view");
            return FALSE ;
    }
    return FALSE ;
}
/*-----------------------------------------------------------------------------------*/
/**
* get the number of children of a graphic object
* @return number of children of object pObj
*/
int sciGetNbChildren( sciPointObj * pObj )
{
    int nbChildren = 0 ;
    sciSons * curSon = sciGetSons( pObj ) ;
    while ( curSon != NULL && curSon->pointobj != NULL )
    {
        nbChildren++ ;
        curSon = curSon->pnext ;
    }
    return nbChildren ;
}
/*-----------------------------------------------------------------------------------*/
/**
* Return if an object directly accessible as a son of an other in Scilab.
* For instance, Label objects are not accessible.
* @return TRUE if the object is accessible, FALSE otherwise.
*/
BOOL sciGetIsAccessibleChild( sciPointObj * pObj )
{
    return sciGetEntityType( pObj ) != SCI_LABEL
           && GetHandleVisibilityOnUimenu( pObj ) ;
}
/*-----------------------------------------------------------------------------------*/
BOOL GetHandleVisibilityOnUimenu( sciPointObj * pobj )
{
    if (sciGetEntityType(pobj) != SCI_UIMENU)
    {
        return TRUE ;
    }

    return pUIMENU_FEATURE(pobj)->handle_visible;
}
/*-----------------------------------------------------------------------------------*/
/**
* return the number of object of a certain type which can be found among the descendants
* of an object.
* To get the number of surfaces of a subwindow, it is much faster to use the
* sciGetSubwinNbSurf funtion.
*/
int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type )
{
    int nbFound = 0 ;
    sciSons * curSon = sciGetSons( pObj ) ;
    while ( curSon != NULL )
    {
        if ( sciGetEntityType( curSon->pointobj ) == type )
        {
            nbFound++ ;
        }
        nbFound += sciGetNbTypedObjects( curSon->pointobj, type ) ;
        curSon = curSon->pnext ;
    }
    return nbFound ;
}
/*----------------------------------------------------------------------------------*/
/**
* return if the drawlater mode is on for the parent figure.
*/
BOOL sciGetIsAutoDrawable( sciPointObj * pobj )
{
    return (sciGetImmediateDrawingMode(pobj) && !sciGetPixmapMode(pobj));
}
/*----------------------------------------------------------------------------------*/
BOOL sciGetImmediateDrawingMode(sciPointObj * pobj)
{
    return pFIGURE_FEATURE(sciGetParentFigure(pobj))->auto_redraw ;
}
/*----------------------------------------------------------------------------------*/
/**
* To retrieve the viewport property
* viewport property is only enable when the auto_resize property of the figure is
* disable.
*/
void sciGetViewport( sciPointObj * pObj, int viewport[4] )
{
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_FIGURE:
            if (isFigureModel(pObj->UID))
            {
                viewport[0] = pFIGURE_FEATURE(pObj)->pModelData->viewport[0];
                viewport[1] = pFIGURE_FEATURE(pObj)->pModelData->viewport[1];
                viewport[2] = pFIGURE_FEATURE(pObj)->pModelData->viewport[2];
                viewport[3] = pFIGURE_FEATURE(pObj)->pModelData->viewport[3];
            }
            else
            {
                sciGetJavaViewport(pObj, viewport);
            }
            break;
        default:
            printSetGetErrorMessage("viewport");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
void sciGetScreenPosition( sciPointObj * pObj, int * posX, int * posY )
{
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_FIGURE:
            if ( pObj == (sciPointObj *)getFigureModel() )
            {
                *posX = pFIGURE_FEATURE(pObj)->pModelData->windowPosition[0] ;
                *posY = pFIGURE_FEATURE(pObj)->pModelData->windowPosition[1] ;
            }
            else
            {
                int pos[2];
                sciGetJavaWindowPosition(pObj, pos) ;
                *posX = pos[0] ;
                *posY = pos[1] ;
            }
            break ;
        default:
            printSetGetErrorMessage("figure_position");
            *posX = -1 ;
            *posY = -1 ;
    }
}
/*----------------------------------------------------------------------------------*/
BOOL sciGetIsEventHandlerEnable( sciPointObj * pObj )
{
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_FIGURE:
        {
            return pFIGURE_FEATURE( pObj )->isEventHandlerEnable ;
        }
        break ;
        default:
            printSetGetErrorMessage("event_handler");
            return FALSE ;
    }
    return FALSE ;
}
/*----------------------------------------------------------------------------------*/
char * sciGetEventHandler( sciPointObj * pObj )
{
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_FIGURE:
        {
            return pFIGURE_FEATURE(pObj)->eventHandler ;
        }
        break ;
        default:
            printSetGetErrorMessage("event_handler");
            return NULL ;
    }
    return NULL ;
}
/*----------------------------------------------------------------------------------*/
double * sciGetAxesBounds( sciPointObj * pObj )
{
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->WRect;
        default:
            printSetGetErrorMessage("axes_bounds");
            return NULL ;
    }
}
/*----------------------------------------------------------------------------------*/
double * sciGetMargins( sciPointObj * pObj )
{
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->ARect;
        default:
            printSetGetErrorMessage("margins");
            return NULL ;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* To get the computed data bounds of a subwin.
* * @param bounds [Xmin,Xmax,Ymin,Ymax,Zmin,Zmax] vector.
*/
void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] )
{
    int i = 0;
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_SUBWIN:
            for ( i = 0 ; i < 6 ; i++ )
            {
                bounds[i] = pSUBWIN_FEATURE(pObj)->FRect[i] ;
            }
            return ;
        default:
            printSetGetErrorMessage("data_bounds");
            for ( i = 0 ; i < 6 ; i++ )
            {
                bounds[i] = 0.0 ;
            }
    }
    return ;
}
/*----------------------------------------------------------------------------------*/
/**
* Get the bounds we need to use for a subwin (between user defined one and zoomed ones).
*/
void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6])
{
    if (sciGetZooming(pObj))
    {
        sciGetZoomBox(pObj, bounds);
    }
    else
    {
        sciGetDataBounds(pObj, bounds);
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get data-bounds defined by the user and not modified for pretty print by scilab.
* @param bounds [Xmin,Xmax,Ymain,Ymax,Zmin,Zmax] vector.
*/
void sciGetDataBounds( sciPointObj * pObj, double bounds[6] )
{
    int i = 0;
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_SUBWIN:
            for ( i = 0 ; i < 6 ; i++ )
            {
                bounds[i] = pSUBWIN_FEATURE(pObj)->SRect[i] ;
            }
            return ;
        case SCI_SURFACE:
            for ( i = 0 ; i < 6 ; i++ )
            {
                bounds[i] = pSURFACE_FEATURE(pObj)->ebox[i] ;
            }
            return ;
        default:
            printSetGetErrorMessage("data_bounds");
            for ( i = 0 ; i < 6 ; i++ )
            {
                bounds[i] = 0.0 ;
            }
    }
    return ;
}
/*----------------------------------------------------------------------------------*/
/**
* Get the viewing angles of a subwindow in degrees.
*/
void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta)
{
    switch ( sciGetEntityType(pObj) )
    {
        case SCI_SUBWIN:
            *alpha = pSUBWIN_FEATURE(pObj)->alpha;
            *theta = pSUBWIN_FEATURE(pObj)->theta;
            break;
        default:
            printSetGetErrorMessage("rotation_angles");
            *alpha = 0.0;
            *theta = 0.0;
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Copy the logflags of a subwin to an array of size 3.
*/
void sciGetLogFlags(char * pObjUID, char flags[3])
{
    char* logflagPropertyNames[3] = {__GO_X_AXIS_LOG_FLAG__, __GO_Y_AXIS_LOG_FLAG__, __GO_Z_AXIS_LOG_FLAG__};
    int i = 0;
    int iLogFlag = 0;
    int* piLogFlag = &iLogFlag;

    for (i = 0; i < 3; i++)
    {
        getGraphicObjectProperty(pObjUID, logflagPropertyNames[i], jni_bool, (void **)&piLogFlag);

        if (piLogFlag == NULL)
        {
            printSetGetErrorMessage("log_flags");
            return;
        }

        if (iLogFlag)
        {
            flags[i] = 'l';
        }
        else
        {
            flags[i] = 'n';
        }
    }
}
/*----------------------------------------------------------------------------------*/
/**
* To know if the pixmap mode a figure is on or off.
*/
BOOL sciGetPixmapMode(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_FIGURE:
            return pFIGURE_FEATURE(pObj)->pixmapMode;
            break;
        default:
            printSetGetErrorMessage("pixmap");
            break;
    }
    return FALSE;
}
/*----------------------------------------------------------------------------------*/
/**
* @return number of vertices of a pointObj
*/
int sciGetNbPoints(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE(pObj)->n1;
        default:
            printSetGetErrorMessage("points");
            break;
    }
    return -1;
}
/*----------------------------------------------------------------------------------*/
int sciGetPolylineStyle(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE(pObj)->plot;
        default:
            printSetGetErrorMessage("polyline_style");
            break;
    }
    return -1;
}
/*----------------------------------------------------------------------------------*/
BOOL sciGetIsClosed(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE(pObj)->closed;
        default:
            printSetGetErrorMessage("closed");
            break;
    }
    return FALSE;
}
/*----------------------------------------------------------------------------------*/
double sciGetArrowSize(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_POLYLINE:
            return pPOLYLINE_FEATURE(pObj)->arsize_factor;
        case SCI_SEGS:
            return pSEGS_FEATURE(pObj)->arrowsize;
        default:
            printSetGetErrorMessage("arrow_size");
            break;
    }
    return -1;
}
/*----------------------------------------------------------------------------------*/
void sciGetTextPos(sciPointObj * pObj, double position[3])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_TEXT:
            position[0] = pTEXT_FEATURE(pObj)->x;
            position[1] = pTEXT_FEATURE(pObj)->y;
            position[2] = pTEXT_FEATURE(pObj)->z;
            break;
        case SCI_LABEL:
            sciGetTextPos(pLABEL_FEATURE(pObj)->text, position);
            break;
        default:
            printSetGetErrorMessage("data");
            position[0] = 0.0;
            position[1] = 0.0;
            position[2] = 0.0;
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Convert user coordinates to pixel ones (relative to the viewing canvas).
* @param pObj subwindow handle
* @param userCoord user coordinates
* @param pixCoord result in pixels.
*/
void sciGetPixelCoordinate(sciPointObj * pObj, const double userCoord[3], int pixCoord[2])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            sciGetJavaPixelCoordinates(pObj, userCoord, pixCoord);
            break;
        default:
            Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
            pixCoord[0] = -1;
            pixCoord[1] = -1;
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Convert user coordinates to user cooordinates (2D).
* @param pObjUID subwindow identifier
* @param userCoord3D user coordinates
* @param userCoords2D result in user coordinates in the default 2D plane.
*/
void sciGet2dViewCoordinate(char * pObjUID, const double userCoords3D[3], double userCoords2D[2])
{
    char *type = NULL;

    getGraphicObjectProperty(pObjUID, __GO_TYPE__, jni_string, (void **)&type);

    if (strcmp(type, __GO_AXES__) == 0)
    {
        sciGetJava2dViewCoordinates(pObjUID, userCoords3D, userCoords2D);
    }
    else
    {
        Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
        userCoords2D[0] = 0.0;
        userCoords2D[1] = 0.0;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Convert pixel coordinates to 2D view coordinate
* @param pObjUID subwindow identifier
* @param userCoord pixel coordinates
* @param userCoords2D user coordinates in default 2D plane
*/
void sciGet2dViewCoordFromPixel(char * pObjUID, const int pixelCoords[2], double userCoords2D[2])
{
    char *type = NULL;

    getGraphicObjectProperty(pObjUID, __GO_TYPE__, jni_string, (void **)&type);

    if (strcmp(type, __GO_AXES__) == 0)
    {
        sciGetJava2dViewCoordFromPixel(pObjUID, pixelCoords, userCoords2D);
    }
    else
    {
        Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
        userCoords2D[0] = 0.0;
        userCoords2D[1] = 0.0;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Convert 2d view coordinates to pixel coordinates
* @param pObjUID subwindow identifier
* @param userCoords2D coordinates in the default 2D plane
* @param pixelsCoords pixel coordinates
*/
void sciGet2dViewPixelCoordinates(char * pObjUID, const double userCoords2D[2], int pixelCoords[2])
{
    char *type = NULL;

    getGraphicObjectProperty(pObjUID, __GO_TYPE__, jni_string, (void **)&type);

    if (strcmp(type, __GO_AXES__) == 0)
    {
        /* create a 3d user coord */
        double userCoord3D[3] = {userCoords2D[0], userCoords2D[1], 0.0};
        sciGetJava2dViewPixelCoordinates(pObjUID, userCoord3D, pixelCoords);
    }
    else
    {
        Scierror(999, _("Coordinates modifications are only applicable on axes objects.\n"));
        pixelCoords[0] = -1;
        pixelCoords[1] = -1;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* @return TRUE if pObj is a subwin with isoview mode on.
*/
BOOL sciGetIsIsoView(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->isoview;
        default:
            return FALSE;
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* @return TRUE if pObj is a subwindow with cube scaling mode on.
*/
BOOL sciGetIsCubeScaled(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->cube_scaling;
        default:
            return FALSE;
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get axes reverse property for each axis of a subwin object
*/
void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            axesReverse[0] = pSUBWIN_FEATURE(pObj)->axes.reverse[0];
            axesReverse[1] = pSUBWIN_FEATURE(pObj)->axes.reverse[1];
            axesReverse[2] = pSUBWIN_FEATURE(pObj)->axes.reverse[2];
            break;
        default:
            printSetGetErrorMessage("axes_reverse");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* @return TRUE if subwindow pObj tight_limit mode is on.
*/
BOOL sciGetTightLimitsOn(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->tight_limits;
        default:
            return FALSE;
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get auto_ticks property for each axis
*/
void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            autoTicks[0] = pSUBWIN_FEATURE(pObj)->axes.auto_ticks[0];
            autoTicks[1] = pSUBWIN_FEATURE(pObj)->axes.auto_ticks[1];
            autoTicks[2] = pSUBWIN_FEATURE(pObj)->axes.auto_ticks[2];
            break;
        default:
            autoTicks[0] = FALSE;
            autoTicks[1] = FALSE;
            autoTicks[2] = FALSE;
            printSetGetErrorMessage("auto_ticks");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get auto_ticks property for each axis
*/
BOOL sciGetAutoSubticks(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            return !(pSUBWIN_FEATURE(pObj)->flagNax);
            break;
        default:
            return FALSE;
            printSetGetErrorMessage("auto_subticks");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get nbsubtics for each axis
*/
void sciGetNbSubTics(sciPointObj * pObj, int nbsubtics[3])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            nbsubtics[0] = pSUBWIN_FEATURE(pObj)->axes.nbsubtics[0];
            nbsubtics[1] = pSUBWIN_FEATURE(pObj)->axes.nbsubtics[1];
            nbsubtics[2] = pSUBWIN_FEATURE(pObj)->axes.nbsubtics[2];
            break;
        default:
            nbsubtics[0] = 0;
            nbsubtics[1] = 0;
            nbsubtics[2] = 0;
            printSetGetErrorMessage("nbsubtics");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get the axes visible property for each axis.
*/
void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            axesVisible[0] = pSUBWIN_FEATURE(pObj)->axes.axes_visible[0];
            axesVisible[1] = pSUBWIN_FEATURE(pObj)->axes.axes_visible[1];
            axesVisible[2] = pSUBWIN_FEATURE(pObj)->axes.axes_visible[2];
            break;
        default:
            axesVisible[0] = FALSE;
            axesVisible[1] = FALSE;
            axesVisible[2] = FALSE;
            printSetGetErrorMessage("axes_visible");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get the current zoom box of a subwin object
* @param[out] zoomBox output parameter with thte zoom box
*/
void sciGetZoomBox(sciPointObj * pObj, double zoomBox[6])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            zoomBox[0] = pSUBWIN_FEATURE(pObj)->ZRect[0] ;
            zoomBox[1] = pSUBWIN_FEATURE(pObj)->ZRect[1] ;
            zoomBox[2] = pSUBWIN_FEATURE(pObj)->ZRect[2] ;
            zoomBox[3] = pSUBWIN_FEATURE(pObj)->ZRect[3] ;
            zoomBox[4] = pSUBWIN_FEATURE(pObj)->ZRect[4] ;
            zoomBox[5] = pSUBWIN_FEATURE(pObj)->ZRect[5] ;
            break;
        default:
            printSetGetErrorMessage("zoom_box");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get the 4 corners of the bounding box of a text object in 3D
*/
void sciGetTextBoundingBox(sciPointObj * pObj, double corner1[3], double corner2[3],
                           double corner3[3], double corner4[3])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_TEXT:
        {
            int i;
            for (i = 0; i < 3; i++)
            {
                // should be up to date
                corner1[i] = pTEXT_FEATURE(pObj)->corners[0][i];
                corner2[i] = pTEXT_FEATURE(pObj)->corners[1][i];
                corner3[i] = pTEXT_FEATURE(pObj)->corners[2][i];
                corner4[i] = pTEXT_FEATURE(pObj)->corners[3][i];
            }
        }
        break;
        case SCI_LABEL:
            sciGetTextBoundingBox(pLABEL_FEATURE(pObj)->text, corner1, corner2, corner3, corner4);
            break;

        default:
            printSetGetErrorMessage("bounding box");
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get the 4 corners of the bounding box of a text object in pixels
*/
void sciGetPixelBoundingBox(sciPointObj * pObj, int corner1[2], int corner2[2],
                            int corner3[2], int corner4[2])
{
    sciGetJavaPixelBoundingBox(pObj, corner1, corner2, corner3, corner4);
}
/*----------------------------------------------------------------------------------*/
/**
* Get the 4 corners of the boundng box of a text object in 2D view user coordinates.
*/
void sciGet2dViewBoundingBox(sciPointObj * pObj, double corner1[2], double corner2[2],
                             double corner3[2], double corner4[2])
{
    sciPointObj * parentSubwin = sciGetParentSubwin(pObj);
    double corners3d[4][3];

    /* get bounding box */
    sciGetTextBoundingBox(pObj, corners3d[0], corners3d[1], corners3d[2], corners3d[3]);

    /* convert it to 2d view coordinates */
    sciGetJava2dViewCoordinates((char*)parentSubwin, corners3d[0], corner1);
    sciGetJava2dViewCoordinates((char*)parentSubwin, corners3d[1], corner2);
    sciGetJava2dViewCoordinates((char*)parentSubwin, corners3d[2], corner3);
    sciGetJava2dViewCoordinates((char*)parentSubwin, corners3d[3], corner4);

}
/*----------------------------------------------------------------------------------*/
/**
* Get the viewing area of a subwindow acoording to its axes scale and margins
* result is in pixels
*/
void sciGetViewingArea(char * pObjUID, int * xPos, int * yPos, int * width, int * height)
{
    char *type = NULL;

    getGraphicObjectProperty(pObjUID, __GO_TYPE__, jni_string, (void **)&type);

    if (strcmp(type, __GO_AXES__) == 0)
    {
        sciGetJavaViewingArea(pObjUID, xPos, yPos, width, height);
    }
    else
    {
        *xPos = -1;
        *yPos = -1;
        *width = -1;
        *height = -1;
        Scierror(999, _("Only axes handles have a viewing area."));
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get the axis aligned bounding box a graphic object
* @param bounds [xmin, xmax, ymin, ymax, zmin, zmax] bounds.
*/
void sciGetAABoundingBox(sciPointObj * pObj, double bounds[6])
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            sciGetDataBounds(pObj, bounds);
            break;
        case SCI_SEGS:
            sciGetJavaSegsBoundingBox(pObj, bounds);
            break;
        case SCI_TEXT:
        {
            double corners[4][3];
            sciGetTextBoundingBox(pObj, corners[0], corners[1], corners[2], corners[3]);
            bounds[0] = Min(corners[0][0], Min(corners[1][0], Min(corners[2][0], corners[3][0])));
            bounds[1] = Max(corners[0][0], Max(corners[1][0], Max(corners[2][0], corners[3][0])));
            bounds[2] = Min(corners[0][1], Min(corners[1][1], Min(corners[2][1], corners[3][1])));
            bounds[3] = Max(corners[0][1], Max(corners[1][1], Max(corners[2][1], corners[3][1])));
            bounds[4] = Min(corners[0][2], Min(corners[1][2], Min(corners[2][2], corners[3][2])));
            bounds[5] = Max(corners[0][2], Max(corners[1][2], Max(corners[2][2], corners[3][2])));
        }
        break;
        case SCI_LABEL:
            sciGetAABoundingBox(pLABEL_FEATURE(pObj)->text, bounds);
            break;
        default:
            Scierror(999, _("Unable to compute data bounds for this kind of object."));
            break;
    }
}
/*----------------------------------------------------------------------------------*/
/**
* Get the x_position value
* @param bounds [xmin, xmax, ymin, ymax, zmin, zmax] bounds.
*/
char sciGetxLocation(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->axes.xdir;
            break;
        default:
            Scierror(999, _("Unable to compute x_location for this kind of object."));
            break;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------*/
char sciGetyLocation(sciPointObj * pObj)
{
    switch (sciGetEntityType(pObj))
    {
        case SCI_SUBWIN:
            return pSUBWIN_FEATURE(pObj)->axes.ydir;
            break;
        default:
            Scierror(999, _("Unable to compute y_location for this kind of object."));
            break;
    }
    return NULL;
}
/*----------------------------------------------------------------------------------*/
/**
* Get the labels and positions of ticks along Z axis.
* String composing ticksLabels are allocated with C++ new.
*/
BOOL sciGetIsAbleToCreateWindow(void)
{
    return sciGetJavaIsAbleToCreateWindow();
}
/*----------------------------------------------------------------------------------*/
/**
* @return index of the subwin between all the subwins lying below its parent figure
*/
int sciGetSubwinIndex(sciPointObj * pSubwin)
{
    sciPointObj * parentFigure = sciGetParentFigure(pSubwin);
    int subwinIndex = 0;
    sciSons * pSons = sciGetSons(parentFigure);

    while (pSons->pointobj != pSubwin)
    {
        if (sciGetEntityType(pSons->pointobj) == SCI_SUBWIN)
        {
            // subwin found
            subwinIndex++;
        }
        pSons = pSons->pnext;
    }
    return subwinIndex;
}
/*----------------------------------------------------------------------------------*/
/**
* @return TRUE if the object is automatically redraw and does not
*         need to be explicitely drawn (using sciDrawObj).
*/
BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj)
{
    sciEntityType entityType = sciGetEntityType(pObj);
    return (entityType == SCI_UICONTROL)
           || (entityType == SCI_UIMENU)
           || (entityType == SCI_UICONTEXTMENU)
           || (entityType == SCI_WAITBAR)
           || (entityType == SCI_PROGRESSIONBAR);
}
/*----------------------------------------------------------------------------------*/
/**
* Print the message "This object has no xxx property." in Scilab.
*/
void printSetGetErrorMessage(const char * propertyName)
{
    Scierror(999, _("This object has no %s property.\n"), propertyName );
}
/*----------------------------------------------------------------------------------*/

