/*
 * Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2007 - INRIA - Jean-Baptiste Silvy
 * desc : Class specialized in drawing ticks using JoGL
 * 
 * This file must be used under the terms of the CeCILL.
 * This source file is licensed as described in the file COPYING, which
 * you should have received as part of this distribution.  The terms
 * are also available at    
 * http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

#include "TicksDrawerJoGL.hxx"

extern "C"
{
#include "GetProperty.h"
}

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
TicksDrawerJoGL::TicksDrawerJoGL(DrawableObject * drawer)
  : TicksDrawer(drawer), DrawableObjectJoGL(drawer)
{
  setJavaMapper(new TicksDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJoGL::~TicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::setTicksPosition(const double ticksPosX[],
                                       const double ticksPosY[],
                                       const double ticksPosZ[],
                                       int nbTicks)
{
  // convert them to pixel coordinates
  

  getTicksDrawerJavaMapper()->setTicksPosition(ticksPosX, ticksPosY, ticksPosZ, nbTicks);
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::setSubticksPosition(const double subticksPosX[],
                                          const double subticksPosY[],
                                          const double subticksPosZ[],
                                          int nbSubticks)
{
  getTicksDrawerJavaMapper()->setSubticksPosition(subticksPosX, subticksPosY, subticksPosZ, nbSubticks);
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::setTicksLabels(char * ticksLabels[],
                                     char * labelsExponent[],
                                     int nbLabels)
{
  if (labelsExponent != NULL)
  {
    getTicksDrawerJavaMapper()->setTicksLabels(ticksLabels, labelsExponent, nbLabels);
  }
  else
  {
    getTicksDrawerJavaMapper()->setTicksLabels(ticksLabels, nbLabels);
  }
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::setAxisPosition(const double axisSegmentStart[3],
                                      const double axisSegmentEnd[3],
                                      const double ticksDirection[3])
{

  getTicksDrawerJavaMapper()->setAxisBounds(axisSegmentStart[0], axisSegmentStart[1], axisSegmentStart[2],
                                            axisSegmentEnd[0], axisSegmentEnd[1], axisSegmentEnd[2]);
  getTicksDrawerJavaMapper()->setTicksDirection(ticksDirection[0], ticksDirection[1], ticksDirection[2]);
}
/*------------------------------------------------------------------------------------------*/
double TicksDrawerJoGL::concreteDrawTicks(void)
{
  return getTicksDrawerJavaMapper()->drawTicks();
}
/*------------------------------------------------------------------------------------------*/
bool TicksDrawerJoGL::checkTicks(void)
{
  return getTicksDrawerJavaMapper()->checkTicks();
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::initializeDrawing(void)
{
  // initialize drawing as usual
  DrawableObjectJoGL::initializeDrawing();

  // also set axis drawing parameters
  sciPointObj * pObj = getDrawer()->getDrawedObject();

  // set other parameters
  getTicksDrawerJavaMapper()->setAxisParamerters(sciGetLineStyle(pObj),
                                                 (float) sciGetLineWidth(pObj),
                                                 sciGetGraphicContext(pObj)->foregroundcolor,
                                                 sciGetFontStyle(pObj),
                                                 sciGetFontSize(pObj),
                                                 sciGetFontContext(pObj)->foregroundcolor);

  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->initializeDrawing();
  }
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::endDrawing(void)
{
  DrawableObjectJoGL::endDrawing();
  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->endDrawing();
  }
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::initializeShowing(void)
{
  DrawableObjectJoGL::initializeDrawing();
  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->initializeShowing();
  }
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::endShowing(void)
{
  DrawableObjectJoGL::endDrawing();
  if (m_pGridDrawer != NULL)
  {
    m_pGridDrawer->endShowing();
  }
}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * TicksDrawerJoGL::getTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
