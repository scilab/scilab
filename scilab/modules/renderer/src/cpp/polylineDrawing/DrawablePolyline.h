/*------------------------------------------------------------------------*/
/* file: DrawablePolyline.h                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing the drawing routine for a polyline object      */
/*------------------------------------------------------------------------*/

#ifndef _DRAWABLE_POLYLINE_H_
#define _DRAWABLE_POLYLINE_H_


#include "../DrawableClippedObject.h"
#include "DrawablePolylineBridge.h"

namespace sciGraphics
{

class DrawablePolyline : public DrawableClippedObject
{

public:

  DrawablePolyline( sciPointObj * pObj ) : DrawableClippedObject( pObj ) {}

  virtual ~DrawablePolyline( void ) {}

  /**
   * Retrieve the vertices of the polyline which will be used to draw it directly.
   * In the polyline sciPointObj, x, y and z vectors does not always stands for the real vertices
   * of the drawn line (specially when staircase mode is on).
   * To allocate the array for retrieving vertices, user shoud use the getDrawnVerticesLength.
   * @param xCoords X coordinates of the drawn vertices
   * @param yCoords Y coordinates of the drawn vertices
   * @param zCoords Z coordinates of the drawn vertices
   */
  virtual void getDrawnVertices(double xCoords[], double yCcoords[], double zCcoords[]) = 0 ;

  /**
   * @return number of vertices which will be computed by the getDrawnVertices routine.
   */
  virtual int getDrawnVerticesLength(void) = 0 ;

  /**
   * Return the real type of implementation object
   */
  DrawablePolylineBridge * getPolylineImp( void ) ;

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Draw the graphic handle and store it representation in memory
   * for later faster drawing.
   */
  virtual void draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual void show( void ) ;

  /**
   * Actually draw the arc on the screen
   */
  virtual void drawPolyline(void) = 0;

  /**
  * Actually show the arc stored data
  */
  virtual void showPolyline(void) = 0;
  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_POLYLINE_H_  */
