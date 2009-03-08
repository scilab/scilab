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



#include "ObjectStructure.h"
#include "HandleManagement.h"
#include "StringMatrix.h"
#include "BOOL.h"

void *sciGetPointerToFeature (sciPointObj * pobj); /* GET */
void sciGetPointerToUserData (sciPointObj * pobj,int ***user_data_ptr, int **size_ptr); /* GET */
sciEntityType sciGetEntityType (sciPointObj * pobj);  /* GET */
char *sciGetCharEntityType (sciPointObj * pobj);  /* GET */

/* Graphic Context Functions */
sciGraphicContext *sciGetGraphicContext (sciPointObj * pobj); /* GET */
int sciGetNumColors (sciPointObj * pobj); /* GET */
int sciGetColormap (sciPointObj * pobj, double rgbmat[] ); /* GET */
int sciGetGoodIndex(sciPointObj * pobj, int colorindex);  /* GET */

int sciGetForegroundToDisplay (sciPointObj * pobj); /* GET */
int sciGetBackgroundToDisplay (sciPointObj * pobj); /* GET */

int sciGetBackground (sciPointObj * pobj); /* GET */
int sciGetForeground (sciPointObj * pobj); /* GET */
int sciGetLineWidth (sciPointObj * pobj); /* GET */
BOOL sciGetIsLine (sciPointObj * pobj); /* GET */
int sciGetLineStyle (sciPointObj * pobj); /* GET */
BOOL sciGetIsMark (sciPointObj * pobj); /* GET */
BOOL sciGetIsFilled (sciPointObj * pobj); /* GET */
double sciGetArrowSize (sciPointObj * pobj); /* GET */

EAxesBoxType sciGetBoxType( sciPointObj * pobj ) ; /* GET */
BOOL sciGetIsBoxed (sciPointObj * pobj); /* GET */
int sciGetMarkStyle (sciPointObj * pobj); /* GET */
int sciGetMarkSize (sciPointObj * pobj); /* GET */
int sciGetMarkSizeUnit (sciPointObj * pobj); /* GET */

/* Text Functions */
sciFont *sciGetFontContext (sciPointObj * pobj);  /* GET */
double sciGetFontSize(sciPointObj * pobj); /* GET */
double sciGetFontOrientation (sciPointObj * pobj); /* GET */
StringMatrix * sciGetText (sciPointObj * pobj); /* GET */
void sciGetTextSize( sciPointObj * pobj, int * nbRow, int * nbCol ) ; /* GET */
BOOL sciisTextEmpty( sciPointObj * pobj);
unsigned int sciGetTextLength (sciPointObj * pobj); /* GET */

int sciGetFontBackground (sciPointObj * pobj); /* GET */
int sciGetFontForeground (sciPointObj * pobj); /* GET */
int sciGetMarkBackground (sciPointObj * pobj); /* GET */
int sciGetMarkForeground (sciPointObj * pobj); /* GET */
/* F.Leray 09.04.04*/
int sciGetFontBackgroundToDisplay (sciPointObj * pobj); /* GET */
int sciGetFontForegroundToDisplay (sciPointObj * pobj); /* GET */
int sciGetMarkBackgroundToDisplay (sciPointObj * pobj); /* GET */
int sciGetMarkForegroundToDisplay (sciPointObj * pobj); /* GET */

int sciGetFontStyle (sciPointObj * pobj); /* GET */

/* Legend */
sciLegendPlace sciGetLegendPlace (sciPointObj * pobj);
void sciGetLegendPos (sciPointObj * pobj , double position[]);

/* Figure / Subwin main functions */
sciPointObj *sciGetParentFigure (sciPointObj * pobj); /* GET */
sciPointObj *sciGetParentSubwin (sciPointObj * pobj); /* GET */
int sciGetNumFigure (sciPointObj * pobj); /* GET */

