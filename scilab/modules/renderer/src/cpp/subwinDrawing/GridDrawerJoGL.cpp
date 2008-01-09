/*------------------------------------------------------------------------*/
/* file: GridDrawerJoGL.cpp                                               */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid using JoGL                    */
/*------------------------------------------------------------------------*/

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
