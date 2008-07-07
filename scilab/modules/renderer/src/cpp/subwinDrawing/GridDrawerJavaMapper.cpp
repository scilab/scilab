/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing java methods needed by XGridDrawerJoGL
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
void GridDrawerJavaMapper::drawGrid(const double firstAxisStart[3], const double firstAxisEnd[3],
                                    const double secondAxisStart[3], const double secondAxisEnd[3],
                                    const double thirdAxisStart[3], const double thirdAxisEnd[3],
                                    const double relativeTicksPositions[], int nbTicks)
{
  m_pJavaObject->drawGrid((double *) firstAxisStart, 3,
                          (double *) firstAxisEnd, 3,
                          (double *) secondAxisStart, 3,
                          (double *) secondAxisEnd, 3,
                          (double *) thirdAxisStart, 3,
                          (double *) thirdAxisEnd, 3,
                          (double *) relativeTicksPositions, nbTicks);
}
/*--------------------------------------------------------------------------*/
void GridDrawerJavaMapper::setGridParameters(int gridColor, float gridThickness)
{
  m_pJavaObject->setGridParameters(gridColor, gridThickness);
}
/*--------------------------------------------------------------------------*/
}

