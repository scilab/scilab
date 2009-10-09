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

void *sciGetPointerToFeature (sciPointObj * pobj);
void sciGetPointerToUserData (sciPointObj * pobj,int ***user_data_ptr, int **size_ptr);
sciEntityType sciGetEntityType (sciPointObj * pobj); 
char *sciGetCharEntityType (sciPointObj * pobj); 

/* Graphic Context Functions */
sciGraphicContext *sciGetGraphicContext (sciPointObj * pobj);
int sciGetNumColors (sciPointObj * pobj);
int sciGetColormap (sciPointObj * pobj, double rgbmat[] );
int sciGetGoodIndex(sciPointObj * pobj, int colorindex); 

/**
* sciGetBackgroundColor function 
* @return the stored value of BackgroundColor index
*/
int sciGetBackgroundColor (sciPointObj * pobj);

/**
* sciGetForegroundColor function 
* @return the stored value of ForegroundColor index
*/
int sciGetForegroundColor (sciPointObj * pobj);

/**
* sciGetMarkBackgroundColor function 
* @return the stored value of MarkBackgroundColor index
*/
int sciGetMarkBackgroundColor (sciPointObj * pobj);

/**
* sciGetMarkForegroundColor function 
* @return the stored value of MarkForegroundColor index
*/
int sciGetMarkForegroundColor (sciPointObj * pobj);

int sciGetForeground (sciPointObj * pobj);
int sciGetForegroundToDisplay (sciPointObj * pobj);

int sciGetBackground (sciPointObj * pobj);
int sciGetBackgroundToDisplay (sciPointObj * pobj);

int sciGetMarkBackground (sciPointObj * pobj);
int sciGetMarkBackgroundToDisplay (sciPointObj * pobj);

int sciGetMarkForeground (sciPointObj * pobj);
int sciGetMarkForegroundToDisplay (sciPointObj * pobj);

int sciGetLineWidth (sciPointObj * pobj);
BOOL sciGetIsLine (sciPointObj * pobj);
int sciGetLineStyle (sciPointObj * pobj);
BOOL sciGetIsMark (sciPointObj * pobj);
BOOL sciGetIsFilled (sciPointObj * pobj);
double sciGetArrowSize (sciPointObj * pobj);

EAxesBoxType sciGetBoxType( sciPointObj * pobj ) ;
BOOL sciGetIsBoxed (sciPointObj * pobj);
int sciGetMarkStyle (sciPointObj * pobj);
int sciGetMarkSize (sciPointObj * pobj);
int sciGetMarkSizeUnit (sciPointObj * pobj);

/* Text Functions */
sciFont *sciGetFontContext (sciPointObj * pobj); 
double sciGetFontSize(sciPointObj * pobj);
double sciGetFontOrientation (sciPointObj * pobj);
StringMatrix * sciGetText (sciPointObj * pobj);
void sciGetTextSize( sciPointObj * pobj, int * nbRow, int * nbCol ) ;
BOOL sciisTextEmpty( sciPointObj * pobj);
unsigned int sciGetTextLength (sciPointObj * pobj);

int sciGetFontBackground (sciPointObj * pobj);
int sciGetFontForeground (sciPointObj * pobj);
/* F.Leray 09.04.04*/
int sciGetFontBackgroundToDisplay (sciPointObj * pobj);
int sciGetFontForegroundToDisplay (sciPointObj * pobj);

int sciGetFontStyle (sciPointObj * pobj);

/* Legend */
sciLegendPlace sciGetLegendPlace (sciPointObj * pobj);
void sciGetLegendPos (sciPointObj * pobj , double position[]);

/* Figure / Subwin main functions */
sciPointObj *sciGetParentFigure (sciPointObj * pobj);
sciPointObj *sciGetParentSubwin (sciPointObj * pobj);
int sciGetNumFigure (sciPointObj * pobj);

/* GMODE */
scigMode *sciGetGraphicMode (sciPointObj * pobj); 
int sciGetIsClipRegionValuated (sciPointObj * pobj);
int sciGetIsClipping (sciPointObj * pobj);
double *sciGetClipping (sciPointObj * pobj);
BOOL sciGetAddPlot (sciPointObj * pobj); 
BOOL sciGetAutoScale (sciPointObj * pobj); 
BOOL sciGetZooming (sciPointObj * pobj); 
int sciGetXorMode (sciPointObj * pobj); 
BOOL sciGetRealVisibility (sciPointObj * pobj) ;
BOOL sciGetVisibility (sciPointObj * pobj); 
BOOL sciGetResize (sciPointObj * pobj); 


