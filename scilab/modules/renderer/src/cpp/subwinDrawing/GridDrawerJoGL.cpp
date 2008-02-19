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

}
/*------------------------------------------------------------------------------------------*/
GridDrawerJoGL::~GridDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::drawGrid(const double gridPositions[], int nbPositions)
{
  getGridDrawerJavaMapper()->drawGrid(gridPositions, nbPositions);
}
/*------------------------------------------------------------------------------------------*/
void GridDrawerJoGL::initializeDrawing(void)
{
  DrawableObjectJoGL::initializeDrawing();

  sciPointObj * pSubwin = getDrawer()->getDrawedObject();

  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getGridDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1], bounds[2],
                                           bounds[3], bounds[4], bounds[5]);
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
