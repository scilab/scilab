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
: GridDrawer(), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new GridDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
GridDrawerJoGL::~GridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::drawGrid(const double firstAxisStart[3], const double firstAxisEnd[3],
                              const double secondAxisStart[3], const double secondAxisEnd[3],
                              const double thirdAxisStart[3], const double thirdAxisEnd[3],
                              const double relativeTicksPositions[], int nbTicks)
{
  getGridDrawerJavaMapper()->drawGrid(firstAxisStart, firstAxisEnd,
                                      secondAxisStart, secondAxisEnd,
                                      thirdAxisStart, thirdAxisEnd,
                                      relativeTicksPositions, nbTicks);
}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::initializeDrawing(void)
{
  DrawableObjectJoGL::initializeDrawing();

  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  // set line color and width
  getGridDrawerJavaMapper()->setGridParameters(getGridStyle(), (float)sciGetLineWidth(pSubwin));

}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::endDrawing(void)
{
  DrawableObjectJoGL::endDrawing();
}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::initializeShowing(void)
{
  DrawableObjectJoGL::initializeDrawing();
}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::endShowing(void)
{
  DrawableObjectJoGL::endDrawing();
}
/*------------------------------------------------------------------------------------------*/
GridDrawerJavaMapper * GridDrawerJoGL::getGridDrawerJavaMapper(void)
{
  return dynamic_cast<GridDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