/* GMODE */
scigMode *sciGetGraphicMode (sciPointObj * pobj);  /* GET */
int sciGetIsClipRegionValuated (sciPointObj * pobj);
int sciGetIsClipping (sciPointObj * pobj); /* GET */
double *sciGetClipping (sciPointObj * pobj); /* GET */
BOOL sciGetAddPlot (sciPointObj * pobj);  /* GET */
BOOL sciGetAutoScale (sciPointObj * pobj);  /* GET */
BOOL sciGetZooming (sciPointObj * pobj);  /* GET */
int sciGetXorMode (sciPointObj * pobj);  /* GET */
BOOL sciGetRealVisibility (sciPointObj * pobj) ; /* GET */
BOOL sciGetVisibility (sciPointObj * pobj);  /* GET */
BOOL sciGetResize (sciPointObj * pobj);  /* GET */


/* Window Functions */
char *sciGetName (sciPointObj * pobj);  /* GET */
int sciGetNum (sciPointObj * pobj);  /* GET */
int sciGetWidth (sciPointObj * pobj);   /* GET */
int sciGetHeight (sciPointObj * pobj);  /* GET */
void sciGetDim( sciPointObj * pobj, int * pWidth, int * pHeight ) ; /* GET */
int sciGetWindowWidth(sciPointObj * pObj) ; /* GET */
int sciGetWindowHeight(sciPointObj * pObj) ; /* GET */
sciPointObj *sciIsExistingSubWin (double *WRect); /* GET */ /* WARNING special case here */


double *sciGetPoint (sciPointObj * pthis, int *num, int *numco); /* GET */

BOOL sciGetdrawmode (sciPointObj *pobj); /* GET */
sciPointObj *sciGetAxes (sciPointObj *pparentfigure,sciPointObj *psubwin); /* GET */


int sciType (char *marker, sciPointObj *pobj); /* GET */

sciPointObj *sciGetSurface(sciPointObj *psubwin); /* GET */

char sciGetxLocation(sciPointObj * pObj);
char sciGetyLocation(sciPointObj * pObj);


int CheckForCompound (long *tabpointobj, int number); /* GET */

/* UNUSED ? */
sciPointObj *sciGetOriginalSubWin (sciPointObj * pfigure);

int * sciGetInterpVector(sciPointObj * pobj); /* GET */
BOOL sciGetIsColorInterpolated(sciPointObj * pobj); /* GET */

BOOL sciGetAutoRotation ( sciPointObj * pObj ) ;
BOOL sciGetAutoPosition ( sciPointObj * pObj ) ;

BOOL sciGetCenterPos( sciPointObj * pObj ) ;

BOOL sciGetIs3d( sciPointObj * pObj ) ;

/**
 * for a subwindow object, return if the its labels (x,y,z) are all
 *       empty or not.
 * @param pObj the subwindow.
 */
BOOL sciGetLegendDefined( sciPointObj * pObj ) ;

BOOL sciGetAutoSize( sciPointObj * pObj ) ;
sciTextAlignment sciGetAlignment( sciPointObj * pObj ) ;
void sciGetUserSize( sciPointObj * pObj, double * width, double * height ) ;

int sciGetNbChildren( sciPointObj * pObj ) ; /* GET */

BOOL sciGetIsAccessibleChild( sciPointObj * pObj ) ; /* GET */

int sciGetNbAccessibleChildren( sciPointObj * pObj ) ; /* GET */

BOOL GetHandleVisibilityOnUimenu( sciPointObj * pobj ) ; /* GET */

int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type ) ;

int sciGetHiddenColor( sciPointObj * pObj ) ; /* GET */

int sciGetHiddenAxisColor( sciPointObj * pObj ) ; /* GET */

BOOL sciGetIsAutoDrawable( sciPointObj * pobj ) ;
BOOL sciGetImmediateDrawingMode( sciPointObj * pobj ) ;


void sciGetGridStyle( sciPointObj * pObj, int * xStyle, int * yStyle, int * zStyle ) ;

void sciGetViewport( sciPointObj * pObj, int viewport[4] );

void sciGetInfoMessage( sciPointObj * pObj, char * infoMessage ) ;

