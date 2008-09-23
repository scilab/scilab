/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy 
 * desc : Strategy for the mesh of a fec object 
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#ifndef _FEC_LINE_DRAWER_JOGL_HXX_
#define _FEC_LINE_DRAWER_JOGL_HXX_

#include "DrawFecStrategy.hxx"
#include "DrawableObjectJoGL.h"
#include "FecLineDrawerJavaMapper.hxx"

namespace sciGraphics
{

class FecLineDrawerJoGL : public DrawFecStrategy, DrawableObjectJoGL
{
public:

  FecLineDrawerJoGL(DrawableFec * fec);

  virtual ~FecLineDrawerJoGL(void);

  /**
   * Draw the fec using this strategy
   * @param xCoords X coordinates of nodes
   * @param yCoords Y coordinates of nodes
   * @param values value for each node
   * @param nbNodes number of nodes (size of xCoords and yCoords).
   * @param firstPoints node indexes of triangles first point
   * @param seconfPoints node indexes of triangles second point
   * @param seconfPoints node indexes of triangles third point
   * @param nbTriangles number of triangle (size of first, second and thirdPoints).
   */
  virtual void drawFec(const double xCoords[], const double yCoords[],
                       const double values[], int nbNodes,
                       const int firstPoints[], const int secondPoints[],
                       const int thirdPoints[], int nbTriangles);

  /**
   * Show the fec using this strategy
   */
  virtual void showFec(void);

protected:

  virtual DrawableFec * getFecDrawer(void);

  virtual FecLineDrawerJavaMapper * getLineDrawerJavaMapper(void);

};

}

#endif /* _FEC_LINE_DRAWER_JOGL_HXX_ */

