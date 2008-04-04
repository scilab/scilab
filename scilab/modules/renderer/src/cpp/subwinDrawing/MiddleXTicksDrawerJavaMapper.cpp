/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing java methods needed by XTicksDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */


#include "MiddleXTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
MiddleXTicksDrawerJavaMapper::MiddleXTicksDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::MiddleXTicksDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
MiddleXTicksDrawerJavaMapper::~MiddleXTicksDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
double MiddleXTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                               int nbTicks, const double subticksPositions[], int nbSubticks)
{
  return m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                                  ticksLabels, nbTicks,
                                  (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
double MiddleXTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                               char * labelsExponents[], int nbTicks,
                                               const double subticksPositions[], int nbSubticks)
{
  return m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                                  ticksLabels, nbTicks,
                                  labelsExponents, nbTicks,
                                  (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                                 double yMin, double yMax,
                                                 double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
void MiddleXTicksDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                                      int fontType, double fontSize, int fontColor)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor,
                                   fontType, fontSize, fontColor);
}
/*--------------------------------------------------------------------------*/
}
