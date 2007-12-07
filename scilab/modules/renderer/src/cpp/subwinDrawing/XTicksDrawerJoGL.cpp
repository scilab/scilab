/*------------------------------------------------------------------------*/
/* file: XTicksDrawerJoGL.cpp                                             */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks for X axis                   */
/*------------------------------------------------------------------------*/

#include "XTicksDrawerJoGL.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::XTicksDrawerJoGL(DrawableSubwin * subwin)
 : TicksDrawer(subwin), DrawableObjectJoGL(subwin)
{
  setJavaMapper(new XTicksDrawerJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJoGL::~XTicksDrawerJoGL(void)
{

}
/*------------------------------------------------------------------------------------------*/
void XTicksDrawerJoGL::drawTicks(double ticksPositions[], char * ticksLabels[], int nbTicks,
                                 double subticksPositions[], int nbSubtics)
{
  initializeDrawing();
  getXTicksDrawerJavaMapper()->drawTicks(ticksPositions, ticksLabels, nbTicks,
                                         subticksPositions, nbSubtics);
  endDrawing();
}
/*------------------------------------------------------------------------------------------*/
XTicksDrawerJavaMapper * XTicksDrawerJoGL::getXTicksDrawerJavaMapper(void)
{
  return dynamic_cast<XTicksDrawerJavaMapper *>(getJavaMapper());
}
/*------------------------------------------------------------------------------------------*/
}