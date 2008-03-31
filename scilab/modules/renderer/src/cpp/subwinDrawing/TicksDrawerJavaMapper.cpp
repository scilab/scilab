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
void TicksDrawerJavaMapper::setFigureIndex(int figureIndex)
{
  m_pJavaObject->setFigureIndex(figureIndex);
}
/*--------------------------------------------------------------------------*/
double TicksDrawerJavaMapper::drawTicks(void)
{
  return m_pJavaObject->drawTicks();
}
/*--------------------------------------------------------------------------*/
bool TicksDrawerJavaMapper::checkTicks(void)
{
  return m_pJavaObject->checkTicks();
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setTicksPosition(const double ticksPosX[],
                                             const double ticksPosY[],
                                             const double ticksPosZ[],
                                             int nbTicks)
{
  m_pJavaObject->setTicksPosition((double *) ticksPosX, nbTicks,
                                  (double *) ticksPosY, nbTicks,
                                  (double *) ticksPosZ, nbTicks);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setSubticksPosition(const double subticksPosX[],
                                                const double subticksPosY[],
                                                const double subticksPosZ[],
                                                int nbSubticks)
{
  m_pJavaObject->setSubticksPosition((double *) subticksPosX, nbSubticks,
                                     (double *) subticksPosY, nbSubticks,
                                     (double *) subticksPosZ, nbSubticks);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setTicksLabels(char * labels[], char * exponents[], int nbLabels)
{
  m_pJavaObject->setTicksLabels((char **) labels, nbLabels,
                                (char **) exponents, nbLabels);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setTicksLabels(char * labels[], int nbLabels)
{
  m_pJavaObject->setTicksLabels((char **) labels, nbLabels);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setTicksDirection(double ticksDirX, double ticksDirY, double ticksDirZ)
{
  m_pJavaObject->setTicksDirection(ticksDirX, ticksDirY, ticksDirZ);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setAxisBounds(double axisStartX, double axisStartY, double axisStartZ,
                                          double axisEndX, double axisEndY, double axisEndZ)
{
  m_pJavaObject->setAxisBounds(axisStartX, axisStartY, axisStartZ, axisEndX, axisEndY, axisEndZ);
}
/*--------------------------------------------------------------------------*/
void TicksDrawerJavaMapper::setAxisParamerters(int lineStyle, float lineWidth, int lineColor,
                                               int fontType, double fontSize, int fontColor)
{
  m_pJavaObject->setAxisParameters(lineStyle, lineWidth, lineColor, fontType, fontSize, fontColor);
}
/*--------------------------------------------------------------------------*/
}

