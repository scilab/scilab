/*------------------------------------------------------------------------*/
/* file: XTicksDrawerJavaMapper.cpp                                       */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by XTicksDrawerJoGL        */
/*------------------------------------------------------------------------*/

#include "XTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
XTicksDrawerJavaMapper::XTicksDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::XTicksDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
XTicksDrawerJavaMapper::~XTicksDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                       int nbTicks, const double subticksPositions[], int nbSubticks)
{
  m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                           ticksLabels, nbTicks,
                           (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool XTicksDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks)
{
  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks);
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                           double yMin, double yMax,
                                           double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
void XTicksDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                                int fontType, double fontSize, int fontColor)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor,
                                   fontType, fontSize, fontColor);
}
/*--------------------------------------------------------------------------*/
}
