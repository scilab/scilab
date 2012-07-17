/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2001 - 2002 - INRIA - Mathieu Philipe
 * Copyright (C) 2002 - 2004 - INRIA - Djalel Abdemouche
 * Copyright (C) 2004 - 2006 - INRIA - Fabrice Leray
 * Copyright (C) 2005 - INRIA - Jean-Baptiste Silvy
 * Copyright (C) 2010 - Paul Griffiths
 * Copyright (C) 2011 - DIGITEO - Bruno JOFRET
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

GRAPHICS_IMPEXP sciEntityType sciGetEntityType (sciPointObj * pobj);  /* GET */

/* Graphic Context Functions */
GRAPHICS_IMPEXP int sciGetNumColors (char* pobjUID); /* GET */

GRAPHICS_IMPEXP double sciGetArrowSize (sciPointObj * pobj); /* GET */

GRAPHICS_IMPEXP EAxesBoxType sciGetBoxType( sciPointObj * pobj ) ; /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsBoxed (sciPointObj * pobj); /* GET */

/* Text Functions */
GRAPHICS_IMPEXP BOOL sciisTextEmpty(char* identifier);

/* Legend */
GRAPHICS_IMPEXP sciLegendPlace sciGetLegendPlace (sciPointObj * pobj);
GRAPHICS_IMPEXP void sciGetLegendPos (sciPointObj * pobj , double position[]);

/* Figure / Subwin main functions */
GRAPHICS_IMPEXP sciPointObj *sciGetParentFigure (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP sciPointObj *sciGetParentSubwin (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP int sciGetNumFigure (sciPointObj * pobj); /* GET */

/* GMODE */
GRAPHICS_IMPEXP scigMode *sciGetGraphicMode (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP int sciGetIsClipping (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP double *sciGetClipping (sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetZooming (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP BOOL sciGetRealVisibility (sciPointObj * pobj) ; /* GET */
GRAPHICS_IMPEXP BOOL sciGetVisibility (sciPointObj * pobj);  /* GET */


/* Window Functions */
GRAPHICS_IMPEXP int sciGetNum (sciPointObj * pobj);  /* GET */
GRAPHICS_IMPEXP sciPointObj *sciIsExistingSubWin (double *WRect); /* GET */ /* WARNING special case here */


GRAPHICS_IMPEXP double *sciGetPoint (char * pthis, int *num, int *numco); /* GET */

GRAPHICS_IMPEXP sciPointObj *sciGetSurface(sciPointObj *psubwin); /* GET */

GRAPHICS_IMPEXP char sciGetxLocation(sciPointObj * pObj);
GRAPHICS_IMPEXP char sciGetyLocation(sciPointObj * pObj);


/* UNUSED ? */
GRAPHICS_IMPEXP sciPointObj *sciGetOriginalSubWin (sciPointObj * pfigure);

GRAPHICS_IMPEXP int * sciGetInterpVector(sciPointObj * pobj); /* GET */
GRAPHICS_IMPEXP BOOL sciGetIsColorInterpolated(sciPointObj * pobj); /* GET */

GRAPHICS_IMPEXP BOOL sciGetAutoRotation ( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP BOOL sciGetAutoPosition ( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP BOOL sciGetCenterPos( sciPointObj * pObj ) ;

/**
 * for a subwindow object, return if the its labels (x,y,z) are all
 *       empty or not.
 * @param pObjUID the subwindow's identifier.
 */
GRAPHICS_IMPEXP BOOL sciGetLegendDefined(char * pObjUID);

GRAPHICS_IMPEXP BOOL sciGetAutoSize( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP sciTextAlignment sciGetAlignment( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP void sciGetUserSize( sciPointObj * pObj, double * width, double * height ) ;

GRAPHICS_IMPEXP int sciGetNbChildren( sciPointObj * pObj ) ; /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsAccessibleChild( sciPointObj * pObj ) ; /* GET */

GRAPHICS_IMPEXP BOOL GetHandleVisibilityOnUimenu( sciPointObj * pobj ) ; /* GET */

GRAPHICS_IMPEXP int sciGetNbTypedObjects( sciPointObj * pObj, sciEntityType type ) ;

GRAPHICS_IMPEXP BOOL sciGetIsAutoDrawable( sciPointObj * pobj ) ;
GRAPHICS_IMPEXP BOOL sciGetImmediateDrawingMode( sciPointObj * pobj ) ;

GRAPHICS_IMPEXP BOOL sciGetIsEventHandlerEnable( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP char * sciGetEventHandler( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP double * sciGetAxesBounds( sciPointObj * pObj ) ;
GRAPHICS_IMPEXP double * sciGetMargins( sciPointObj * pObj ) ;

GRAPHICS_IMPEXP void sciGetRealDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* GET */
GRAPHICS_IMPEXP void sciGetDataBounds( sciPointObj * pObj, double bounds[6] ) ; /* GET */
GRAPHICS_IMPEXP void sciGetDisplayedDataBounds(sciPointObj * pObj, double bounds[6]); /* GET */

GRAPHICS_IMPEXP void sciGetViewingAngles( sciPointObj * pObj, double * alpha, double * theta) ; /* GET */

GRAPHICS_IMPEXP void sciGetLogFlags(char *pObjUID, char flags[3]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetPixmapMode(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP int sciGetNbPoints(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP int sciGetPolylineStyle(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsClosed(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewCoordinate(char * pObjUID, const double userCoords3D[3], double userCoords2D[2]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewCoordFromPixel(char * pObjUID, const int pixelCoords[2], double userCoords2D[2]); /* GET */

GRAPHICS_IMPEXP void sciGet2dViewPixelCoordinates(char * pObjUID, const double userCoords2D[2], int pixelCoords[2]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsIsoView(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP BOOL sciGetIsCubeScaled(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGetAxesReverse(sciPointObj * pObj, BOOL axesReverse[3]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetTightLimitsOn(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGetAutoTicks(sciPointObj * pObj, BOOL autoTicks[3]); /* GET */

GRAPHICS_IMPEXP BOOL sciGetAutoSubticks(sciPointObj * pObj); /* GET */

GRAPHICS_IMPEXP void sciGetAxesVisible(sciPointObj * pObj, BOOL axesVisible[3]); /* GET */

GRAPHICS_IMPEXP void sciGetZoomBox(sciPointObj * pObj, double zoomBox[6]); /* GET */

GRAPHICS_IMPEXP void sciGetViewingArea(char * pObjUID, int * xPos, int * yPos, int * width, int * height); /* GET */

GRAPHICS_IMPEXP int sciGetSubwinIndex(sciPointObj * pSubwin); /* GET */

GRAPHICS_IMPEXP BOOL sciIsAutomaticallyRedrawn(sciPointObj * pObj);

GRAPHICS_IMPEXP void printSetGetErrorMessage(const char * propertyName);

GRAPHICS_IMPEXP void sciGetNbSubTics(sciPointObj * pObj, int nbsubtics[3]);

#endif /* __SCI_GET_PROPERTY__ */
