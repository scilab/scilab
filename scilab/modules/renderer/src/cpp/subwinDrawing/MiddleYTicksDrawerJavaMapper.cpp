/*------------------------------------------------------------------------*/
/* file: MiddleYTicksDrawerJavaMapper.cpp                                 */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by YTicksDrawerJoGL        */
/*------------------------------------------------------------------------*/


#include "MiddleYTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
MiddleYTicksDrawerJavaMapper::MiddleYTicksDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::MiddleYTicksDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
MiddleYTicksDrawerJavaMapper::~MiddleYTicksDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                             int nbTicks, const double subticksPositions[], int nbSubticks)
{
  m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                           ticksLabels, nbTicks,
                           (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool MiddleYTicksDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks)
{
  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks);
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                             char * labelsExponents[], int nbTicks,
                                             const double subticksPositions[], int nbSubticks)
{
  m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                           ticksLabels, nbTicks,
                           labelsExponents, nbTicks,
                           (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool MiddleYTicksDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[],
                                              char * labelsExponents[], int nbTicks)
{

  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks,
                                   labelsExponents, nbTicks);
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                                 double yMin, double yMax,
                                                 double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
void MiddleYTicksDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                                      int fontType, double fontSize, int fontColor)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor,
                                   fontType, fontSize, fontColor);
}
/*--------------------------------------------------------------------------*/
}

