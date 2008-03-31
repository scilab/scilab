/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing grid using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "GridDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{
 /*------------------------------------------------------------------------------------------*/
GridDrawerJoGL::GridDrawerJoGL(DrawableSubwin * subwin)
: GridDrawer(subwin), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new GridDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
GridDrawerJoGL::~GridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::setGridStartPoints(const double startPointsX[],
                                        const double startPointsY[],
                                        const double startPointsZ[],
                                        int nbPoints)
{
  getGridDrawerJavaMapper()->setGridStartPoints(startPointsX, startPointsY, startPointsZ, nbPoints);
}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::setGridMiddlePoints(const double middlePointsX[],
                                         const double middlePointsY[],
                                         const double middlePointsZ[],
                                         int nbPoints)
{
  getGridDrawerJavaMapper()->setGridMiddlePoints(middlePointsX,
                                                 middlePointsY,
                                                 middlePointsZ, nbPoints);
}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::setGridEndPoints(const double endPointsX[],
                                      const double endPointsY[],
                                      const double endPointsZ[],
                                      int nbPoints)
{
  getGridDrawerJavaMapper()->setGridEndPoints(endPointsX,
                                              endPointsY,
                                              endPointsZ, nbPoints);
}
/*------------------------------------------------------------------------------------------*/
GridDrawerJavaMapper * GridDrawerJoGL::getGridDrawerJavaMapper(void)
{
  return dynamic_cast<GridDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
