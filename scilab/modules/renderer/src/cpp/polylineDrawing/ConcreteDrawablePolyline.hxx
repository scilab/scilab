/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Class containing implementation of driver independent
 * algorithms
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _CONCRETE_DRAWABLE_POLYLINE_HXX_
#define _CONCRETE_DRAWABLE_POLYLINE_HXX_

#include "DrawablePolyline.h"
#include <list>
#include "DrawPolylineStrategy.hxx"
#include "DecomposeLineStrategy.hxx"

namespace sciGraphics
{

class ConcreteDrawablePolyline : public DrawablePolyline
{
public:

  ConcreteDrawablePolyline(sciPointObj * pObj);

  virtual ~ConcreteDrawablePolyline(void);

  /**
   * Retrieve the vertices of the polyline which will be used to draw it directly.
   * In the polyline sciPointObj, x, y and z vectors does not always stands for the real vertices
   * of the drawn line (specially when staircase mode is on).
   * To allocate the array for retrieving vertices, user shoud use the getDrawnVerticesLength.
   * @param xCoords X coordinates of the drawn vertices
   * @param yCoords Y coordinates of the drawn vertices
   * @param zCoords Z coordinates of the drawn vertices
   */
  virtual void getDrawnVertices(double xCoords[], double yCcoords[], double zCcoords[]);

  /**
  * Retrieve the colors to use for each vertex. To be used for color interpolation.
  * The size of the array must be retrieved by getDrawnVerticesLength().
  * @param colors color inddex of the draw vertices
  */
  virtual void getDrawnVerticesColor(int colors[]);

  /**
   * @return number of vertices which will be computed by the getDrawnVertices routine.
   */
  virtual int getDrawnVerticesLength(void);

  /**
   * Add a rendering algorithm
   */
  void addDrawingStrategy(DrawPolylineStrategy * strategy);

  /**
   * Remove all drawing strategies.
   */
  void removeDrawingStrategies(void);

  /**
   * Add an algorithm for decomposing line.
   */
  void setDecompositionStrategy(DecomposeLineStrategy * strategy);


protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Actually draw the polyline on the screen
   */
  virtual void drawPolyline(void);

  /**
   * Actually show the polyline stored data
   */
  virtual void showPolyline(void);

  /**
   * Actually redraw the polyline stored data
   */
  virtual void redrawPolyline(void);
  /*---------------------------------------------------------------------------------*/
  std::list<DrawPolylineStrategy *> m_oDrawingStrategies;
  DecomposeLineStrategy * m_pDecomposeStrategy;
  /*---------------------------------------------------------------------------------*/

};

}

#endif /* _CONCRETE_DRAWABLE_POLYLINE_HXX_ */