int sciGetInfoMessageLength( sciPointObj * pObj ) ;

void sciGetScreenPosition( sciPointObj * pObj, int * posX, int * posY ) ;

BOOL sciGetIsEventHandlerEnable( sciPointObj * pObj ) ;
char * sciGetEventHandler( sciPointObj * pObj ) ;

double * sciGetAxesBounds( sciPointObj * pObj ) ;
double * sciGetMargins( sciPointObj * pObj ) ;

void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* GET */
void sciGetDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* GET */
void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6]); /* GET */

void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta) ; /* GET */

void sciGetLogFlags(sciPointObj * pObj, char flags[3]); /* GET */

BOOL sciGetPixmapMode(sciPointObj * pObj); /* GET */

int sciGetNbPoints(sciPointObj * pObj); /* GET */

int sciGetPolylineStyle(sciPointObj * pObj); /* GET */

BOOL sciGetIsClosed(sciPointObj * pObj); /* GET */

void sciGetTextPos(sciPointObj * pObj, double position[3]); /* GET */

void sciGetPixelCoordinate(sciPointObj * pObj, const double userCoord[3], int pixCoord[2]); /* GET */

void sciGet2dViewCoordinate(sciPointObj * pObj, const double userCoords3D[3], double userCoords2D[2]); /* GET */

void sciGet2dViewCoordFromPixel(sciPointObj * pObj, const int pixelCoords[2], double userCoords2D[2]); /* GET */

void sciGet2dViewPixelCoordinates(sciPointObj * pObj, const double userCoords2D[2], int pixelCoords[2]); /* GET */

BOOL sciGetIsIsoView(sciPointObj * pObj); /* GET */

BOOL sciGetIsCubeScaled(sciPointObj * pObj); /* GET */

void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3]); /* GET */

BOOL sciGetTightLimitsOn(sciPointObj * pObj); /* GET */

void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3]); /* GET */

BOOL sciGetAutoSubticks(sciPointObj * pObj); /* GET */

void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3]); /* GET */

void sciGetZoomBox(sciPointObj * pObj, double zoomBox[6]); /* GET */

void sciGet2dViewBoundingBox(sciPointObj * pObj, double corner1[2], double corner2[2],
                             double corner3[2], double corner4[2]); /* GET */

void sciGetTextBoundingBox(sciPointObj * pObj, double corner1[3], double corner2[3],
                           double corner3[3], double corner4[3]); /* GET */

void sciGetPixelBoundingBox(sciPointObj * pObj, int corner1[2], int corner2[2],
                            int corner3[2], int corner4[2]); /* GET */

void sciGetViewingArea(sciPointObj * pObj, int * xPos, int * yPos, int * width, int * height); /* GET */

void sciGetAABoundingBox(sciPointObj * pObj, double bounds[6]); /* GET */

BOOL sciGetUseNurbs(sciPointObj * pObj); /* GET */

BOOL sciGetIsUsingFractionalMetrics(sciPointObj * pObj); /* GET */

int sciGetNbXTicks(sciPointObj * pSubwin);
void sciGetXTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
int sciGetNbYTicks(sciPointObj * pSubwin);
void sciGetYTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
int sciGetNbZTicks(sciPointObj * pSubwin);
void sciGetZTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);

BOOL sciGetIsAbleToCreateWindow(void);

int sciGetSubwinIndex(sciPointObj * pSubwin); /* GET */

BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj);

void sciGetColorRange(sciPointObj * pObj, int subset[2]); /* GET */
void sciGetOutsideColor(sciPointObj * pObj, int colors[2]); /* GET */
void sciGetZBounds(sciPointObj * pObj, double bounds[2]); /* GET */

BOOL sciGetGridFront(sciPointObj * pObj); /* GET */

sciLegendPlace sciGetLegendLocation(sciPointObj * pObj); /* GET */

void printSetGetErrorMessage(const char * propertyName);

#endif /* __SCI_GET_PROPERTY__ */
