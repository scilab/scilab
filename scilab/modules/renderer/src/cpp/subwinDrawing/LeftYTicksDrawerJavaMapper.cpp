/*------------------------------------------------------------------------*/
/* file: LeftYTicksDrawerJavaMapper.cpp                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class containing java methods needed by YTicksDrawerJoGL        */
/*------------------------------------------------------------------------*/


#include "LeftYTicksDrawerJavaMapper.hxx"

extern "C"
{
#include "getScilabJavaVM.h"
}

namespace sciGraphics
{

/*--------------------------------------------------------------------------*/
LeftYTicksDrawerJavaMapper::LeftYTicksDrawerJavaMapper(void)
{
  m_pJavaObject = new org_scilab_modules_renderer_subwinDrawing::LeftYTicksDrawerGL(getScilabJavaVM());
}
/*--------------------------------------------------------------------------*/
LeftYTicksDrawerJavaMapper::~LeftYTicksDrawerJavaMapper(void)
{
  delete m_pJavaObject;
  m_pJavaObject = NULL;
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::display(void)
{
  m_pJavaObject->display();
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::initializeDrawing(int figureIndex)
{
  m_pJavaObject->initializeDrawing(figureIndex);
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::endDrawing(void)
{
  m_pJavaObject->endDrawing();
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::show(int figureIndex)
{
  m_pJavaObject->show(figureIndex);
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::destroy(int parentFigureIndex)
{
  m_pJavaObject->destroy(parentFigureIndex);
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                           int nbTicks, const double subticksPositions[], int nbSubticks)
{
  m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                           ticksLabels, nbTicks,
                           (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool LeftYTicksDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[], int nbTicks)
{
  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks);
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::drawTicks(const double ticksPositions[], char * ticksLabels[],
                                           char * labelsExponents[], int nbTicks,
                                           const double subticksPositions[], int nbSubticks)
{
  m_pJavaObject->drawTicks((double *)ticksPositions, nbTicks,
                           ticksLabels, nbTicks,
                           labelsExponents, nbTicks,
                           (double *)subticksPositions, nbSubticks);
}
/*--------------------------------------------------------------------------*/
bool LeftYTicksDrawerJavaMapper::checkTicks(const double ticksPositions[], char * ticksLabels[],
                                            char * labelsExponents[], int nbTicks)
{

  return m_pJavaObject->checkTicks((double *)ticksPositions, nbTicks,
                                   ticksLabels, nbTicks,
                                   labelsExponents, nbTicks);
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::setAxesBounds(double xMin, double xMax,
                                               double yMin, double yMax,
                                               double zMin, double zMax)
{
  m_pJavaObject->setAxesBounds(xMin, xMax, yMin, yMax, zMin, zMax);
}
/*--------------------------------------------------------------------------*/
void LeftYTicksDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                                    int fontType, double fontSize, int fontColor)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor,
                                   fontType, fontSize, fontColor);
}
/*--------------------------------------------------------------------------*/
}

