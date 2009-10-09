/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
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



#ifndef __SCI_GET_PROPERTY__
#define __SCI_GET_PROPERTY__

#include "dynlib_graphics.h"
#include "ObjectStructure.h"
#include "StringMatrix.h"
#include "BOOL.h"

GRAPHICS_IMPEXP void *sciGetPointerToFeature (sciPointObj * pobj);
GRAPHICS_IMPEXP void sciGetPointerToUserData (sciPointObj * pobj,int ***user_data_ptr, int **size_ptr);
GRAPHICS_IMPEXP sciEntityType sciGetEntityType (sciPointObj * pobj); 
GRAPHICS_IMPEXP char *sciGetCharEntityType (sciPointObj * pobj); 

/* Graphic Context Functions */
GRAPHICS_IMPEXP sciGraphicContext *sciGetGraphicContext (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetNumColors (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetColormap (sciPointObj * pobj, double rgbmat[] );
GRAPHICS_IMPEXP int sciGetGoodIndex(sciPointObj * pobj, int colorindex); 

GRAPHICS_IMPEXP int sciGetForegroundToDisplay (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetBackgroundToDisplay (sciPointObj * pobj);

GRAPHICS_IMPEXP int sciGetBackground (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetForeground (sciPointObj * pobj);
GRAPHICS_IMPEXP double sciGetLineWidth (sciPointObj * pobj);
GRAPHICS_IMPEXP BOOL sciGetIsLine (sciPointObj * pobj);
GRAPHICS_IMPEXP BOOL sciGetIsDisplayingLines(sciPointObj * pObj);
GRAPHICS_IMPEXP int sciGetLineStyle (sciPointObj * pobj);
GRAPHICS_IMPEXP BOOL sciGetIsMark (sciPointObj * pobj);
GRAPHICS_IMPEXP BOOL sciGetIsFilled (sciPointObj * pobj);
GRAPHICS_IMPEXP double sciGetArrowSize (sciPointObj * pobj);

GRAPHICS_IMPEXP EAxesBoxType sciGetBoxType( sciPointObj * pobj ) ;
GRAPHICS_IMPEXP BOOL sciGetIsBoxed (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetMarkStyle (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetMarkSize (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetMarkSizeUnit (sciPointObj * pobj);

/* Text Functions */
GRAPHICS_IMPEXP sciFont *sciGetFontContext (sciPointObj * pobj); 
GRAPHICS_IMPEXP double sciGetFontSize(sciPointObj * pobj);
GRAPHICS_IMPEXP double sciGetFontOrientation (sciPointObj * pobj);
GRAPHICS_IMPEXP StringMatrix * sciGetText (sciPointObj * pobj);
GRAPHICS_IMPEXP void sciGetTextSize( sciPointObj * pobj, int * nbRow, int * nbCol ) ;
GRAPHICS_IMPEXP BOOL sciisTextEmpty( sciPointObj * pobj);
GRAPHICS_IMPEXP unsigned int sciGetTextLength (sciPointObj * pobj);

GRAPHICS_IMPEXP int sciGetFontBackground (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetFontForeground (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetMarkBackground (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetMarkForeground (sciPointObj * pobj);
/* F.Leray 09.04.04*/
GRAPHICS_IMPEXP int sciGetFontBackgroundToDisplay (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetFontForegroundToDisplay (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetMarkBackgroundToDisplay (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetMarkForegroundToDisplay (sciPointObj * pobj);

GRAPHICS_IMPEXP int sciGetFontStyle (sciPointObj * pobj);

/**
* sciGetBackgroundColor function 
* @return the stored value of BackgroundColor index
*/
GRAPHICS_IMPEXP int sciGetBackgroundColor (sciPointObj * pobj);

/**
* sciGetForegroundColor function 
* @return the stored value of ForegroundColor index
*/
GRAPHICS_IMPEXP int sciGetForegroundColor (sciPointObj * pobj);

/**
* sciGetMarkBackgroundColor function 
* @return the stored value of MarkBackgroundColor index
*/
GRAPHICS_IMPEXP int sciGetMarkBackgroundColor (sciPointObj * pobj);

/**
* sciGetMarkForegroundColor function 
* @return the stored value of MarkForegroundColor index
*/
GRAPHICS_IMPEXP int sciGetMarkForegroundColor (sciPointObj * pobj);


/* Legend */
GRAPHICS_IMPEXP sciLegendPlace sciGetLegendPlace (sciPointObj * pobj);
GRAPHICS_IMPEXP void sciGetLegendPos (sciPointObj * pobj , double position[]);

/* Figure / Subwin main functions */
GRAPHICS_IMPEXP sciPointObj *sciGetParentFigure (sciPointObj * pobj);
GRAPHICS_IMPEXP sciPointObj *sciGetParentSubwin (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetNumFigure (sciPointObj * pobj);

/* GMODE */
GRAPHICS_IMPEXP scigMode *sciGetGraphicMode (sciPointObj * pobj); 
GRAPHICS_IMPEXP int sciGetIsClipRegionValuated (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetIsClipping (sciPointObj * pobj);
GRAPHICS_IMPEXP double *sciGetClipping (sciPointObj * pobj);
GRAPHICS_IMPEXP BOOL sciGetAddPlot (sciPointObj * pobj); 
GRAPHICS_IMPEXP BOOL sciGetAutoScale (sciPointObj * pobj); 
GRAPHICS_IMPEXP BOOL sciGetZooming (sciPointObj * pobj); 
GRAPHICS_IMPEXP int sciGetXorMode (sciPointObj * pobj); 
GRAPHICS_IMPEXP BOOL sciGetRealVisibility (sciPointObj * pobj) ;
GRAPHICS_IMPEXP BOOL sciGetVisibility (sciPointObj * pobj); 
GRAPHICS_IMPEXP BOOL sciGetResize (sciPointObj * pobj); 


/* Window Functions */
GRAPHICS_IMPEXP char *sciGetName (sciPointObj * pobj); 
GRAPHICS_IMPEXP int sciGetNum (sciPointObj * pobj); 
GRAPHICS_IMPEXP int sciGetWidth (sciPointObj * pobj);  
GRAPHICS_IMPEXP int sciGetHeight (sciPointObj * pobj); 
GRAPHICS_IMPEXP void sciGetDim( sciPointObj * pobj, int * pWidth, int * pHeight ) ;
GRAPHICS_IMPEXP int sciGetWindowWidth(sciPointObj * pObj) ;
GRAPHICS_IMPEXP int sciGetWindowHeight(sciPointObj * pObj) ;
GRAPHICS_IMPEXP sciPointObj *sciIsExistingSubWin (double *WRect); /* WARNING special case here */


GRAPHICS_IMPEXP double *sciGetPoint (sciPointObj * pthis, int *num, int *numco);

GRAPHICS_IMPEXP BOOL sciGetdrawmode (sciPointObj *pobj);
GRAPHICS_IMPEXP sciPointObj *sciGetAxes (sciPointObj *pparentfigure,sciPointObj *psubwin);


GRAPHICS_IMPEXP int sciType (char *marker, sciPointObj *pobj);

GRAPHICS_IMPEXP sciPointObj *sciGetSurface(sciPointObj *psubwin);

GRAPHICS_IMPEXP char sciGetxLocation(sciPointObj * pObj);
GRAPHICS_IMPEXP char sciGetyLocation(sciPointObj * pObj);

GRAPHICS_IMPEXP int CheckForCompound (long *tabpointobj, int number);

/* UNUSED ? */
GRAPHICS_IMPEXP sciPointObj *sciGetOriginalSubWin (sciPointObj * pfigure);

GRAPHICS_IMPEXP int * sciGetInterpVector(sciPointObj * pobj);
GRAPHICS_IMPEXP BOOL sciGetIsColorInterpolated(sciPointObj * pobj);

GRAPHICS_IMPEXP BOOL sciGetAutoRotation ( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP BOOL sciGetAutoPosition ( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP BOOL sciGetCenterPos( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP BOOL sciGetIs3d( sciPointObj * pObj ) ;

/**
 * for a subwindow object, return if the its labels (x,y,z) are all
 *       empty or not.
 * @param pObj the subwindow.
 */
GRAPHICS_IMPEXP BOOL sciGetLegendDefined( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP BOOL sciGetAutoSize( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP sciTextAlignment sciGetAlignment( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP void sciGetUserSize( sciPointObj * pObj, double * width, double * height ) ;

GRAPHICS_IMPEXP int sciGetNbChildren( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP BOOL sciGetIsAccessibleChild( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP int sciGetNbAccessibleChildren( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP BOOL GetHandleVisibilityOnUimenu( sciPointObj * pobj ) ;

GRAPHICS_IMPEXP int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type ) ;

GRAPHICS_IMPEXP int sciGetHiddenColor( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP int sciGetHiddenAxisColor( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP BOOL sciGetIsAutoDrawable( sciPointObj * pobj ) ;
GRAPHICS_IMPEXP BOOL sciGetImmediateDrawingMode( sciPointObj * pobj ) ;


GRAPHICS_IMPEXP void sciGetGridStyle( sciPointObj * pObj, int * xStyle, int * yStyle, int * zStyle ) ;

GRAPHICS_IMPEXP void sciGetViewport( sciPointObj * pObj, int viewport[4] );

GRAPHICS_IMPEXP void sciGetInfoMessage( sciPointObj * pObj, char * infoMessage ) ;

GRAPHICS_IMPEXP int sciGetInfoMessageLength( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP void sciGetScreenPosition( sciPointObj * pObj, int * posX, int * posY ) ;

GRAPHICS_IMPEXP BOOL sciGetIsEventHandlerEnable( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP char * sciGetEventHandler( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP double * sciGetAxesBounds( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP double * sciGetMargins( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] ) ;
GRAPHICS_IMPEXP void sciGetDataBounds( sciPointObj * pObj, double bounds[6] ) ;
GRAPHICS_IMPEXP void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6]);

GRAPHICS_IMPEXP void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta) ;

GRAPHICS_IMPEXP void sciGetLogFlags(sciPointObj * pObj, char flags[3]);

GRAPHICS_IMPEXP BOOL sciGetPixmapMode(sciPointObj * pObj);

GRAPHICS_IMPEXP int sciGetNbPoints(sciPointObj * pObj);

GRAPHICS_IMPEXP int sciGetPolylineStyle(sciPointObj * pObj);

GRAPHICS_IMPEXP BOOL sciGetIsClosed(sciPointObj * pObj);

GRAPHICS_IMPEXP void sciGetTextPos(sciPointObj * pObj, double position[3]);

GRAPHICS_IMPEXP void sciGetPixelCoordinate(sciPointObj * pObj, const double userCoord[3], int pixCoord[2]);

GRAPHICS_IMPEXP void sciGet2dViewCoordinate(sciPointObj * pObj, const double userCoords3D[3], double userCoords2D[2]);

GRAPHICS_IMPEXP void sciGet2dViewCoordFromPixel(sciPointObj * pObj, const int pixelCoords[2], double userCoords2D[2]);

GRAPHICS_IMPEXP void sciGet2dViewPixelCoordinates(sciPointObj * pObj, const double userCoords2D[2], int pixelCoords[2]);

GRAPHICS_IMPEXP BOOL sciGetIsIsoView(sciPointObj * pObj);

GRAPHICS_IMPEXP BOOL sciGetIsCubeScaled(sciPointObj * pObj);

GRAPHICS_IMPEXP void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3]);

GRAPHICS_IMPEXP BOOL sciGetTightLimitsOn(sciPointObj * pObj);

GRAPHICS_IMPEXP void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3]);

GRAPHICS_IMPEXP BOOL sciGetAutoSubticks(sciPointObj * pObj);

GRAPHICS_IMPEXP void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3]);

GRAPHICS_IMPEXP void sciGetZoomBox(sciPointObj * pObj, double zoomBox[6]);

GRAPHICS_IMPEXP void sciGet2dViewBoundingBox(sciPointObj * pObj, double corner1[2], double corner2[2],
                             double corner3[2], double corner4[2]);

GRAPHICS_IMPEXP void sciGetTextBoundingBox(sciPointObj * pObj, double corner1[3], double corner2[3],
                           double corner3[3], double corner4[3]);

GRAPHICS_IMPEXP void sciGetPixelBoundingBox(sciPointObj * pObj, int corner1[2], int corner2[2],
                            int corner3[2], int corner4[2]);

GRAPHICS_IMPEXP void sciGetViewingArea(sciPointObj * pObj, int * xPos, int * yPos, int * width, int * height);

GRAPHICS_IMPEXP void sciGetAABoundingBox(sciPointObj * pObj, double bounds[6]);

GRAPHICS_IMPEXP BOOL sciGetUseNurbs(sciPointObj * pObj);

GRAPHICS_IMPEXP BOOL sciGetIsUsingFractionalMetrics(sciPointObj * pObj);

GRAPHICS_IMPEXP int sciGetNbXTicks(sciPointObj * pSubwin);
GRAPHICS_IMPEXP void sciGetXTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
GRAPHICS_IMPEXP int sciGetNbYTicks(sciPointObj * pSubwin);
GRAPHICS_IMPEXP void sciGetYTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
GRAPHICS_IMPEXP int sciGetNbZTicks(sciPointObj * pSubwin);
GRAPHICS_IMPEXP void sciGetZTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);

GRAPHICS_IMPEXP BOOL sciGetIsAbleToCreateWindow(void);

GRAPHICS_IMPEXP int sciGetSubwinIndex(sciPointObj * pSubwin);

GRAPHICS_IMPEXP BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj);

GRAPHICS_IMPEXP void sciGetColorRange(sciPointObj * pObj, int subset[2]);
GRAPHICS_IMPEXP void sciGetOutsideColor(sciPointObj * pObj, int colors[2]);
GRAPHICS_IMPEXP void sciGetZBounds(sciPointObj * pObj, double bounds[2]);

GRAPHICS_IMPEXP BOOL sciGetGridFront(sciPointObj * pObj);

GRAPHICS_IMPEXP int sciGetAntialiasingQuality(sciPointObj * pObj);

GRAPHICS_IMPEXP sciLegendPlace sciGetLegendLocation(sciPointObj * pObj);

GRAPHICS_IMPEXP void printSetGetErrorMessage(const char * propertyName);

#endif /* __SCI_GET_PROPERTY__ */
