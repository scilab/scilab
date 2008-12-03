/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing java methods needed by XTicksDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TicksDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
TicksDrawerJavaMapper::TicksDrawerJavaMapper(void)
 : DrawableObjectJavaMapper()
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::TicksDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
TicksDrawerJavaMapper::~TicksDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
double TicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                        int nbTicks, const double subticksPositions[], int nbSubticks,
                                        double axisSegmentStart[3], double axisSegmentEnd[3], double ticksDirection[3])
{
  return m_pJavaObject->drawTicks(axisSegmentStart, 3,
                                  axisSegmentEnd, 3,
                                  ticksDirection, 3,
                                  (double *)ticksPositions, nbTicks,
                                  (double *)subticksPositions, nbSubticks,
                                  ticksLabels, nbTicks);
}
/*--------------------------------------------------------------------------*/
double TicksDrawerJavaMapper::showTicks(void)
{
  return m_pJavaObject->showTicks();
}
/*--------------------------------------------------------------------------*/
double TicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                                        int nbTicks, const double subticksPositions[], int nbSubticks,
                                        double axisSegmentStart[3], double axisSegmentEnd[3], double ticksDirection[3])
{
  return m_pJavaObject->drawTicks(axisSegmentStart, 3,
                                  axisSegmentEnd, 3,
                                  ticksDirection, 3,
                                  (double *)ticksPositions, nbTicks,
                                  (double *)subticksPositions, nbSubticks,
                                  ticksLabels, nbTicks,
                                  labelsExponents, nbTicks);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                                      int fontType, double fontSize, int fontColor,
                                                      bool useFractionalMetrics)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor,
                                   fontType, fontSize, fontColor, useFractionalMetrics);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setAxisLineDrawing(bool drawAxisLine)
{
  m_pJavaObject->setAxisLineDrawing(drawAxisLine);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setNeedTicksDecimation(bool needTicksDecimation)
{
  m_pJavaObject->setNeedTicksDecimation(needTicksDecimation);
}
/*--------------------------------------------------------------------------*/
}
