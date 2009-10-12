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

GRAPHICS_IMPEXP void *sciGetPointerToFeature (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP void sciGetPointerToUserData (sciPointObj * pobj,int ***user_data_ptr, int **size_ptr); /* GET */
GRAPHICS_IMPEXP sciEntityType sciGetEntityType (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP char *sciGetCharEntityType (sciPointObj * pobj);  /* GET */

/* Graphic Context Functions */
GRAPHICS_IMPEXP sciGraphicContext *sciGetGraphicContext (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetNumColors (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetColormap (sciPointObj * pobj, double rgbmat[] ); /* GET */
GRAPHICS_IMPEXP int sciGetGoodIndex(sciPointObj * pobj, int colorindex);  /* GET */

GRAPHICS_IMPEXP int sciGetForegroundToDisplay (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetBackgroundToDisplay (sciPointObj * pobj); /* GET */

GRAPHICS_IMPEXP int sciGetBackground (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetForeground (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP double sciGetLineWidth (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsLine (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsDisplayingLines(sciPointObj * pObj);
GRAPHICS_IMPEXP int sciGetLineStyle (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsMark (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsFilled (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP double sciGetArrowSize (sciPointObj * pobj); /* GET */

GRAPHICS_IMPEXP EAxesBoxType sciGetBoxType( sciPointObj * pobj ) ; /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsBoxed (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetMarkStyle (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetMarkSize (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetMarkSizeUnit (sciPointObj * pobj); /* GET */

/* Text Functions */
GRAPHICS_IMPEXP sciFont *sciGetFontContext (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP double sciGetFontSize(sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP double sciGetFontOrientation (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP StringMatrix * sciGetText (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP void sciGetTextSize( sciPointObj * pobj, int * nbRow, int * nbCol ) ; /* GET */
GRAPHICS_IMPEXP BOOL sciisTextEmpty( sciPointObj * pobj);
GRAPHICS_IMPEXP unsigned int sciGetTextLength (sciPointObj * pobj); /* GET */

GRAPHICS_IMPEXP int sciGetFontBackground (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetFontForeground (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetMarkBackground (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetMarkForeground (sciPointObj * pobj); /* GET */
/* F.Leray 09.04.04*/
GRAPHICS_IMPEXP int sciGetFontBackgroundToDisplay (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetFontForegroundToDisplay (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetMarkBackgroundToDisplay (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetMarkForegroundToDisplay (sciPointObj * pobj); /* GET */

GRAPHICS_IMPEXP int sciGetFontStyle (sciPointObj * pobj); /* GET */

/* Legend */
GRAPHICS_IMPEXP sciLegendPlace sciGetLegendPlace (sciPointObj * pobj);
GRAPHICS_IMPEXP void sciGetLegendPos (sciPointObj * pobj , double position[]);

/* Figure / Subwin main functions */
GRAPHICS_IMPEXP sciPointObj *sciGetParentFigure (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP sciPointObj *sciGetParentSubwin (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetNumFigure (sciPointObj * pobj); /* GET */

/* GMODE */
GRAPHICS_IMPEXP scigMode *sciGetGraphicMode (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP int sciGetIsClipRegionValuated (sciPointObj * pobj);
GRAPHICS_IMPEXP int sciGetIsClipping (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP double *sciGetClipping (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetAddPlot (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP BOOL sciGetAutoScale (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP BOOL sciGetZooming (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP int sciGetXorMode (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP BOOL sciGetRealVisibility (sciPointObj * pobj) ; /* GET */
GRAPHICS_IMPEXP BOOL sciGetVisibility (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP BOOL sciGetResize (sciPointObj * pobj);  /* GET */


/* Window Functions */
GRAPHICS_IMPEXP char *sciGetName (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP int sciGetNum (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP int sciGetWidth (sciPointObj * pobj);   /* GET */
GRAPHICS_IMPEXP int sciGetHeight (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP void sciGetDim( sciPointObj * pobj, int * pWidth, int * pHeight ) ; /* GET */
GRAPHICS_IMPEXP int sciGetWindowWidth(sciPointObj * pObj) ; /* GET */
GRAPHICS_IMPEXP int sciGetWindowHeight(sciPointObj * pObj) ; /* GET */
GRAPHICS_IMPEXP sciPointObj *sciIsExistingSubWin (double *WRect); /* GET */ /* WARNING special case here */


GRAPHICS_IMPEXP double *sciGetPoint (sciPointObj * pthis, int *num, int *numco); /* GET */

GRAPHICS_IMPEXP BOOL sciGetdrawmode (sciPointObj *pobj); /* GET */
GRAPHICS_IMPEXP sciPointObj *sciGetAxes (sciPointObj *pparentfigure,sciPointObj *psubwin); /* GET */


GRAPHICS_IMPEXP int sciType (char *marker, sciPointObj *pobj); /* GET */

GRAPHICS_IMPEXP sciPointObj *sciGetSurface(sciPointObj *psubwin); /* GET */

GRAPHICS_IMPEXP char sciGetxLocation(sciPointObj * pObj);
GRAPHICS_IMPEXP char sciGetyLocation(sciPointObj * pObj);


GRAPHICS_IMPEXP int CheckForCompound (long *tabpointobj, int number); /* GET */

/* UNUSED ? */
GRAPHICS_IMPEXP sciPointObj *sciGetOriginalSubWin (sciPointObj * pfigure);

GRAPHICS_IMPEXP int * sciGetInterpVector(sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsColorInterpolated(sciPointObj * pobj); /* GET */

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

GRAPHICS_IMPEXP int sciGetNbChildren( sciPointObj * pObj ) ; /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsAccessibleChild( sciPointObj * pObj ) ; /* GET */

GRAPHICS_IMPEXP int sciGetNbAccessibleChildren( sciPointObj * pObj ) ; /* GET */

GRAPHICS_IMPEXP BOOL GetHandleVisibilityOnUimenu( sciPointObj * pobj ) ; /* GET */

GRAPHICS_IMPEXP int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type ) ;

GRAPHICS_IMPEXP int sciGetHiddenColor( sciPointObj * pObj ) ; /* GET */

GRAPHICS_IMPEXP int sciGetHiddenAxisColor( sciPointObj * pObj ) ; /* GET */

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

GRAPHICS_IMPEXP void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* GET */
GRAPHICS_IMPEXP void sciGetDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* GET */
GRAPHICS_IMPEXP void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6]); /* GET */

GRAPHICS_IMPEXP void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta) ; /* GET */

GRAPHICS_IMPEXP void sciGetLogFlags(sciPointObj * pObj, char flags[3]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetPixmapMode(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP int sciGetNbPoints(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP int sciGetPolylineStyle(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsClosed(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGetTextPos(sciPointObj * pObj, double position[3]); /* GET */

GRAPHICS_IMPEXP void sciGetPixelCoordinate(sciPointObj * pObj, const double userCoord[3], int pixCoord[2]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewCoordinate(sciPointObj * pObj, const double userCoords3D[3], double userCoords2D[2]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewCoordFromPixel(sciPointObj * pObj, const int pixelCoords[2], double userCoords2D[2]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewPixelCoordinates(sciPointObj * pObj, const double userCoords2D[2], int pixelCoords[2]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsIsoView(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsCubeScaled(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetTightLimitsOn(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetAutoSubticks(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3]); /* GET */

GRAPHICS_IMPEXP void sciGetZoomBox(sciPointObj * pObj, double zoomBox[6]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewBoundingBox(sciPointObj * pObj, double corner1[2], double corner2[2],
                             double corner3[2], double corner4[2]); /* GET */

GRAPHICS_IMPEXP void sciGetTextBoundingBox(sciPointObj * pObj, double corner1[3], double corner2[3],
                           double corner3[3], double corner4[3]); /* GET */

GRAPHICS_IMPEXP void sciGetPixelBoundingBox(sciPointObj * pObj, int corner1[2], int corner2[2],
                            int corner3[2], int corner4[2]); /* GET */

GRAPHICS_IMPEXP void sciGetViewingArea(sciPointObj * pObj, int * xPos, int * yPos, int * width, int * height); /* GET */

GRAPHICS_IMPEXP void sciGetAABoundingBox(sciPointObj * pObj, double bounds[6]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetUseNurbs(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsUsingFractionalMetrics(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP int sciGetNbXTicks(sciPointObj * pSubwin);
GRAPHICS_IMPEXP void sciGetXTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
GRAPHICS_IMPEXP int sciGetNbYTicks(sciPointObj * pSubwin);
GRAPHICS_IMPEXP void sciGetYTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
GRAPHICS_IMPEXP int sciGetNbZTicks(sciPointObj * pSubwin);
GRAPHICS_IMPEXP void sciGetZTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);

GRAPHICS_IMPEXP BOOL sciGetIsAbleToCreateWindow(void);

GRAPHICS_IMPEXP int sciGetSubwinIndex(sciPointObj * pSubwin); /* GET */

GRAPHICS_IMPEXP BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj);

GRAPHICS_IMPEXP void sciGetColorRange(sciPointObj * pObj, int subset[2]); /* GET */
GRAPHICS_IMPEXP void sciGetOutsideColor(sciPointObj * pObj, int colors[2]); /* GET */
GRAPHICS_IMPEXP void sciGetZBounds(sciPointObj * pObj, double bounds[2]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetGridFront(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP int sciGetAntialiasingQuality(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP sciLegendPlace sciGetLegendLocation(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void printSetGetErrorMessage(const char * propertyName);

#endif /* __SCI_GET_PROPERTY__ */
