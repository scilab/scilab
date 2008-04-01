/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2008 - INRIA - Jean-Baptiste Silvy
 * desc : Class containing java methods needed by XAxesDrawerJoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "XAxesDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
XAxesDrawerJavaMapper::XAxesDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_axesDrawing::XAxesDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
XAxesDrawerJavaMapper::~XAxesDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
double XAxesDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                         int nbTicks, const double subticksPositions[], int nbSubticks)
{
  return m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                                  ticksLabels, nbTicks,
                                  (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool XAxesDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks)
{
  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks);
}
/*--------------------------------------------------------------------------*/
double XAxesDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                         char * labelsExponents[], int nbTicks,
                                         const double subticksPositions[], int nbSubticks)
{
  return m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                                  ticksLabels, nbTicks,
                                  labelsExponents, nbTicks,
                                 (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool XAxesDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[],
                                        char * labelsExponents[], int nbTicks)
{

  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks,
                                   labelsExponents, nbTicks);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                           double yMin, double yMax,
                                           double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                                int fontType, double fontSize, int fontColor)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor,
                                   fontType, fontSize, fontColor);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::setConstantCoordinates(double yCoordinate, double zCoordinate)
{
  m_pJavaObject->setConstantCoordinates(yCoordinate, zCoordinate);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::setTicksDirection(char dir)
{
  // giws doesn't not support char yet.
  // so create a C string with 1 character
  char dirString[2] = {dir, 0};
  m_pJavaObject->setTicksDirection(dirString);
}
/*--------------------------------------------------------------------------*/
void XAxesDrawerJavaMapper::setAxisLineDrawing(bool drawAxisLine)
{
  m_pJavaObject->setAxisLineDrawing(drawAxisLine);
}
/*--------------------------------------------------------------------------*/
}
