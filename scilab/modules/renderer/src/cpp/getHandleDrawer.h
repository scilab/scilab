/*------------------------------------------------------------------------*/
/* file: getHandleDrawer.h                                                */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Contains the function to retrieve a the drawer of a graphic     */
/*        handle                                                          */
/*------------------------------------------------------------------------*/

#ifndef _GET_HANDLE_DRAWER_H_
#define _GET_HANDLE_DRAWER_H_

extern "C"
{
#include "../../../graphics/includes/ObjectStructure.h"
}
#include "DrawableObject.h"
#include "figureDrawing/DrawableFigure.h"
#include "rectangleDrawing/DrawableRectangle.h"
#include "arcDrawing/DrawableArc.h"
#include "polylineDrawing/DrawablePolyline.h"
#include "textDrawing/DrawableText.h"
#include "subwinDrawing/DrawableSubwin.h"
#include "labelDrawing/DrawableLabel.h"
#include "surfaceDrawing/DrawableSurface.h"
#include "segsDrawing/DrawableSegs.h"

namespace sciGraphics
{

/**
 * Wrapping of the polymorph class drawable object.
 * It is not possible to use (void *) to cast polymorph class
 * so we need to wrap it.
 */
struct DrawableObjectWrapper
{
  DrawableObject * drawer ;
} ;

/*------------------------------------------------------------------------------*/

/**
 * Return the drawer of a graphic handle
 * If none exist it is automaticaly created using the drawable
 * object factory.
 */
DrawableObject * getHandleDrawer( sciPointObj * pObj ) ;

/**
 * Return the pointer on the drawer of a graphic handle
 * Direct access, no allocation is done.
 */
DrawableObject * getHandleDrawerPointer( sciPointObj * pObj ) ;

/**
 * Set the drawer of a sciPointObj.
 * Should not be used directly.
 */
void setHandleDrawerPointer( sciPointObj * pObj, DrawableObject * drawer) ;

/*------------------------------------------------------------------------------*/

/**
 * Return a DrawableFigure object corresponding to a sciPointObj object.
 * @return the corresponding figure to pFigure if pFigure if of type figure, NULL otherwise
 */
DrawableFigure * getFigureDrawer( sciPointObj * pFigure ) ;

/**
 * Return a DrawableRectangle object corresponding to a sciPointObj object.
 * @return the corresponding rectangle to pRectangle if pRectangle if of type rectangle, NULL otherwise
 */
DrawableRectangle * getRectangleDrawer( sciPointObj * pRectangle ) ;

/**
 * Return a DrawableArc object corresponding to a sciPointObj object.
 * @return the corresponding arc to pArc if pArc if of type arc, NULL otherwise
 */
DrawableArc * getArcDrawer( sciPointObj * pArc ) ;

/**
 * Return a DrawablePolyline object corresponding to a sciPointObj object.
 * @return the corresponding polyline to pPolyline if pPolyline if of type polyline, NULL otherwise
 */
DrawablePolyline * getPolylineDrawer( sciPointObj * pPolyline );

/**
 * Return a DrawableText object corresponding to a sciPointObj object.
 * @return the corresponding text to pText if pText if of type text, NULL otherwise
 */
DrawableText * getTextDrawer( sciPointObj * pText );

/**
 * Return a DrawableSubwin object corresponding to a sciPointObj object.
 * @return the corresponding subwin to pSubwin if pSubwin if of type subwin, NULL otherwise
 */
DrawableSubwin * getSubwinDrawer( sciPointObj * pSubwin );

/**
 * Return a DrawableLabel object corresponding to a sciPointObj object.
 * @return the corresponding label to pLabel if pLabel if of type label, NULL otherwise
 */
DrawableLabel * getLabelDrawer( sciPointObj * pLabel );

/**
 * Return a DrawableSurface object corresponding to a sciPointObj object.
 * @return the corresponding surface to pSurface if pSurface if of type surface, NULL otherwise
 */
DrawableSurface * getSurfaceDrawer( sciPointObj * pSurface );

/**
 * Return a DrawableSegs object corresponding to a sciPointObj object.
 * @return the corresponding segs to pSegs if pSegs if of type segs, NULL otherwise
 */
DrawableSegs * getSegsDrawer( sciPointObj * pSegs );

}

#endif /* _GET_HANDLE_DRAWER_H_  */
