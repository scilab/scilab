/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "GridDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
GridDrawerJavaMapper::GridDrawerJavaMapper(void)
  : DrawableObjectJavaMapper()
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::GridDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
GridDrawerJavaMapper::~GridDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::drawGrid(void)
{
  m_pJavaObject->drawGrid();
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::setGridParameters(int gridColor, float gridThickness)
{
  m_pJavaObject->setGridParameters(gridColor, gridThickness);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::setGridStartPoints(const double startPointsX[],
                                              const double startPointsY[],
                                              const double startPointsZ[],
                                              int nbPoints)
{
  m_pJavaObject->setGridStartPoints((double *) startPointsX, nbPoints,
                                    (double *) startPointsY, nbPoints,
                                    (double *) startPointsZ, nbPoints);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::setGridMiddlePoints(const double middlePointsX[],
                                               const double middlePointsY[],
                                               const double middlePointsZ[],
                                               int nbPoints)
{
  m_pJavaObject->setGridMiddlePoints((double *) middlePointsX, nbPoints,
                                     (double *) middlePointsY, nbPoints,
                                     (double *) middlePointsZ, nbPoints);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::setGridEndPoints(const double endPointsX[],
                                            const double endPointsY[],
                                            const double endPointsZ[],
                                            int nbPoints)
{
  m_pJavaObject->setGridEndPoints((double *) endPointsX, nbPoints,
                                  (double *) endPointsY, nbPoints,
                                  (double *) endPointsZ, nbPoints);
}
/*--------------------------------------------------------------------------*/
}

