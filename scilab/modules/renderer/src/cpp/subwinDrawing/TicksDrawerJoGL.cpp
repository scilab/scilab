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
TicksDrawerJoGL::TicksDrawerJoGL(DrawableSubwin * subwin)
  : TicksDrawer(subwin), DrawableObjectJoGL(subwin)
{

}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJoGL::~TicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void TicksDrawerJoGL::drawTicks(double ticksPositions[], char * ticksLabels[], char * labelsExponents[],
                                int nbTicks, double subticksPositions[], int nbSubtics)
{
  if (labelsExponents == NULL)
  {
    getTicksDrawerJavaMapper()->drawTicks(ticksPositions, ticksLabels, nbTicks,
                                          subticksPositions, nbSubtics);
  }
  else
  {
    getTicksDrawerJavaMapper()->drawTicks(ticksPositions, ticksLabels,
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
  sciPointObj * pSubwin = m_pSubwin->getDrawedObject();

  // set axes bounds
  double bounds[6];
  sciGetRealDataBounds(pSubwin, bounds);
  getTicksDrawerJavaMapper()->setAxesBounds(bounds[0], bounds[1], bounds[2],
                                            bounds[3], bounds[4], bounds[5]);
  // set other parameters
  getTicksDrawerJavaMapper()->setAxisParamerters(sciGetLineStyle(pSubwin),
                                                 (float) sciGetLineWidth(pSubwin),
                                                 sciGetGraphicContext(pSubwin)->foregroundcolor,
                                                 sciGetFontStyle(pSubwin),
                                                 sciGetFontSize(pSubwin),
                                                 sciGetFontContext(pSubwin)->foregroundcolor);

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