/* Window Functions */
char *sciGetName (sciPointObj * pobj); 
int sciGetNum (sciPointObj * pobj); 
int sciGetWidth (sciPointObj * pobj);  
int sciGetHeight (sciPointObj * pobj); 
void sciGetDim( sciPointObj * pobj, int * pWidth, int * pHeight ) ;
int sciGetWindowWidth(sciPointObj * pObj) ;
int sciGetWindowHeight(sciPointObj * pObj) ;
sciPointObj *sciIsExistingSubWin (double *WRect); /* WARNING special case here */


double *sciGetPoint (sciPointObj * pthis, int *num, int *numco);

BOOL sciGetdrawmode (sciPointObj *pobj);
sciPointObj *sciGetAxes (sciPointObj *pparentfigure,sciPointObj *psubwin);


int sciType (char *marker, sciPointObj *pobj);

sciPointObj *sciGetSurface(sciPointObj *psubwin);

char sciGetxLocation(sciPointObj * pObj);
char sciGetyLocation(sciPointObj * pObj);


int CheckForCompound (long *tabpointobj, int number);

/* UNUSED ? */
sciPointObj *sciGetOriginalSubWin (sciPointObj * pfigure);

int * sciGetInterpVector(sciPointObj * pobj);
BOOL sciGetIsColorInterpolated(sciPointObj * pobj);

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

int sciGetNbChildren( sciPointObj * pObj ) ;

BOOL sciGetIsAccessibleChild( sciPointObj * pObj ) ;

int sciGetNbAccessibleChildren( sciPointObj * pObj ) ;

BOOL GetHandleVisibilityOnUimenu( sciPointObj * pobj ) ;

int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type ) ;

int sciGetHiddenColor( sciPointObj * pObj ) ;

int sciGetHiddenAxisColor( sciPointObj * pObj ) ;

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

void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] ) ;
void sciGetDataBounds( sciPointObj * pObj, double bounds[6] ) ;
void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6]);

void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta) ;

void sciGetLogFlags(sciPointObj * pObj, char flags[3]);

BOOL sciGetPixmapMode(sciPointObj * pObj);

int sciGetNbPoints(sciPointObj * pObj);

int sciGetPolylineStyle(sciPointObj * pObj);

BOOL sciGetIsClosed(sciPointObj * pObj);

void sciGetTextPos(sciPointObj * pObj, double position[3]);

void sciGetPixelCoordinate(sciPointObj * pObj, const double userCoord[3], int pixCoord[2]);

void sciGet2dViewCoordinate(sciPointObj * pObj, const double userCoords3D[3], double userCoords2D[2]);

void sciGet2dViewCoordFromPixel(sciPointObj * pObj, const int pixelCoords[2], double userCoords2D[2]);

void sciGet2dViewPixelCoordinates(sciPointObj * pObj, const double userCoords2D[2], int pixelCoords[2]);

BOOL sciGetIsIsoView(sciPointObj * pObj);

BOOL sciGetIsCubeScaled(sciPointObj * pObj);

void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3]);

BOOL sciGetTightLimitsOn(sciPointObj * pObj);

void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3]);

BOOL sciGetAutoSubticks(sciPointObj * pObj);

void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3]);

void sciGetZoomBox(sciPointObj * pObj, double zoomBox[6]);

void sciGet2dViewBoundingBox(sciPointObj * pObj, double corner1[2], double corner2[2],
                             double corner3[2], double corner4[2]);

void sciGetTextBoundingBox(sciPointObj * pObj, double corner1[3], double corner2[3],
                           double corner3[3], double corner4[3]);

void sciGetPixelBoundingBox(sciPointObj * pObj, int corner1[2], int corner2[2],
                            int corner3[2], int corner4[2]);

void sciGetViewingArea(sciPointObj * pObj, int * xPos, int * yPos, int * width, int * height);

void sciGetAABoundingBox(sciPointObj * pObj, double bounds[6]);

BOOL sciGetUseNurbs(sciPointObj * pObj);

BOOL sciGetIsUsingFractionalMetrics(sciPointObj * pObj);

int sciGetNbXTicks(sciPointObj * pSubwin);
void sciGetXTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
int sciGetNbYTicks(sciPointObj * pSubwin);
void sciGetYTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);
int sciGetNbZTicks(sciPointObj * pSubwin);
void sciGetZTicksPos(sciPointObj * pSubwin, double ticksPos[], char ** ticksLabels);

BOOL sciGetIsAbleToCreateWindow(void);

int sciGetSubwinIndex(sciPointObj * pSubwin);

BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj);

void sciGetColorRange(sciPointObj * pObj, int subset[2]);
void sciGetOutsideColor(sciPointObj * pObj, int colors[2]);
void sciGetZBounds(sciPointObj * pObj, double bounds[2]);

BOOL sciGetGridFront(sciPointObj * pObj);

sciLegendPlace sciGetLegendLocation(sciPointObj * pObj);

void printSetGetErrorMessage(const char * propertyName);

#endif /* __SCI_GET_PROPERTY__ */
