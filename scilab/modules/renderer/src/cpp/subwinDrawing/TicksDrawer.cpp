/*------------------------------------------------------------------------*/
/* file: TicksDrawer.cpp                                                  */
/* Copyright INRIA 2007                                                   */
/* Authors : Jean-Baptiste Silvy                                          */
/* desc : Class specialized in drawing ticks                              */
/*------------------------------------------------------------------------*/

#include "TicksDrawer.hxx"

namespace sciGraphics
{

/*------------------------------------------------------------------------------------------*/
TicksDrawer::TicksDrawer(DrawableSubwin * subwin)
{
  m_pDrawer = subwin;
  m_pTicksComputer = NULL;
}
/*------------------------------------------------------------------------------------------*/
TicksDrawer::~TicksDrawer(void)
{
  if (m_pTicksComputer != NULL)
  {
    delete m_pTicksComputer;
    m_pTicksComputer = NULL;
  }
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::setTicksComputer(ComputeTicksStrategy * ticksComputer)
{
  m_pTicksComputer = ticksComputer;
}
/*------------------------------------------------------------------------------------------*/
void TicksDrawer::draw(void)
{
  if (m_pTicksComputer == NULL) { return; }
  int nbTicks = m_pTicksComputer->getNbTicks();
  int nbSubticks = m_pTicksComputer->getNbSubticks();

  // allocate positions and ticks
  char ** labels = new char *[nbTicks];
  double * ticksPos = new double[nbTicks];
  double * subticksPos = new double[nbSubticks];

  m_pTicksComputer->getTicksLabels(labels);
  m_pTicksComputer->getTicksPosition(ticksPos);
  m_pTicksComputer->getSubticksPosition(subticksPos);

  drawTicks(ticksPos, labels, nbTicks, subticksPos, nbSubticks);

  delete[] labels;
  delete[] ticksPos;
  delete[] subticksPos;

}
/*------------------------------------------------------------------------------------------*/

}
