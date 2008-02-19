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

#include "BottomXTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
BottomXTicksDrawerJavaMapper::BottomXTicksDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::BottomXTicksDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
BottomXTicksDrawerJavaMapper::~BottomXTicksDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
double BottomXTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                               int nbTicks, const double subticksPositions[], int nbSubticks)
{
  return m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                                  ticksLabels, nbTicks,
                                  (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool BottomXTicksDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks)
{
  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks);
}
/*--------------------------------------------------------------------------*/
double BottomXTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                               char * labelsExponents[], int nbTicks,
                                               const double subticksPositions[], int nbSubticks)
{
  return m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                                  ticksLabels, nbTicks,
                                  labelsExponents, nbTicks,
                                  (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool BottomXTicksDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[],
  char * labelsExponents[], int nbTicks)
{

  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks,
                                   labelsExponents, nbTicks);
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                                 double yMin, double yMax,
                                                 double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
void BottomXTicksDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                                      int fontType, double fontSize, int fontColor)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor,
                                   fontType, fontSize, fontColor);
}
/*--------------------------------------------------------------------------*/
}
