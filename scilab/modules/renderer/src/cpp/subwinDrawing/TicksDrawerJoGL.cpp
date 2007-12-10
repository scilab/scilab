/*------------------------------------------------------------------------*/
/* file: TicksDrawerJoGL.cpp                                              */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks using JoGL                   */
/*------------------------------------------------------------------------*/

#include "TicksDrawerJoGL.hxx"

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
void TicksDrawerJoGL::drawTicks(double ticksPositions[], char * ticksLabels[], int nbTicks,
                                double subticksPositions[], int nbSubtics)
{
  getTicksDrawerJavaMapper()->drawTicks(ticksPositions, ticksLabels, nbTicks,
                                        subticksPositions, nbSubtics);
}
/*------------------------------------------------------------------------------------------*/
bool TicksDrawerJoGL::checkTicks(double ticksPositions[], char * ticksLabels[], int nbTicks)
{
  return getTicksDrawerJavaMapper()->checkTicks(ticksPositions, ticksLabels, nbTicks);
}
/*------------------------------------------------------------------------------------------*/
TicksDrawerJavaMapper * TicksDrawerJoGL::getTicksDrawerJavaMapper(void)
{
  return dynamic_cast<TicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/

}
