/*------------------------------------------------------------------------*/
/* file: GridDrawer.cpp                                                   */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing grid on subwin axes                */
/*------------------------------------------------------------------------*/

#include "GridDrawer.hxx"

namespace sciGraphics
{
/*------------------------------------------------------------------------------------------*/
GridDrawer::GridDrawer(DrawableSubwin * subwin)
  : DrawableObjectBridge()
{
  m_bIsLogModeOn = false;
}
/*------------------------------------------------------------------------------------------*/
GridDrawer::~GridDrawer(void)
{

}
/*------------------------------------------------------------------------------------------*/
void GridDrawer::draw(const double ticksPositions[], int nbTicks,
                      const double subticksPositions[], int nbSubticks)
{
  drawGrid(ticksPositions, nbTicks);

  // in logarithmic mode ticks must also been drawn in front of subticks
  if(m_bIsLogModeOn)
  {    
    drawGrid(subticksPositions, nbSubticks);
  }

}
/*------------------------------------------------------------------------------------------*/

}
