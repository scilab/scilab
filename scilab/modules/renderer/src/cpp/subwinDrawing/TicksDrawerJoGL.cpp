/*------------------------------------------------------------------------*/
/* file: TicksDrawerJoGL.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks using JoGL                   */
/*------------------------------------------------------------------------*/

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

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJoGL::~TicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
double TicksDrawerJoGL::drawTicks(double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                                  int nbTicks, double subticksPositions[], int nbSubtics)
{
  if (labelsExponents == NULL)
  {
    return getTicksDrawerJavaMapper()->drawTicks(ticksPositions, ticksLabels, nbTicks,
                                                 subticksPositions, nbSubtics);
  }
  else
  {
    return getTicksDrawerJavaMapper()->drawTicks(ticksPositions, ticksLabels,
                                                 labelsExponents, nbTicks,
                                                 subticksPositions, nbSubtics);
  }
  
}
/*------------------------------------------------------------------------------------------*/
bool TicksDrawerJoGL::checkTicks(double ticksPositions[], char * ticksLabels[],
                                 char * labelsExponents[], int nbTicks)
{
  if (labelsExponents == NULL)
  {
    return getTicksDrawerJavaMapper()->checkTicks(ticksPositions, ticksLabels, nbTicks);
  }
  else
  {
    return getTicksDrawerJavaMapper()->checkTicks(ticksPositions, ticksLabels,
                                                  labelsExponents, nbTicks);
  }
  
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::initializeDrawing(void)
{
  // initialize drawing as usual
  DrawableObjectJoGL::initializeDrawing();

  // also set axis drawing parameters
  sciPointObj * pObj = getDrawer()->getDrawedObject();

  // set axes bounds
  double bounds[6];
  sciGetRealDataBounds(sciGetParentSubwin(pObj), bounds);
  getTicksDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1], bounds[2],
                                            bounds[3], bounds[4], bounds[5]);
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
