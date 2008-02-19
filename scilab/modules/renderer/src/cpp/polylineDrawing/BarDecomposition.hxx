/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy 
 * desc : Decompose polyline to get data for bar drawing   
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _BAR_DECOMPOSITION_HXX_
#define _BAR_DECOMPOSITION_HXX_

#include "DecomposeLineStrategy.hxx"
#include "LinearBarDecomposition.hxx"

namespace sciGraphics
{

class BarDecomposition : public DecomposeLineStrategy
{
public:

  BarDecomposition( DrawablePolyline * polyline );

  virtual ~BarDecomposition( void );

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

  /**
   * Get the top and bottom ordinates of each bar to draw.
   * @param top array containing top abscissa of the bars. Its size is getDrawnVerticesLength().
   * @param bottom array containing the bottom abscissa of the bars. Its size is getDrawnVerticesLength().
   */
  void getBarOrdinates(double bottom[], double top[]);

  /**
   * Get the left and right abscissas of each bar to draw.
   * @param left array containing left abscissa of the bars. Its size is getDrawnVerticesLength().
   * @param right array containing the right abscissa of the bars. Its size is getDrawnVerticesLength().
   */
  void getBarAbscissas(double left[], double right[]);

  /**
   * Bar plot mode (ie polyline_style = 3), also draws mark on y = 0 line.
   */
  void getBarPlotMarkVertices(double xCoords[], double yCoords[], double zCoords[]);

  /**
   * @return number of vertices for bar plot drawing of marks
   */
  int getBarPlotMarkVerticesLength(void);

protected:

  /*---------------------------------------------------------------------------------*/
  /**
   * Get the top and bottom ordinates of each bar in linear coordinates.
   */
  void getBarLinearOrdinates(double bottom[], double top[]);

  /**
   * Get the left and right abscissas of each bar in linear coordinates.
   */
  void getBarLinearAbscissas(double left[], double right[]);
  /*---------------------------------------------------------------------------------*/
  LinearBarDecomposition * m_pExtentComputer;
  /*---------------------------------------------------------------------------------*/

};

}

#endif /* _BAR_DECOMPOSITION_HXX_ */

