/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing the drawing routine for a polyline object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _DRAWABLE_POLYLINE_H_
#define _DRAWABLE_POLYLINE_H_


#include "DrawableClippedObject.h"
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
   * Retrieve the colors to use for each vertex. To be used for color interpolation.
   * The size of the array must be retrieved by getDrawnVerticesLength().
   * @param colors color inddex of the draw vertices
   */
  virtual void getDrawnVerticesColor(int colors[]) = 0;

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
  virtual EDisplayStatus draw( void ) ;

  /**
   * Fast draw of the graphic handle on the screen using the data created by draw.
   * Warning, be sure that draw is called before show each time the handle is modified.
   */
  virtual EDisplayStatus show( void ) ;

  /**
   * Draw the polyline using precomputed data
   */
  virtual EDisplayStatus redraw(void);

  /**
   * Actually draw the polyline on the screen
   */
  virtual void drawPolyline(void) = 0;

  /**
   * Actually show the polyline stored data
   */
  virtual void showPolyline(void) = 0;

  /**
   * Actually redraw the polyline stored data
   */
  virtual void redrawPolyline(void) = 0;
  /*---------------------------------------------------------------------------------*/

} ;

}

#endif /* _DRAWABLE_POLYLINE_H_  */
