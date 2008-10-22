/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Decompose polyline with creating interpolation between each
 * data (this is the default mode) 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _INTERPOLATED_DECOMPOSITION_HXX_
#define _INTERPOLATED_DECOMPOSITION_HXX_

#include "DecomposeLineStrategy.hxx"

namespace sciGraphics
{

class InterpolatedDecomposition : public DecomposeLineStrategy
{
public:

  InterpolatedDecomposition( DrawablePolyline * polyline );

  virtual ~InterpolatedDecomposition( void );

  /**
   * Retrieve the vertices of the polyline which will be used to draw it directly.
   * In the polyline sciPointObj, x, y and z vectors does not always stands for the real vertices
   * of the drawn line (specially when staircase mode is on).
   * To allocate the array for retrieving vertices, user should use the getDrawnVerticesLength.
   * @param xCoords X coordinates of the drawn vertices
   * @param yCoords Y coordinates of the drawn vertices
   * @param zCoords Z coordinates of the drawn vertices
   */
  virtual void getDrawnVertices(double xCoords[], double yCoords[], double zCoords[]);

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

protected:

};

}

#endif /* _INTERPOLATED_DECOMPOSITION_HXX_ */

